// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/PathPlatformPart.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"


void APathPlatformPart::BeginPlay()
{
	Super::BeginPlay();
}

void APathPlatformPart::ActivatorCover()
{
	Super::ActivatorCover();

	if (bIsActivatorCover)
	{
		if (IsValid(ActivatiorFlipbook))
		{
			Flipbook->SetFlipbook(ActivatiorFlipbook);
			IsStepedInCover = true;
			bIsActivatorCover = false;
		}
		if (IsValid(ActivatiorSprite))
		{
			Sprite->SetSprite(ActivatiorSprite);
			IsStepedInCover = true;
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
			Flipbook->SetFlipbook(ActivatiorFlipbook);
		}
	}
	else if (LevelType == ELevelType::Level)
	{
			Flipbook->SetFlipbook(BaseFlipbook);		
	}
}
