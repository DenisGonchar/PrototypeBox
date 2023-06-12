// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/BlockPlatformPart.h"
#include "PathPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API APathPlatformPart : public ABlockPlatformPart
{
	GENERATED_BODY()
	
public:
	
	virtual void ActivatorCover() override;

	virtual void NewLevelType() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	UPaperFlipbook* ActivatiorFlipbook;

};
