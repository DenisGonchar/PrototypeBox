// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/CoverPlatformPart.h"

void ACoverPlatformPart::BeginPlay()
{
	Super::BeginPlay();


}

void ACoverPlatformPart::ActivatorCover()
{
	if (ELevelType::Level == CurrentLevel)
	{
		CurrentLevel = ELevelType::UnderCover;
	}
	else
	{
		CurrentLevel = ELevelType::Level;
	}

	if (OnActivatorCover.IsBound())
	{
		OnActivatorCover.Broadcast(CurrentLevel);
	
	}
}
