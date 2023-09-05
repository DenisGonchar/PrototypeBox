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
	lastDirection = Direc;
	FLedgeDescription LedgeDescription;
	LedgeDetertorComponent->lastPlatformPart = this;
	if (LedgeDetertorComponent->BoxDetectLedge(/*this,*/ LedgeDescription, Direc))
	{
		lastBox = LedgeDescription.BoxMesh;
		//DirectionDynamicType();
		GetWorldTimerManager().SetTimer( moveDelayTimer, this, &AMovePlatformPart::DirectionDynamicType, moveDelay, false, 0.3f);
		return true;
	}

	return false;
}

void AMovePlatformPart::DirectionDynamicType()
{
	switch (DynamicType)
	{	
		case EDynamic::Active:
		{
			MoveToLocationFloor(lastBox);
			PlaySound(moveSound);
			GetWorldTimerManager().SetTimer(DynamicTimer, this, &AMovePlatformPart::StartActive, ActiveTimeStep, true);
		
			break;
		}
		default:
			PlaySound(moveSound);
			MoveToLocationFloor(lastBox);
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
	
	if (lastDirection != EMoveCharacterDirection::None)
	{
		FLedgeDescription LedgeDescription;
		if (LedgeDetertorComponent->BoxDetectLedge(/*this,*/ LedgeDescription, lastDirection))
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
