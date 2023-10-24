// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/LimitedMovePlatformPart.h"

#include "PaperFlipbookComponent.h"
#include "WallColorPlatformPart.h"
#include "Components/LedgeDetectorComponent.h"


ALimitedMovePlatformPart::ALimitedMovePlatformPart()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ALimitedMovePlatformPart::GenerateMoveLimit()
{
	moveLimit = FMath::RandRange(2,6);	
}

bool ALimitedMovePlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	if(moveLimit > 0 && bIsMoving)
	{
		moveLimit--;
		CheckAndChangeColorCode(startLimit, moveLimit);
		
		if (Super::MoveDirection(Direc))
		{
				return true;
		}
		else
		{
			moveLimit++;
			CheckAndChangeColorCode(startLimit, moveLimit);
			bIsMoving = false;
		}
	}
	
	/*if(moveLimit > 0)
	{
		moveLimit--;
		CheckAndChangeColorCode(startLimit, moveLimit);
		return Super::MoveDirection(Direc);		
	}*/
	return false;
}

void ALimitedMovePlatformPart::DirectionDynamicType()
{
	Super::DirectionDynamicType();
}

void ALimitedMovePlatformPart::MoveToLocationFloor(APSPlatformPart* Box)
{
	Super::MoveToLocationFloor(Box);
	
	EBoxType Type = Box->GetBoxType();
	switch (Type)
	{
		case EBoxType::Wall:
		{
			AWallColorPlatformPart* ColorWall = Cast<AWallColorPlatformPart>(Box);
			if (IsValid(ColorWall))
			{
				switch (ColorWall->GetWallType())
				{
				case EWallType::ColorWall:
					{
						ColorWall->SetMoveMaterials(Flipbook->GetFlipbook(), nullptr);
						break;
					}
				
				}
			
			}
			break;
		}
	}
	
}

void ALimitedMovePlatformPart::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(ColorTable))
	{
		ColorMaterials = *ColorTable->FindRow<FColorsType>(NameColorWall, "", false);
	}
		
	/*if (IsValid(ColorTable))
	{
		ColorsType = *ColorTable->FindRow<FColorsType>(NameColorWall, "", false);
		if (ColorsType.Materials.Num() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("IsValid = true, %.i"), ColorsType.Materials.Num()));

			for (int i = 0; i < ColorsType.Materials.Num(); i++)
			{
				float PF = *ColorsType.Materials.FindKey(FlipbookCaver);

				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT(" %i = %.2f"), i, PF));

			}
			
		}

	}*/

	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &ALimitedMovePlatformPart::UpdateLimits,0.1f,false);	
}

void ALimitedMovePlatformPart::UpdateLimits()
{
	if (bUseRandomLimit)
	{
		GenerateMoveLimit();
	}
	startLimit = moveLimit;

	CheckAndChangeColorCode(startLimit, moveLimit);
}


