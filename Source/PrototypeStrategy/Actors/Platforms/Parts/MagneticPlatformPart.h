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

class APSBaseCharacter;

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsHaveBadplate = false;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		EMagneticType MagneticType;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		EPolarizationType polarization = EPolarizationType::Positive;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		TArray<AMagneticPlatformPart*> MagneticParts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Polarizer")
		UPaperSprite* NonPolarizationSprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Polarizer")
		UPaperSprite* PositivePolarizationSprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Polarizer")
		UPaperSprite* NegativePolarizationSprite;
	
	UFUNCTION(BlueprintImplementableEvent)
		void ChangeBlockSprite(UPaperSprite* newSprite);

	void SwitchSprite(EPolarizationType newPolarization);


	UFUNCTION()
		void UpdateStatus(bool newStatus);

	UFUNCTION()
		void ChangeMagneticsStatus(TArray<AMagneticPlatformPart*> parts, bool newStatus);

	UFUNCTION()
		void SwitchActivator();
		void Magnetic(APSBaseCharacter* player);
		void CheckPlayer();

		EMoveCharacterDirection FindMagneticDirection(APSBaseCharacter* player);
		EMoveCharacterDirection ReversDirection(EMoveCharacterDirection direction);

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeMaterial(bool value);

	
	

};

