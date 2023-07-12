
#include "Pawns/Character/PSBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include <PSTypes.h>
#include <Components/LedgeDetectorComponent.h>
#include <Actors/Platforms/PSPlatformPart.h>
#include <Kismet/GameplayStatics.h>
#include <Actors/Platforms/Parts/BlockPlatformPart.h>
#include <GameMode/PSGameMode.h>
#include <GameState/PSGameState.h>
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

	LedgeDetertorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));

}

void APSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GMode = Cast<APSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
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
	
	if (Direction == EMoveCharacterDirection::Top || Direction == EMoveCharacterDirection::Down)
	{
		Flipbook->SetRelativeScale3D(FVector(0.6f,0.5f, 0.4f));
	}
	else if (Direction == EMoveCharacterDirection::Left || Direction == EMoveCharacterDirection::Right)
	{
		Flipbook->SetRelativeScale3D(FVector(0.4f, 0.5f, 0.6f));
	}


	FLedgeDescription LedgeDescription;
	if (LedgeDetertorComponent->DetectLedge(LedgeDescription, Direction))
	{
		MoveToLocationType(LedgeDescription.BoxMesh);
	}
	GetWorldTimerManager().SetTimer(spriteTimer,this,&APSBaseCharacter::NormalizePlayerFlipbook,0.1f,false);
}

void APSBaseCharacter::NormalizePlayerFlipbook()
{
	Flipbook->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void APSBaseCharacter::MoveToLocationType(APSPlatformPart* Box)
{	
	ABlockPlatformPart* BoxBlock = Cast<ABlockPlatformPart>(Box);
	
	if (IsValid(BoxBlock))
	{
		BoxType = BoxBlock->GetBoxType();
	}

	switch (BoxType)
	{
		case EBoxType::Path:
		{
			switch (LevelType)
			{
				case ELevelType::Level:
				{
					MoveToPosition(Box);

					break;
				}

				case ELevelType::UnderCover:
				{
					if (IsValid(BoxBlock))
					{
						if (BoxBlock->GetbIsActivatorCover())
						{
							BoxBlock->ActivatorCover();

							MoveToPosition(Box);
						}
					}
					
				}
				default:
					break;			
			}

			break;
		}

		case EBoxType::Exit:
		{
			AExitPlatformPart* ExitPart = Cast<AExitPlatformPart>(BoxBlock);
			MoveToPosition(Box);

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
				MoveToPosition(Box);

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
				MoveToPosition(Box);

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

						SetActorLocation(FloorLocation);
						
						Step(StepIndex);

					}					
					
				}
				else
				{
					MoveToPosition(Box);
				}
			}
			else
			{
				MoveToPosition(Box);
			}

			break;
		}
		
		//if move to magnetic activator
		case EBoxType::Magnetic:
		{
			AMagneticPlatformPart* MagneticBox = Cast<AMagneticPlatformPart>(BoxBlock);

			if (MagneticBox->MagneticType == EMagneticType::Activator)
			{
				MoveToPosition(Box);

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
			MoveToPosition(Box);
		}

	case EBoxType::Wall:
	{
		/*AWallColorPlatformPart* WallColor = Cast<AWallColorPlatformPart>(BoxBlock);
		if (IsValid(WallColor))
		{
			
		}*/
		MoveToPosition(Box);
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
		SetActorLocation(FloorLocation);			
	}
	
}

void APSBaseCharacter::MoveToPosition(FVector location)
{	
		SetActorLocation(location);	
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
		GetWorldTimerManager().SetTimer(startFindMagnetic,this,&APSBaseCharacter::FindNearestMagnetic,0.1f,false);
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
		nearestPart->Magnetic(this);
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

