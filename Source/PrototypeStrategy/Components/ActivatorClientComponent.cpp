// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActivatorClientComponent.h"
#include <Actors/Platforms/Parts/TeleportPlatformPart.h>
#include <Actors/Platforms/Parts/BlockPlatformPart.h>

void UActivatorClientComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedBaseTeleportOwner = StaticCast<ATeleportPlatformPart*>(GetOwner());
	
	SetActivator();
}

void UActivatorClientComponent::SetActivator()
{
	
		if (CachedBaseTeleportOwner.IsValid())
		{
			BoxActivator = CachedBaseTeleportOwner->GetActivator();
		}

		if (IsValid(BoxActivator))
		{

			BoxActivator->OnActivatorSwitched.AddUObject(this, &UActivatorClientComponent::ActivatorSwitched);
		}

	
}

void UActivatorClientComponent::ActivatorSwitched(bool bIsOn)
{
	SetActivator();

	if (CachedBaseTeleportOwner.IsValid())
	{
		CachedBaseTeleportOwner->TeleportInfo.bIsTeleportType = bIsOn;

	}

	if (OnActivatorClientOnSwitch.IsBound())
	{
		OnActivatorClientOnSwitch.Broadcast(bIsOn);
	}
}

