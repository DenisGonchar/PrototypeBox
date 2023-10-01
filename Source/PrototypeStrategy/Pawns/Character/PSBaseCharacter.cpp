
#include "Pawns/Character/PSBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include <PSTypes.h>
#include <Components/LedgeDetectorComponent.h>
#include <Actors/Platforms/PSPlatformPart.h>
#include <Kismet/GameplayStatics.h>
#include <Actors/Platforms/Parts/BlockPlatformPart.h>
#include <GameState/PSGameState.h>
#include <GameMode/PSGameMode.h>
#include "Actors/Platforms/Parts/MagneticPlatformPart.h"
#include "Actors/Platforms/Parts/TeleportPlatformPart.h"
#include "Actors/Platforms/Parts/CoverPlatformPart.h"
#include "Actors/Platforms/Parts/PathPlatformPart.h"
#include "Actors/Platforms/Parts/ExitPlatformPart.h"
#include "PaperFlipbookComponent.h"
#include "Actors/Platforms/Parts/WallColorPlatformPart.h"

APSBaseCharacter::APSBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(BoxComponent);
	Flipbook->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	/*CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	CharacterMesh->SetupAttachment(BoxComponent);*/

	LedgeDetertorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));

}

void APSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GMode = Cast<APSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameInstance = Cast<UPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FullStep();
	
	FLedgeDescription LedgeDescription;

	if (LedgeDetertorComponent->StartDetectDown(LedgeDescription))
	{
		ABlockPlatformPart* BoxBlock = Cast<ABlockPlatformPart>(LedgeDescription.BoxMesh);
		if (IsValid(BoxBlock))
		{
			BoxType = BoxBlock->GetBoxType();
			switch (BoxType)
			{
				case EBoxType::Path:
				{
					MoveToPositionStart(BoxBlock);
				
					break;
				}
			}
		}	
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(LevelType)));

}

void APSBaseCharacter::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

}

void APSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Purple, bIsMoveFinished ? "true" : "false");

}

void APSBaseCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);	
}

float APSBaseCharacter::GetMoveDistance() const
{
	return MoveDistance;
}

EMoveCharacterDirection APSBaseCharacter::GetCharacterDirection() const
{
	return CharacterDirection;
}

void APSBaseCharacter::MoveTop()
{
	CharacterDirection = EMoveCharacterDirection::Top;
	IsPlayerStep = true;
	MovementDirection(CharacterDirection);
}

void APSBaseCharacter::MoveDown()
{
	CharacterDirection = EMoveCharacterDirection::Down;
	IsPlayerStep = true;
	MovementDirection(CharacterDirection);
}

void APSBaseCharacter::MoveRight()
{
	CharacterDirection = EMoveCharacterDirection::Right;
	IsPlayerStep = true;
	MovementDirection(CharacterDirection);
}

void APSBaseCharacter::MoveLeft()
{
	CharacterDirection = EMoveCharacterDirection::Left;
	IsPlayerStep = true;
	MovementDirection(CharacterDirection);
}


void APSBaseCharacter::MovementDirection(EMoveCharacterDirection Direction)
{
	if (!bIsMoveFinished)
	{
		return;
	}
	bIsMoveFinished = false;

	FLedgeDescription LedgeDescription;
	if (LedgeDetertorComponent->DetectLedge(LedgeDescription, Direction))
	{
		lastDirection = Direction;
		detectedBlock = LedgeDescription.BoxMesh;
		if (LedgeDetertorComponent->bIsNeedPush)
		{
			deltaEquals = 5.f;
			PushAndMove();
		}
		else
		{
			deltaEquals = 40.f;
			MoveWithAnim();
		}		
	}
	else
	{
		bIsMoveFinished = true;
		return;
	}
}

