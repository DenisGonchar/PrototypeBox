// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PSTypes.h"
#include "ActivatorCoverComponent.generated.h"

class ACoverPlatformPart;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPESTRATEGY_API UActivatorCoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	virtual void BeginPlay() override;

	ACoverPlatformPart* BoxCover; 

	void SetCover();

	void ActivatorCover(ELevelType World);

protected:

	TWeakObjectPtr<class APSPlatformPart> CachedBasePlatformPartOwner;
};
