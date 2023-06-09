// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/PSPlatformPart.h"
#include <PSTypes.h>
#include "BlockPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API ABlockPlatformPart : public APSPlatformPart
{
	GENERATED_BODY()
	
public:
	ABlockPlatformPart();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	FTeleportInfo TeleportInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	TSubclassOf<class APSPlatformPart> MovePlatform;

	FTeleportInfo GetTeleportInfo() const;
	bool GetbIsActivatorCover() const;
	float GetSpawnDistance() const;

	virtual void NewLevelType() override;

	virtual void ActivatorCover();

protected:
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	float SpawnDistance = 150.0f;

	bool bIsActivatorCover = true;

private:
	
	
};
