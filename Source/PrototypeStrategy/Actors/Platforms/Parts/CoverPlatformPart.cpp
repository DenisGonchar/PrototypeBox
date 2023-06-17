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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, *UEnum::GetValueAsString(CurrentLevel));
	}
	else
	{
		CurrentLevel = ELevelType::Level;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, *UEnum::GetValueAsString(CurrentLevel));
	}

	if (OnActivatorCover.IsBound())
	{
		OnActivatorCover.Broadcast(CurrentLevel);	
	}
}
