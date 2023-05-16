// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PSTypes.h"
#include <Actors/Platforms/PSPlatformPart.h>
#include <Actors/Platforms/PSPlatform.h>
#include "LedgeDetectorComponent.generated.h"

USTRUCT(BlueprintType)
struct FLedgeDescription
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	APSPlatform* Platform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	APSPlatformPart* BoxMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	EBoxType Type = EBoxType::None;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPESTRATEGY_API ULedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	bool StartDetectDown(OUT FLedgeDescription& LedgeDescription);
	bool StartBoxDetectDown(OUT FLedgeDescription& LedgeDescription);

	bool DetectLedge(OUT FLedgeDescription& LedgeDescription, OUT EMoveCharacterDirection Direction);
	bool BoxDetectLedge(OUT FLedgeDescription& LedgeDescription, OUT EMoveCharacterDirection Direction);

	bool DetectHitBlock(FHitResult Hit);

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	float DrawTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	FColor DrawColor = FColor::Red;

protected:
	
	virtual void BeginPlay() override;

	EMoveCharacterDirection CharacterDirection = EMoveCharacterDirection::None;
private:
	
	TWeakObjectPtr<class APSBaseCharacter> CachedPawnOwner;
	TWeakObjectPtr<class AMovePlatformPart> CachedActorOwner;

};
