// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/PathPlatformPart.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"


void APathPlatformPart::ActivatorCover()
{
	Super::ActivatorCover();

	if (bIsActivatorCover)
	{
		if (IsValid(ActivatiorFlipbook))
		{
			Flipbook->SetFlipbook(ActivatiorFlipbook);

			bIsActivatorCover = false;
		}
		if (IsValid(ActivatiorSprite))
		{
			Sprite->SetSprite(ActivatiorSprite);

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
			if (IsValid(ActivatiorFlipbook))
			{
				Flipbook->SetFlipbook(ActivatiorFlipbook);

			}
			if (IsValid(ActivatiorSprite))
			{
				Sprite->SetSprite(ActivatiorSprite);
			}
		}
	}
}
