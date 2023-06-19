// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/BlockPlatformPart.h"
#include "PSTypes.h"
#include "WallPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API AWallPlatformPart : public ABlockPlatformPart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	UPaperFlipbook* FlipbookDead;

	virtual void ActivatorCover() override;

	virtual void NewLevelType() override;

	EWallType GetWallType() const;

	void StartDeadBox();
	void EndDeadBox();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param | Type")
	EWallType WallType = EWallType::DefaultWall;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayDestroySequence();

	FTimerHandle WallTimer;
	//FHitResult HitResult;

};
