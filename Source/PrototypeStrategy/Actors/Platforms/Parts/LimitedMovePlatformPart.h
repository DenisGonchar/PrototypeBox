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

protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int moveLimit;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int startLimit;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool bUseRandomLimit = true;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void CheckAndChangeColorCode(int limitOnStart ,int currentLimit);
		
};
