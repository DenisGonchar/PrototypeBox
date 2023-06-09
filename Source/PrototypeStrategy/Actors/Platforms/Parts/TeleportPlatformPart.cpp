// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/TeleportPlatformPart.h"
#include <Components/ActivatorClientComponent.h>

ATeleportPlatformPart::ATeleportPlatformPart()
{
	ActivatorComponent = CreateDefaultSubobject<UActivatorClientComponent>(TEXT("ActivatorComponent"));
}

void ATeleportPlatformPart::BeginPlay()
{
	Super::BeginPlay();

}

UActivatorClientComponent* ATeleportPlatformPart::GetActivatorComponent() const
{
	return ActivatorComponent;
}

ATeleportPlatformPart* ATeleportPlatformPart::GetTeleport() const
{
	return Teleport;
}

ATeleportPlatformPart* ATeleportPlatformPart::GetActivator() const
{
	return BaseActivator;
}

void ATeleportPlatformPart::SetTeleport(ATeleportPlatformPart* Actor)
{
	Teleport = Actor;
}

void ATeleportPlatformPart::SetActivator(ATeleportPlatformPart* Actor)
{
	BaseActivator = Actor;

	if (!IsValid(BaseActivator))
	{
		TeleportInfo.bIsTeleportType = true;
	}

	GetActivatorComponent()->SetActivator();
}

void ATeleportPlatformPart::SwitchActivator()
{
	if (TeleportInfo.TeleportType == ETeleport::Activator)
	{
		TeleportInfo.bIsTeleportType = !TeleportInfo.bIsTeleportType;

		if (OnActivatorSwitched.IsBound())
		{
			OnActivatorSwitched.Broadcast(TeleportInfo.bIsTeleportType);
		}

		if (OnActivator.IsBound())
		{
			OnActivator.Broadcast(TeleportInfo.bIsTeleportType);
		}
	}
}

