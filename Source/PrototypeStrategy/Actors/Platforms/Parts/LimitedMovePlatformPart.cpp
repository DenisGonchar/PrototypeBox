// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/LimitedMovePlatformPart.h"

#include "Components/LedgeDetectorComponent.h"


ALimitedMovePlatformPart::ALimitedMovePlatformPart()
{
	PrimaryActorTick.bCanEverTick = true;
	
	if(bUseRandomLimit)
	{
		GenerateMoveLimit();
	}
}

void ALimitedMovePlatformPart::GenerateMoveLimit()
{
	moveLimit = FMath::RandRange(2,6);
}


