// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PSGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API APSGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	int GetSteps() const;

	void IncrementSteps(int Index);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	int Steps = -1;

};
