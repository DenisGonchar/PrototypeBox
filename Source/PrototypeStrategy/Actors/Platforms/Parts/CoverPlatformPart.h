// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/BlockPlatformPart.h"
#include "CoverPlatformPart.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnActivatorCover, ELevelType);

UCLASS()
class PROTOTYPESTRATEGY_API ACoverPlatformPart : public ABlockPlatformPart
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	FOnActivatorCover OnActivatorCover;

	void ActivatorCover();

protected:
	
	ELevelType CurrentLevel = ELevelType::Level;

private:


};
