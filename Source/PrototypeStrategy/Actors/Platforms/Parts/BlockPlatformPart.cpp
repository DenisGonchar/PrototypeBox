// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/BlockPlatformPart.h"
#include "MovePlatformPart.h"

ABlockPlatformPart::ABlockPlatformPart()
{

}

void ABlockPlatformPart::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MovePlatform))
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += SpawnDistance;

		APSPlatformPart* MoveBox = GetWorld()->SpawnActor<APSPlatformPart>(MovePlatform, SpawnLocation, FRotator::ZeroRotator);


	}
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
