// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include <Components/LedgeDetectorComponent.h>
#include <PSTypes.h>

#include "PaperFlipbookComponent.h"
#include "WallColorPlatformPart.h"

AMovePlatformPart::AMovePlatformPart()
{
	PrimaryActorTick.bCanEverTick = true;

	LedgeDetertorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));

}

void AMovePlatformPart::BeginPlay()
{
	Super::BeginPlay();

}

bool AMovePlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	Direction = Direc;	
	FLedgeDescription LedgeDescription;
	if (LedgeDetertorComponent->BoxDetectLedge(LedgeDescription, Direc))
	{
		DirectionDynamicType(LedgeDescription.BoxMesh);

		return true;
	}

	return false;
}

void AMovePlatformPart::DirectionDynamicType(APSPlatformPart* Box)
{
	switch (DynamicType)
	{
	case EDynamic::Passive:
	{
		MoveToLocationFloor(Box);
		break;
	}	
	case EDynamic::Active:
	{
		MoveToLocationFloor(Box);

		GetWorldTimerManager().SetTimer(DynamicTimer, this, &AMovePlatformPart::StartActive, ActiveTimeStep, true);
		
		break;
	}
	case EDynamic::Limited:
		{
			MoveToLocationFloor(Box);
			break;
		}
	default:
		break;
	}
}

void AMovePlatformPart::MoveToLocationFloor(APSPlatformPart* Box)
{
	
	EBoxType Type = Box->GetBoxType();

	FVector FloorLocation = Box->GetActorLocation();
	FloorLocation.Z = GetActorLocation().Z;

	switch (Type)
	{
	case EBoxType::Path:
	{
		
		SetActorLocation(FloorLocation);

		break;
	}

	case EBoxType::Cover:
	{

		SetActorLocation(FloorLocation);

		break;
	}

	case EBoxType::Exit:
	{
		
		SetActorLocation(FloorLocation);

		break;
	}

	case EBoxType::Teleport:
	{

		SetActorLocation(FloorLocation);

		break;
	}
	case EBoxType::Wall:
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("MovePart = Wall")));
		AWallColorPlatformPart* ColorWall = Cast<AWallColorPlatformPart>(Box);
		if (IsValid(ColorWall))
		{
			switch (ColorWall->GetWallType())
			{
			case EWallType::None: break;
			case EWallType::DefaultWall: break;
			case EWallType::CrackedWall: break;
			case EWallType::ColorWall:
				{
					SetActorLocation(FloorLocation);
					break;
				}
			default: ;
			}
			
		}
			break;
	}

	default:
		break;
	}
	

}

void AMovePlatformPart::StartActive()
{
	
	if (Direction != EMoveCharacterDirection::None)
	{
		FLedgeDescription LedgeDescription;
		if (LedgeDetertorComponent->BoxDetectLedge(LedgeDescription, Direction))
		{
			MoveToLocationFloor(LedgeDescription.BoxMesh);
			UE_LOG(LogTemp, Warning, TEXT(" Move"));

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" else"));

			GetWorldTimerManager().ClearTimer(DynamicTimer);
				
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DynamicTimer);
	}

}

EDynamic AMovePlatformPart::GetDynamicType() const
{
	return DynamicType;
}

void AMovePlatformPart::NewLevelType()
{
	Super::NewLevelType();


}


float AMovePlatformPart::GetMoveDistance() const
{
	return MoveDistance;
}