void APSBaseCharacter::MoveToLocationType()
{	
	ABlockPlatformPart* BoxBlock = Cast<ABlockPlatformPart>(detectedBlock);

	if (IsValid(BoxBlock))
	{
		BoxType = BoxBlock->GetBoxType();
	}

	switch (BoxType)
	{
		case EBoxType::Path:
		{
			APathPlatformPart* Path = Cast<APathPlatformPart>(detectedBlock);
			if (IsValid(Path))
			{
				Path->PlaySound(Path->interactSound);
				switch (LevelType)
				{
				case ELevelType::Level:
				{
					MoveToPosition(detectedBlock);

					break;
				}

				case ELevelType::UnderCover:
				{
					if (IsValid(BoxBlock))
					{
						if (BoxBlock->GetbIsActivatorCover())
						{
							BoxBlock->ActivatorCover();

							MoveToPosition(detectedBlock);
						}
					}

				}
				default:
					break;
				}
			}
			break;
		}

		case EBoxType::Exit:
		{
			AExitPlatformPart* ExitPart = Cast<AExitPlatformPart>(BoxBlock);
			MoveToPosition(detectedBlock);
			ExitPart->PlaySound(ExitPart->interactSound);
			if (ExitPart->GetLevelType() == ELevelType::UnderCover)
			{
				if (ExitPart->GetIsActivCaver())
				{
					NextLevel = ExitPart->GetOpenLevel();

					if (OpenLevel.IsBound())
					{
						OpenLevel.Broadcast(GMode->GetWin());
					}

				}
			}
			else if (ExitPart->GetLevelType() == ELevelType::Level)
			{
				if (ExitPart->GetIsActivLevel())
				{
					NextLevel = ExitPart->GetOpenLevel();

					if (OpenLevel.IsBound())
					{
						OpenLevel.Broadcast(GMode->GetWin());
					}
				}
			}
			
			break;
		}

		case EBoxType::Cover:
		{
			ACoverPlatformPart* BoxCover = Cast<ACoverPlatformPart>(BoxBlock);
			
			if (IsValid(BoxCover))
			{
				MoveToPosition(detectedBlock);
				BoxCover->PlaySound(BoxCover->interactSound);
				BoxCover->ActivatorCover();
				
				LevelType = BoxCover->GetLevelType();
			}
			
			break;
		}
		
		case EBoxType::Teleport:
		{
			ATeleportPlatformPart* BoxTeleport = Cast<ATeleportPlatformPart>(BoxBlock);

			if (BoxTeleport->GetTeleportInfo().TeleportType == ETeleport::Activator)
			{				
				MoveToPosition(detectedBlock);
				BoxTeleport->PlaySound(BoxTeleport->activateSound);
				BoxTeleport->SwitchActivator();

			}
			else if (BoxTeleport->GetTeleportInfo().TeleportType == ETeleport::Teleport)
			{
				if (BoxTeleport->GetTeleportInfo().bIsTeleportType)
				{
					if (IsValid(BoxTeleport->GetTeleport()))
					{
						FVector FloorLocation = BoxTeleport->GetTeleport()->GetActorLocation();
						FloorLocation.Z = GetActorLocation().Z;
						//UGameplayStatics::PlaySoundAtLocation(GetWorld(), BoxTeleport->interactSound, BoxTeleport->GetActorLocation());
						BoxTeleport->PlaySound(BoxTeleport->interactSound);
						SetActorLocation(FloorLocation);
						bIsMoveFinished = true;
						Step(StepIndex);

					}					
					
				}
				else
				{
					MoveToPosition(detectedBlock);
				}
			}
			else
			{
				MoveToPosition(detectedBlock);
			}

			break;
		}
		
		//if move to magnetic activator
		case EBoxType::Magnetic:
		{
			AMagneticPlatformPart* MagneticBox = Cast<AMagneticPlatformPart>(BoxBlock);

			if (MagneticBox->MagneticType == EMagneticType::Activator)
			{
				MoveToPosition(detectedBlock);
				MagneticBox->PlaySound(MagneticBox->interactSound);
				MagneticBox->SwitchActivator();
			}			
		}

		case EBoxType::Polarizator:
		{
			AMagneticPlatformPart* magneticPart = Cast<AMagneticPlatformPart>(BoxBlock);

			if (IsValid(magneticPart))
			{
				if (magneticPart->MagneticType == EMagneticType::Polarizer)
				{
					magneticPart->PlaySound(magneticPart->interactSound);
					switch (polarizationType)
					{
						case EPolarizationType::None:
							polarizationType = EPolarizationType::Positive;
							break;
						case EPolarizationType::Positive:
							polarizationType = EPolarizationType::Negative;
							break;
						case EPolarizationType::Negative:
							polarizationType = EPolarizationType::None;
							break;
					default:
						break;
					}
					magneticPart->SwitchSprite(polarizationType);
				}
			}
			MoveToPosition(detectedBlock);
		}

	case EBoxType::Wall:
	{
		/*AWallColorPlatformPart* WallColor = Cast<AWallColorPlatformPart>(BoxBlock);
		if (IsValid(WallColor))
		{
			
		}*/
		BoxBlock->PlaySound(BoxBlock->interactSound);
		MoveToPosition(detectedBlock);
	}
		default:
			break;
	}
	

}

