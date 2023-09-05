// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include "Actors/Platforms/Parts/MirroredPlatformPart.h"
#include "Actors/Platforms/Parts/ConstructPlatformPart.h"
#include "../../../PSTypes.h"
#include <Kismet/GameplayStatics.h>
#include "../../../GameInstance/PSGameInstance.h"
#include "PacmanPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API APacmanPlatformPart : public AMovePlatformPart
{
	GENERATED_BODY()

public:
	void SetSteps(FPacmanSteps newSteps);

	virtual void BeginPlay() override;
	virtual bool MoveDirection(EMoveCharacterDirection Direc) override;

private:

	FPacmanSteps stepsStruct;

	TArray<EMoveCharacterDirection> directions;
	TArray<int> directionCount;

	TArray<AActor*> actToIgnore;
	FHitResult traceResult;
	UWorld* worldRef;

	void UpdateMoveData();

protected:
	UFUNCTION(BlueprintCallable)
		void NextStep();

	int currentStep = 0;

	FVector FindTraceLocationByDirection(EMoveCharacterDirection direction, float traceDistance);

	bool CheckObstacles(EMoveCharacterDirection directionToCheck);

};
