// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSTypes.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "PSPlatformPart.generated.h"

class UPaperSpriteComponent;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		UAudioComponent* Audio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
		UPaperSpriteComponent* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UPaperFlipbook* FlipbookCaver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UPaperSprite* SpriteCaver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
		FConstructedBlockData constructionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* interactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* moveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* activateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* deactivateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* destroySound;

	virtual void BeginPlay() override;

	void PlaySound(USoundBase* soudToPlay);
	
	virtual void Tick(float DeltaTime) override;

	UActivatorCoverComponent* ActivatorCoverComponent; 
	UActivatorCoverComponent* GetActivatorCoverComponent() const;
	
	EBoxType GetBoxType() const;

	void SetCoverPart(TArray<ACoverPlatformPart*> Actors);
	TArray<ACoverPlatformPart*> GetCoverPart() const;

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

	TArray<ACoverPlatformPart*> BaseCover;
	
	UPaperFlipbook* BaseFlipbook;
	UPaperSprite* BaseSprite;

};
