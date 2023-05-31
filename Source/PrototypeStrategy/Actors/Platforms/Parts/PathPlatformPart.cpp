// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/PathPlatformPart.h"


void APathPlatformPart::ActivatorCover()
{
	Super::ActivatorCover();

	if (bIsActivatorCover)
	{
		if (IsValid(ActivatiorMaterial))
		{
			BoxMesh->SetMaterial(0, ActivatiorMaterial);

			bIsActivatorCover = false;
		}
	}
}

void APathPlatformPart::NewLevelType()
{
	Super::NewLevelType();
	
	if (LevelType == ELevelType::UnderCover)
	{
		if (!bIsActivatorCover)
		{
			if (IsValid(ActivatiorMaterial))
			{
				BoxMesh->SetMaterial(0, ActivatiorMaterial);

			}
		}
	}
}
