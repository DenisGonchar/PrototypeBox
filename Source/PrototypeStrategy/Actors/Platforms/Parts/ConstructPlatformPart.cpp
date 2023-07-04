// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/ConstructPlatformPart.h"
#include "Kismet/KismetSystemLibrary.h"

bool AConstructPlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	for (auto block : constructBlocks)
	{
		if (IsValid(block))
		{
			if (!block->LedgeDetectorComponent->BoxDetectLedge(block->LedgeDescription, Direc))
			{
				return false;
			}
		}
	}
	return Super::MoveDirection(Direc);
}

void AConstructPlatformPart::MoveToLocationFloor(APSPlatformPart* Box)
{
	Super::MoveToLocationFloor(Box);
	traceLocation = GetActorLocation();
	ignoreActors.Add(this);
	hitResults.Empty();

	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), traceLocation, traceLocation,FVector(100.f, 100.f, 100.f), FRotator::ZeroRotator, TraceTypeQuery1, 
		false, ignoreActors, EDrawDebugTrace::ForDuration, hitResults, true);	
}
