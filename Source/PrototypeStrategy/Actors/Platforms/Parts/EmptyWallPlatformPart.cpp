// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/EmptyWallPlatformPart.h"

TArray<AEmptyWallPlatformPart*> AEmptyWallPlatformPart::GetNearEmptyWallParts()
{
    return nearEmptyWalls;
}

void AEmptyWallPlatformPart::SetNearEmptyWallParts(TArray<AEmptyWallPlatformPart*> parts)
{
    nearEmptyWalls = parts;
}