void APSBaseCharacter::MoveToPosition(APSPlatformPart* Box)
{
	if (IsPlayerStep)
	{
		Step(StepIndex);
	}

	if (IsValid(Box))
	{
		FVector FloorLocation = Box->GetActorLocation();
		FloorLocation.Z = GetActorLocation().Z;
		startLocation = GetActorLocation();
		targetLocation = FloorLocation;
		deltaLocation = (startLocation - targetLocation) * moveTimerSpeed;
		deltaLocation = deltaLocation * -8.f;
		bIsMoveFinished = false;
		bIsPushMove = false;
		GetWorldTimerManager().SetTimer(moveTimerHandle, this, &APSBaseCharacter::MoveCharacterOnTimer, moveTimerSpeed, true);					
	}	
}

void APSBaseCharacter::MoveCharacterOnTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x = %f"), deltaEquals));
	if (targetLocation.Equals(GetActorLocation(), deltaEquals))
	{	
		SetActorLocation(targetLocation);
		GetWorldTimerManager().PauseTimer(moveTimerHandle);
		GetWorldTimerManager().ClearTimer(moveTimerHandle);
		if (GetWorldTimerManager().IsTimerActive(playFlipbookAnimHandle))
		{
			GetWorldTimerManager().PauseTimer(playFlipbookAnimHandle);
			GetWorldTimerManager().ClearTimer(playFlipbookAnimHandle);
		}

		bIsMoveFinished = true;
		PlayAnimation(idleAnim);
		if (gameInstance->OnMove.IsBound())
		{
			gameInstance->OnMove.Broadcast();
		}

		return;
	}
	SetActorLocation(GetActorLocation() + deltaLocation);
}

void APSBaseCharacter::MoveToPosition(FVector location)
{	
	startLocation = GetActorLocation();
	targetLocation = location;
	deltaLocation = (deltaLocation - targetLocation) * moveTimerSpeed;
	deltaLocation = deltaLocation * -8.f;
	bIsMoveFinished = false;
	bIsPushMove = false;
	GetWorldTimerManager().SetTimer(moveTimerHandle, this, &APSBaseCharacter::MoveCharacterOnTimer, moveTimerSpeed, true);
}

void APSBaseCharacter::MoveToPositionStart(APSPlatformPart* Box)
{
	FVector FloorLocation = Box->GetActorLocation();
	FloorLocation.Z = GetActorLocation().Z;

	SetActorLocation(FloorLocation);
}

void APSBaseCharacter::AddActualMagnetics(AMagneticPlatformPart* part)
{
	ActiveMagnetics.AddUnique(part);
	if (!IsMagneticFindStarted)
	{
		GetWorldTimerManager().SetTimer(startFindMagnetic, this, &APSBaseCharacter::FindNearestMagnetic, 0.1f, false);
		IsMagneticFindStarted = true;
	}
}

void APSBaseCharacter::FindNearestMagnetic()
{
	float minDistance = 9999;
	AMagneticPlatformPart* nearestPart = nullptr;

	for(auto part : ActiveMagnetics)
	{
		float Distance = GetDistanceTo(part);
		if(Distance < minDistance)
		{
			minDistance = Distance;
			nearestPart = part;
		}
	}

	ActiveMagnetics.Empty();
	IsMagneticFindStarted = false;
	GetWorldTimerManager().ClearTimer(startFindMagnetic);
	if (IsValid(nearestPart))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, nearestPart->GetName());
		nearestPart->MagneticPlayer(this);
	}
}


