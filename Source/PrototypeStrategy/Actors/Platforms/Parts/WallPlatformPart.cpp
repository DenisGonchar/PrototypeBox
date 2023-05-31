// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/WallPlatformPart.h"

void AWallPlatformPart::ActivatorCover()
{
	Super::ActivatorCover();

	switch (WallType)
	{
		case EWallType::CrackedWall :
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Cracked Wall")));

			break;
		}
	}
	
}

void AWallPlatformPart::NewLevelType()
{
	Super::NewLevelType();

	if (IsValid(MaterialCaver))
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Cover")));

	}
}

EWallType AWallPlatformPart::GetWallType() const
{
	return WallType;
}

void AWallPlatformPart::DeadBox()
{
	this->Destroy();
}
