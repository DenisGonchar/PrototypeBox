// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/BlockPlatformPart.h"
#include "MovePlatformPart.h"
#include "../PSPlatformPart.h"

ABlockPlatformPart::ABlockPlatformPart()
{

}

void ABlockPlatformPart::BeginPlay()
{
	Super::BeginPlay();

}

FName ABlockPlatformPart::GetOpenLevel() const
{
	return OpenLevel;
}

void ABlockPlatformPart::SetOpenLevel(FName Map)
{
	OpenLevel = Map;
}

FTeleportInfo ABlockPlatformPart::GetTeleportInfo() const
{
	return TeleportInfo;
}

bool ABlockPlatformPart::GetbIsActivatorCover() const
{
	return bIsActivatorCover;
}

float ABlockPlatformPart::GetSpawnDistance() const
{
	return SpawnDistance;
}

void ABlockPlatformPart::NewLevelType()
{
	Super::NewLevelType();

	
	
}

void ABlockPlatformPart::ActivatorCover()
{

}
