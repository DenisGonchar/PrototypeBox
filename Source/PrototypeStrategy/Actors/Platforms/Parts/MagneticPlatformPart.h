// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/BlockPlatformPart.h"
#include "PSTypes.h"
#include "MagneticPlatformPart.generated.h"

/**
 * 
 */
// Fill out your copyright notice in the Description page of Project Settings.

UCLASS()
class PROTOTYPESTRATEGY_API AMagneticPlatformPart : public ABlockPlatformPart
{
	GENERATED_BODY()
	
	AMagneticPlatformPart();

private:
	FTimerHandle magneticTimer;
	FHitResult traceResult;
	
public:
	void BeginPlay() override;


	bool bIsActive = false;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		EMagneticType MagneticType;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		EPolarizationType polarization = EPolarizationType::None;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		TArray<AMagneticPlatformPart*> MagneticParts;

	UFUNCTION()
		void UpdateStatus(bool newStatus);

	UFUNCTION()
		void ChangeMagneticsStatus(TArray<AMagneticPlatformPart*> parts, bool newStatus);

	UFUNCTION()
		void SwitchActivator();
		void Magnetic(class APSBaseCharacter* player);
		void CheckPlayer();

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeMaterial(bool value);

	
	

};

