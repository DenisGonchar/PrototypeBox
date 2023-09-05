// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../PSTypes.h"
#include "../SaveGame/PSSaveGame.h"
#include "PSGameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterMove);
/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API UPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FOnCharacterMove OnMove;

	UFUNCTION(BlueprintCallable)
		void SaveLevel(FLevelData levelData);

	UFUNCTION(BlueprintCallable)
		FLevelData LoadLevel(FString levelName);

	UFUNCTION(BlueprintCallable)
		bool LoadLevels(TArray<FLevelData>& levels);

protected:
	UPSSaveGame* gameSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveSlotName")
		FString levelsSaveSlot = "levels";

	UFUNCTION(BlueprintCallable)
		void RemoveSaveInSlot(FString slotName);



};
