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
	startLimit = moveLimit;
}

void ALimitedMovePlatformPart::GenerateMoveLimit()
{
	moveLimit = FMath::RandRange(2,6);	
}

bool ALimitedMovePlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Cyan,FString::FromInt(moveLimit));
	if(moveLimit > 0)
	{
		moveLimit--;
		CheckAndChangeColorCode(startLimit, moveLimit);
		return Super::MoveDirection(Direc);		
	}
	return false;
}

void ALimitedMovePlatformPart::BeginPlay()
{
	Super::BeginPlay();	
	CheckAndChangeColorCode(startLimit, moveLimit);
}


