// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/WallPlatformPart.h"
#include "EmptyWallPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API AEmptyWallPlatformPart : public AWallPlatformPart
{
	GENERATED_BODY()

private:
	TArray<AEmptyWallPlatformPart*> nearEmptyWalls;

public:
	TArray<AEmptyWallPlatformPart*> GetNearEmptyWallParts();
	
	void SetNearEmptyWallParts(TArray<AEmptyWallPlatformPart*> parts);
};
