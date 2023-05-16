// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/BlockPlatformPart.h"
//#include <Components/ActivatorClientComponent.h>
#include "TeleportPlatformPart.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActivatorSwitched, bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivator, bool, bIsOn);

class UActivatorClientComponent;

UCLASS()
class PROTOTYPESTRATEGY_API ATeleportPlatformPart : public ABlockPlatformPart
{
	GENERATED_BODY()
	
	ATeleportPlatformPart();

public:
	void BeginPlay() override;

	UActivatorClientComponent* GetActivatorComponent() const;
	UActivatorClientComponent* ActivatorComponent;

	UPROPERTY(BlueprintAssignable)
	FOnActivator OnActivator;

	FOnActivatorSwitched OnActivatorSwitched;

	ATeleportPlatformPart* GetTeleport() const;
	ATeleportPlatformPart* GetActivator() const;

	void SetTeleport(ATeleportPlatformPart* Actor);
	void SetActivator(ATeleportPlatformPart* Actor);

	void SwitchActivator();

	ATeleportPlatformPart* Teleport;

	ATeleportPlatformPart* BaseActivator;

};
