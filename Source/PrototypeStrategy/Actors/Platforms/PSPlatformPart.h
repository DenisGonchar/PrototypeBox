// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTypes.h"
#include "PSPlatformPart.generated.h"

class UActivatorCoverComponent;
class ACoverPlatformPart;

UCLASS()
class PROTOTYPESTRATEGY_API APSPlatformPart : public AActor
{
	GENERATED_BODY()
	
public:	
	APSPlatformPart();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* BoxMesh;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UActivatorCoverComponent* ActivatorCoverComponent; 
	UActivatorCoverComponent* GetActivatorCoverComponent() const;
	
	EBoxType GetBoxType() const;

	void SetCoverPart(ACoverPlatformPart* Actor);
	ACoverPlatformPart* GetCoverPart() const;

	void SetLevelType(ELevelType World);
	ELevelType GetLevelType() const;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param | Type")
	EBoxType BoxType = EBoxType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param | Type")
	ELevelType LevelType = ELevelType::Level;

	ACoverPlatformPart* BaseCover; 
};
