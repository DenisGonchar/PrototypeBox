// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include "MirroredPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API AMirroredPlatformPart : public AMovePlatformPart
{
	GENERATED_BODY()


public:
	/*AMirroredPlatformPart();

	virtual void BeginPlay() override;*/

	virtual bool MoveDirection(EMoveCharacterDirection Direc) override;


	virtual void NewLevelType() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsClone = false;

	TArray<AMirroredPlatformPart*> clones;

private:

	EMoveCharacterDirection ReversDirection(EMoveCharacterDirection direction);



	
};
