// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/MirroredPlatformPart.h"


bool AMirroredPlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	if (!IsClone)
	{
		for (auto clone : clones)
		{
			clone->MoveDirection(Direc);
		}
		return Super::MoveDirection(Direc);
	}
	else
	{
		return Super::MoveDirection(ReversDirection(Direc));
	}
	return false;
}

void AMirroredPlatformPart::NewLevelType()
{
	Super::NewLevelType();
	
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