void APSBaseCharacter::Step(int Index)
{
	if (IsValid(GMode))
	{
		GMode->MadeStep(StepIndex);
	}
}

void APSBaseCharacter::FullStep()
{
	if (IsValid(GMode))
	{
		GMode->SetFullSteps(FullStepIndex);
	}
}

void APSBaseCharacter::SetFullSteps(int32 Step)
{
	FullStepIndex = Step;
}

EBoxType APSBaseCharacter::GetBoxType()
{
	return BoxType;
}

ELevelType APSBaseCharacter::GetLevelType()
{
	return LevelType;
}

FName APSBaseCharacter::GetNameMap() const
{
	return NameMap;
}

void APSBaseCharacter::SetNameMap(FName Map)
{
	NameMap = Map;
}

FName APSBaseCharacter::GetNestLevel() const
{
	return NextLevel;
}

void APSBaseCharacter::SetNextLevel(FName Map)
{
	NextLevel = Map;
}

void APSBaseCharacter::PlayAnimation(UPaperFlipbook* animToPlay)
{
	//CharacterMesh->PlayAnimation(animToPlay, false);
	Flipbook->SetFlipbook(animToPlay);
}

void APSBaseCharacter::PushAndMove()
{
	PlayAnimation(GetAnimationByDirection(lastDirection, true));
	PrepearMove();
}

void APSBaseCharacter::PrepearMove()
{
	startLocation = GetActorLocation();
	targetLocation = GetPushImpulseMoveLocation(lastDirection);
	deltaLocation = (startLocation - targetLocation) * moveTimerSpeed;
	deltaLocation = deltaLocation * -2.f;
	bIsMoveFinished = false;	
	GetWorldTimerManager().SetTimer(moveTimerHandle, this, &APSBaseCharacter::MoveCharacterOnTimer, moveTimerSpeed, true);
	GetWorldTimerManager().SetTimer(pushTimer, this, &APSBaseCharacter::MoveWithAnim, 0.1f, false, pushDelay);
}

void APSBaseCharacter::MoveWithAnim()
{
	deltaEquals = 40.f;
	bIsPushMove = true;
	PlayAnimation(GetAnimationByDirection(lastDirection));
	GetWorldTimerManager().SetTimer(moveTimer, this, &APSBaseCharacter::MoveToLocationType, 0.1f, false, moveDelay);
}

UPaperFlipbook* APSBaseCharacter::GetAnimationByDirection(EMoveCharacterDirection direction, bool bIsPushAnim)
{
	switch (direction)
	{
		case EMoveCharacterDirection::Top:
			if (bIsPushAnim)
			{
				return pushForward;
			}
			return moveForward;
			break;
		case EMoveCharacterDirection::Down:			
			if (bIsPushAnim)
			{
				return pushBack;
			}
			return moveBack;
			break;
		case EMoveCharacterDirection::Left:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Left");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, moveLeft ? "true" : "false");
			if (bIsPushAnim)
			{
				return pushLeft;
			}
			return moveLeft;
			break;
		case EMoveCharacterDirection::Right:
			if (bIsPushAnim)
			{
				return pushRight;
			}
			return moveRight;
			break;
	}
	return nullptr;
}

FVector APSBaseCharacter::GetPushImpulseMoveLocation(EMoveCharacterDirection direc)
{
	switch (direc)
	{
		case EMoveCharacterDirection::Top:
			return GetActorLocation() - FVector( 25.f, 0.f, 0.f);
			break;
		case EMoveCharacterDirection::Down:
			return GetActorLocation() - FVector( -25.f, 0.f, 0.f);
			break;
		case EMoveCharacterDirection::Right:
			return GetActorLocation() - FVector( 0.f, 25.f, 0.f);
			break;
		case EMoveCharacterDirection::Left:
			return GetActorLocation() - FVector( 0.f, -25.f, 0.f);
			break;
		default:
			return GetActorLocation();
			break;
	}
}

