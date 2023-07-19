// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/MovePlatformPart.h"
#include "LimitedMovePlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API ALimitedMovePlatformPart : public AMovePlatformPart
{
	GENERATED_BODY()

public:
	ALimitedMovePlatformPart();
	void GenerateMoveLimit();
	virtual bool MoveDirection(EMoveCharacterDirection Direc) override;

	virtual void DirectionDynamicType(APSPlatformPart* Box);
	virtual void MoveToLocationFloor(APSPlatformPart* Box);
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Params | Tables")
	UDataTable* ColorTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Params | Tables")
	FName NameColorWall;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int moveLimit;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool bUseRandomLimit = false;

protected:
	UPROPERTY(BlueprintReadOnly)
		FColorsType ColorMaterials;	

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		int startLimit;


	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void CheckAndChangeColorCode(int limitOnStart ,int currentLimit);

	void UpdateLimits();

	bool bIsMoving = true;
};
