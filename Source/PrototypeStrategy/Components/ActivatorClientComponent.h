// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Actors/Platforms/Parts/TeleportPlatformPart.h>
#include "ActivatorClientComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPESTRATEGY_API UActivatorClientComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	class ATeleportPlatformPart* BoxActivator;
	
	
	void SetActivator();

	void ActivatorSwitched(bool bIsOn);

private:

	TWeakObjectPtr<class ATeleportPlatformPart> CachedBaseTeleportOwner;
};
