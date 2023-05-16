// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTypes.h"
#include "PSPlatformPart.generated.h"

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

	EBoxType GetBoxType() const;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param")
	EBoxType BoxType = EBoxType::None;

};
