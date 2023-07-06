// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include "LimitedMovePlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API ALimitedMovePlatformPart : public AMovePlatformPart
{
	GENERATED_BODY()

public:
	ALimitedMovePlatformPart();
	void GenerateMoveLimit();
	virtual bool MoveDirection(EMoveCharacterDirection Direc) override;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool bUseRandomLimit = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int moveLimit;

protected:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int startLimit;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void CheckAndChangeColorCode(int limitOnStart ,int currentLimit);
		
};
