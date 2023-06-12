// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/ExitPlatformPart.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "PaperFlipbook.h"


void AExitPlatformPart::BeginPlay()
{
	Super::BeginPlay();

}

void AExitPlatformPart::NewLevelType()
{
	Super::NewLevelType();

	if (GetLevelType() == ELevelType::UnderCover)
	{
		if (bIsActivCaver && bIsActivLevel)
		{
			if (IsValid(BaseFlipbook))
			{
				Flipbook->SetFlipbook(BaseFlipbook);
			}
		}
		
	}
	

}

void AExitPlatformPart::ActivatorCover()
{
	Super::ActivatorCover();
	
}

FName AExitPlatformPart::GetOpenLevel() const
{
	return OpenLevel;
}

void AExitPlatformPart::SetOpenLevel(FName Map)
{
	OpenLevel = Map;
}

bool AExitPlatformPart::GetIsActivCaver() const
{
	return bIsActivCaver;
}

bool AExitPlatformPart::GetIsActivLevel() const
{
	return bIsActivLevel;
}
