// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActivatorCoverComponent.h"
#include "Actors/Platforms/PSPlatformPart.h"
#include "Actors/Platforms/Parts/CoverPlatformPart.h"

void UActivatorCoverComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedBasePlatformPartOwner = StaticCast<APSPlatformPart*>(GetOwner());
	
}

void UActivatorCoverComponent::SetCover()
{
	
	if (CachedBasePlatformPartOwner.IsValid())
	{
		BoxCover = CachedBasePlatformPartOwner->GetCoverPart();
	}

	if (IsValid(BoxCover))
	{
		BoxCover->OnActivatorCover.AddUObject(this, &UActivatorCoverComponent::ActivatorCover);
	}


}

void UActivatorCoverComponent::ActivatorCover(ELevelType World)
{	
	CachedBasePlatformPartOwner->SetLevelType(World);


}

