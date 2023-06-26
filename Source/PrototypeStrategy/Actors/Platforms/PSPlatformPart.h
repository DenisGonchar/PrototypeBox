// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTypes.h"
#include "PSPlatformPart.generated.h"

class UActivatorCoverComponent;
class ACoverPlatformPart;
class UMaterialInterface;
class USceneComponent;
class UBoxComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;

UCLASS()
class PROTOTYPESTRATEGY_API APSPlatformPart : public AActor
{
	GENERATED_BODY()
	
public:	
	APSPlatformPart();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scene")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoxComponent")
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
		UPaperFlipbookComponent* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
		UPaperFlipbook* FlipbookCaver;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
		FConstructedBlockData constructionData;

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UActivatorCoverComponent* ActivatorCoverComponent; 
	UActivatorCoverComponent* GetActivatorCoverComponent() const;
	
	EBoxType GetBoxType() const;

	void SetCoverPart(ACoverPlatformPart* Actor);
	ACoverPlatformPart* GetCoverPart() const;

	void SetLevelType(ELevelType World);
	ELevelType GetLevelType() const;

	UPaperFlipbook* GetBaseFlibook();

	virtual void NewLevelType();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param | Type")
		EBoxType BoxType = EBoxType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Param | Type")
		ELevelType LevelType = ELevelType::Level;

	UFUNCTION(BlueprintCallable)
		void AcceptConstructionData();

	ACoverPlatformPart* BaseCover; 
	
	UPaperFlipbook* BaseFlipbook;

};
