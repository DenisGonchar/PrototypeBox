
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
#include "Actors/Platforms/Parts/TeleportPlatformPart.h"
#include "Actors/Platforms/Parts/CoverPlatformPart.h"
#include "Actors/Platforms/Parts/PathPlatformPart.h"

APSBaseCharacter::APSBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(BoxComponent);

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

	MovementDirection(CharacterDirection);
}

void APSBaseCharacter::MoveDown()
{
	CharacterDirection = EMoveCharacterDirection::Down;

	MovementDirection(CharacterDirection);

}

void APSBaseCharacter::MoveRight()
{
	CharacterDirection = EMoveCharacterDirection::Right;

	MovementDirection(CharacterDirection);

}

void APSBaseCharacter::MoveLeft()
{
	CharacterDirection = EMoveCharacterDirection::Left;

	MovementDirection(CharacterDirection);

}

void APSBaseCharacter::MovementDirection(EMoveCharacterDirection Direction)
{
	FLedgeDescription LedgeDescription;
	if (LedgeDetertorComponent->DetectLedge(LedgeDescription, CharacterDirection))
	{
		MoveToLocationType(LedgeDescription.BoxMesh);
	}
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
					if (BoxBlock->GetbIsActivatorCover())
					{
						BoxBlock->ActivatorCover();
						
						MoveToPosition(Box);
					}

// 					APathPlatformPart* BoxPath = Cast<APathPlatformPart>(BoxBlock);
// 					if (IsValid(BoxPath))
// 					{
// 						
// 					}
				}
			}

			break;
		}

		case EBoxType::Exit:
		{
			MoveToPosition(Box);

			NextLevel = BoxBlock->GetOpenLevel();

			if (OpenLevel.IsBound())
			{
				OpenLevel.Broadcast(GMode->GetWin());
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
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(LevelType)));

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
	}
	

}

void APSBaseCharacter::MoveToPosition(APSPlatformPart* Box)
{
	if (IsValid(Box))
	{
		FVector FloorLocation = Box->GetActorLocation();
		FloorLocation.Z = GetActorLocation().Z;

		SetActorLocation(FloorLocation);

		Step(StepIndex);

	}
	
}

void APSBaseCharacter::MoveToPositionStart(APSPlatformPart* Box)
{
	FVector FloorLocation = Box->GetActorLocation();
	FloorLocation.Z = GetActorLocation().Z;

	SetActorLocation(FloorLocation);
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

