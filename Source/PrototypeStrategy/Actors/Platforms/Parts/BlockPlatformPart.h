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

	FName GetOpenLevel() const;
	void SetOpenLevel(FName Map);

	FTeleportInfo GetTeleportInfo() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	FTeleportInfo TeleportInfo;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	TSubclassOf<class APSPlatformPart> MovePlatform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	float SpawnDistance = 150.0f;

	FName OpenLevel;

	
private:
	

	
};
