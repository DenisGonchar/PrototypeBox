// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace PSTraceUtils
{
	bool LineTraceSingleByChannel(const UWorld* World, struct FHitResult& OutHit, const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam, bool bDrawDebug = false, float DrawTime = -1.0f, FColor TraceColor = FColor::Black);


}