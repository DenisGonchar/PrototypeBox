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

	void MagneticFinded(AActor* actor);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params | Cover")
		bool bIsActivCaver = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params | Cover")
		bool bIsActivLevel = false;

	virtual void NewLevelType() override;
		void MoveMagnetic(EMoveCharacterDirection moveDirection);
		FVector GetLocationByDirection(EMoveCharacterDirection Direction);

	void SwitchSprite(EPolarizationType newPolarization);


	UFUNCTION()
		void UpdateStatus(bool newStatus);

	UFUNCTION()
		void ChangeMagneticsStatus(TArray<AMagneticPlatformPart*> parts, bool newStatus);

	UFUNCTION()
		void SwitchActivator();
		void MagneticPlayer(APSBaseCharacter* player);
		void Magnetic(AMagneticPlatformPart* magneticPart);
		void CheckPlayer();

		EMoveCharacterDirection FindMagneticDirection(FVector location);
		EMoveCharacterDirection ReversDirection(EMoveCharacterDirection direction);

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeMaterial(bool value);

protected:

	
	

};

