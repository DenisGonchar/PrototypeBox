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
	virtual void ActivatorCover() override;

	virtual void NewLevelType() override;

	EWallType GetWallType() const;

	void DeadBox();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param | Type")
	EWallType WallType = EWallType::DefaultWall;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayDestroySequence();

};
