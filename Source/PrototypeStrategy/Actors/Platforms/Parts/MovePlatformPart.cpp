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
		case EDynamic::Active:
		{
			MoveToLocationFloor(Box);
			PlaySound(moveSound);
			GetWorldTimerManager().SetTimer(DynamicTimer, this, &AMovePlatformPart::StartActive, ActiveTimeStep, true);
		
			break;
		}
		default:
			PlaySound(moveSound);
			MoveToLocationFloor(Box);
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
		case EBoxType::Wall:
		{		
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("MovePart = Wall")));
			AWallColorPlatformPart* ColorWall = Cast<AWallColorPlatformPart>(Box);
			if (IsValid(ColorWall))
			{
				switch (ColorWall->GetWallType())
				{
					case EWallType::ColorWall:
					{
						SetActorLocation(FloorLocation);
						break;
					}
					default: 
						break;
				}
			
			}
				break;
		}

		default:
			SetActorLocation(FloorLocation);
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
			PlaySound(interactSound);
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
