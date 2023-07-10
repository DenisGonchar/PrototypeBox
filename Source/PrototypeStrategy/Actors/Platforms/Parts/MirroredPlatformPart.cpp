// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/MirroredPlatformPart.h"
#include "Kismet/KismetSystemLibrary.h"


bool AMirroredPlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{

	if (!IsClone)
	{
		lastDirection = Direc;
		for (auto clone : clones)
		{
			clone->MoveDirection(Direc);
		}
		return Super::MoveDirection(Direc);
	}
	else
	{
		EMoveCharacterDirection ReversedDirection = ReversDirection(Direc);
		lastDirection = ReversedDirection;
		return Super::MoveDirection(ReversedDirection);
	}
	return false;
}

void AMirroredPlatformPart::MoveToLocationFloor(APSPlatformPart* Box)
{
	Super::MoveToLocationFloor(Box);

	FHitResult hitResult;
	TArray<AActor*> actToIgnore;
	actToIgnore.Add(this);

	if (!IsClone || LevelType != ELevelType::UnderCover)
	{
		return;
	}

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), FindTraceLocationByDirection(lastDirection), ETraceTypeQuery::TraceTypeQuery1, true, actToIgnore, EDrawDebugTrace::ForDuration, hitResult, false);
	
	AWallPlatformPart* wall = Cast<AWallPlatformPart>(hitResult.Actor);
	if (IsValid(wall))
	{
		if (wall->GetWallType() == EWallType::CrackedWall)
		{
			if (IsValid(generalMirrored))
			{
				generalMirrored->CrackedWallFindedByClone(this, wall);
			}
		}
	}
	
}

void AMirroredPlatformPart::NewLevelType()
{
	Super::NewLevelType();	
}

FVector AMirroredPlatformPart::FindTraceLocationByDirection(EMoveCharacterDirection direction, float traceDistance)
{
	FVector currentLocation = this->GetActorLocation();
	switch (direction)
	{
		case EMoveCharacterDirection::Top:
			return FVector(currentLocation.X + traceDistance, currentLocation.Y, currentLocation.Z);
			break;
		case EMoveCharacterDirection::Down:
			return FVector(currentLocation.X - traceDistance, currentLocation.Y, currentLocation.Z);
			break;
		case EMoveCharacterDirection::Right:
			return FVector(currentLocation.X , currentLocation.Y + traceDistance, currentLocation.Z);
			break;
		case EMoveCharacterDirection::Left:
			return FVector(currentLocation.X, currentLocation.Y - traceDistance, currentLocation.Z);
			break;
		default:
			break;
	}
	return FVector();
}

EMoveCharacterDirection AMirroredPlatformPart::ReversDirection(EMoveCharacterDirection direction)
{
	switch (direction)
	{
		case EMoveCharacterDirection::Top:
			return EMoveCharacterDirection::Down;
			break;
		case EMoveCharacterDirection::Down:
			return EMoveCharacterDirection::Top;
			break;
		case EMoveCharacterDirection::Right:
			return EMoveCharacterDirection::Left;
			break;
		case EMoveCharacterDirection::Left:
			return EMoveCharacterDirection::Right;
			break;
		default:
			break;
	}
	return EMoveCharacterDirection::None;
}

void AMirroredPlatformPart::CrackedWallFindedByClone(AMirroredPlatformPart* clone, AWallPlatformPart* wall)
{
	FHitResult hitResult;
	TArray<AActor*> actToIgnore;
	actToIgnore.Add(this);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), FindTraceLocationByDirection(lastDirection), ETraceTypeQuery::TraceTypeQuery1, true, actToIgnore, EDrawDebugTrace::Persistent, hitResult, true);

	AWallPlatformPart* wallPart = Cast<AWallPlatformPart>(hitResult.Actor);
	if (IsValid(wall))
	{
		if (wall == wallPart)
		{
			wallPart->StartDeadBox();
		}
	}
}
