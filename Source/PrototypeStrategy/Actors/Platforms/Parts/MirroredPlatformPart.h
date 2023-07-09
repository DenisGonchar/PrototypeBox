// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include "Actors/Platforms/Parts/WallPlatformPart.h"
#include "Actors/Platforms/PSPlatformPart.h"
#include "MirroredPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API AMirroredPlatformPart : public AMovePlatformPart
{
	GENERATED_BODY()


public:
	/*AMirroredPlatformPart();

	virtual void BeginPlay() override;*/

	virtual bool MoveDirection(EMoveCharacterDirection Direc) override;
	virtual void MoveToLocationFloor(APSPlatformPart* Box) override;


	virtual void NewLevelType() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsClone = false;

	AMirroredPlatformPart* generalMirrored;

	TArray<AMirroredPlatformPart*> clones;

	FVector FindTraceLocationByDirection(EMoveCharacterDirection direction, float traceDistance = 800.f);

private:

	EMoveCharacterDirection lastDirection;

	EMoveCharacterDirection ReversDirection(EMoveCharacterDirection direction);

	void CrackedWallFindedByClone(AMirroredPlatformPart* clone, AWallPlatformPart* wall);




	
};
