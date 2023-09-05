// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include "../../../PSTypes.h"
//#include <Components/LedgeDetectorComponent.h>
#include "ConstructPlatformPart.generated.h"
/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API AConstructPlatformPart : public AMovePlatformPart
{
	GENERATED_BODY()	

	TArray<FHitResult> hitResults;

	FVector traceLocation;
	TArray<AActor*> ignoreActors;


	//FLedgeDescription LedgeDescription;

public:
	void SetConstractBlocksArray(TArray<AConstructPlatformPart*>blocks);
	TArray<AConstructPlatformPart*> GetConstractBlocksArray();

	virtual bool MoveDirection(EMoveCharacterDirection Direc) override;
	virtual void MoveToLocationFloor(APSPlatformPart* Box) override;

	bool IsMovedByCharacter = false;

protected:
	TArray<AConstructPlatformPart*> constructBlocks;
	TArray<AConstructPlatformPart*> blocksToMove;

	class ULedgeDetectorComponent* LedgeDetectorComponent;

	void DetectConstructedBlock(TArray<AConstructPlatformPart*> DetectedBlocks);


	FVector LocationByDirection(EMoveCharacterDirection moveDirection);

	bool CheckObstacles(EMoveCharacterDirection directionToCheck);

	
};
