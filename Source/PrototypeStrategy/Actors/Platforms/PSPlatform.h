// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <PSTypes.h>
#include "Parts/MirroredPlatformPart.h"
#include "PSPlatform.generated.h"

class APSPlatformPart;
class ATeleportPlatformPart;
class ACoverPlatformPart;
class AMagneticPlatformPart;
//class AMirroredPlatformPart;

UCLASS()
class PROTOTYPESTRATEGY_API APSPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	APSPlatform();

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void Tick(float DeltaTime) override;

	TArray<TSubclassOf<APSPlatformPart>> GetGridParts() const;

protected:
	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	TSubclassOf<class APSPlatformPart> FloorPart; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	int32 HorizontalIndex = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	float Distance = 5.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	TArray<TSubclassOf<APSPlatformPart>> GridParts;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
		TArray<int> moveLimits;

	UFUNCTION(BlueprintCallable)
		void SpawnPlatformPartFloor(TArray<AActor*> parts);

	UFUNCTION(BlueprintCallable)
		TArray<AActor*> SpawnBlocks(TArray<FConstructedBlockData> blocks);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
		int32 FullSteps = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
		int32 MaxIndex = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
		FName NameMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
		FName ExitMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
		FString levelName;

	TArray<AActor*> spawnedBlocks;
	
private:

	AMagneticPlatformPart* MagneticActivator;
	TArray<AMagneticPlatformPart*> MagneticArray;
	
	ACoverPlatformPart* CoverPart;	

	ATeleportPlatformPart* ActivatorTeleport;
	TArray<ATeleportPlatformPart*> ArrayTeleport;

	TArray <APSPlatformPart* > ArrayPlatformPart;

	TArray<TSubclassOf<APSPlatformPart>> Floor;

	TArray<AMirroredPlatformPart*> mirroredClones;
	AMirroredPlatformPart* mirroredBlock;

};
