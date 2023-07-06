// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LimitedMovePlatformPart.h"
#include "Actors/Platforms/Parts/WallPlatformPart.h"
#include "PSTypes.h"
#include "WallColorPlatformPart.generated.h"


UCLASS()
class PROTOTYPESTRATEGY_API AWallColorPlatformPart : public AWallPlatformPart
{
	GENERATED_BODY()

public:
	
	virtual void ActivatorCover() override;

	virtual void NewLevelType() override;

	void SetMoveMaterials(UPaperFlipbook* ActorFlipbook = nullptr, UPaperSprite* ActorSprite = nullptr);
protected:
	virtual void BeginPlay() override;

private:
	FVector StartBoxExtent = FVector::ZeroVector;
	FVector StartBoxLocation = FVector::ZeroVector;

	UPaperFlipbook* PawnFlipbook;
	UPaperSprite* PawnSprite;
};
