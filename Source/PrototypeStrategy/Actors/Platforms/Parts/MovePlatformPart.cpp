// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include <Components/LedgeDetectorComponent.h>
#include <PSTypes.h>

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

		for (int i = 0; i < MaxStepIndex; i++)
		{
			//UE_LOG(LogTemp, Warning, TEXT(" for = %i"), i);

			if (Direction != EMoveCharacterDirection::None)
			{
				FLedgeDescription LedgeDescription;
				if (LedgeDetertorComponent->BoxDetectLedge(LedgeDescription, Direction))
				{
					MoveToLocationFloor(LedgeDescription.BoxMesh);
					UE_LOG(LogTemp, Warning, TEXT(" Move = %i"), i);

				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT(" else = %i"), i);

					break;
				}
			}
			
		}

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

	case EBoxType::Exit:
	{
		
		SetActorLocation(FloorLocation);

		break;
	}

	default:
		break;
	}
	

}

EDynamic AMovePlatformPart::GetDynamicType() const
{
	return DynamicType;
}

float AMovePlatformPart::GetMoveDistance() const
{
	return MoveDistance;
}
