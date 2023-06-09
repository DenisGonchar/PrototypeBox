// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../PSTypes.h"
#include "PSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API UPSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void SaveLevel(FLevelData levelData);

	UFUNCTION(BlueprintCallable)
		FLevelData LoadLevel(FString levelName);

	UFUNCTION(BlueprintCallable)
		TArray<FLevelData> LoadLevels();

	UPROPERTY(BlueprintReadWrite)
		TArray<FLevelData> levels;




	
};
