// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include <Components/LedgeDetectorComponent.h>
#include "ConstructPlatformPart.generated.h"
/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API AConstructPlatformPart : public AMovePlatformPart
{
	GENERATED_BODY()

	virtual bool MoveDirection(EMoveCharacterDirection Direc) override;
	virtual void MoveToLocationFloor(APSPlatformPart* Box) override;

	TArray<FHitResult> hitResults;

	FVector traceLocation;
	TArray<AActor*> ignoreActors;


	FLedgeDescription LedgeDescription;

protected:
	TArray<AConstructPlatformPart*> constructBlocks;

	class ULedgeDetectorComponent* LedgeDetectorComponent;
	
};
