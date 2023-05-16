// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSPlatformPart.h"
#include <PSTypes.h>
#include "Parts/TeleportPlatformPart.h"
#include "PSPlatform.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	TSubclassOf<class APSPlatformPart> FloorPart; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	int32 FullSteps = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	int32 MaxIndex = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	int32 HorizontalIndex = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	float Distance = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	FName NameMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	FName ExitMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	TArray<TSubclassOf<APSPlatformPart>> GridParts;

	void SpawnPlatformPartFloor();
	
private:
		
	
	ATeleportPlatformPart* ActivatorTeleport;
	
	TArray<ATeleportPlatformPart*> ArrayTeleport;

	TArray<TSubclassOf<APSPlatformPart>> Floor;

};
