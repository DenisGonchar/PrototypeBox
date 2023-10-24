// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Actors/Platforms/PSPlatform.h"
#include "PSGameMode.generated.h"



UCLASS(minimalapi)
class APSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APSGameMode();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int GetFullSteps() const;

	bool GetWin() const;

	void SetFullSteps(int Full);

	void MadeStep(int Index);
	void CheckWin();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	bool bIsWin = true;

	int FullSteps = 1;

	void SetLevelPlatform(APSPlatform* platf);
	APSPlatform* GetLevelPlatform();
protected:

	class APSGameState* WState;
	class APSBaseCharacter* BaseCharacter;

private:
	APSPlatform* levelPlatform;
};



