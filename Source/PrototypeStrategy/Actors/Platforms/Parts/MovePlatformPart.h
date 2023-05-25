// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/PSPlatformPart.h"
#include "PSTypes.h"
#include "MovePlatformPart.generated.h"


UCLASS()
class PROTOTYPESTRATEGY_API AMovePlatformPart : public APSPlatformPart
{
	GENERATED_BODY()
	
public:
	AMovePlatformPart();

	virtual void BeginPlay() override;

	float GetMoveDistance() const;

	bool MoveDirection(EMoveCharacterDirection Direc);
	void DirectionDynamicType(APSPlatformPart* Box);
	void MoveToLocationFloor(APSPlatformPart* Box);
	
	EDynamic GetDynamicType() const;

protected:
	class ULedgeDetectorComponent* LedgeDetertorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param")
	float MoveDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param")
	EDynamic DynamicType = EDynamic::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Active")
	int ActiveMaxStepIndex = 10.0f;

	EMoveCharacterDirection Direction;
};
