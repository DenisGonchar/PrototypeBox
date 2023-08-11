// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/ConstructPlatformPart.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WallPlatformPart.h"

bool AConstructPlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	if (IsMovedByCharacter)
	{
		IsMovedByCharacter = false;
		DetectConbstructedBlock(constructBlocks);
		if (blocksToMove.Num() > 0)
		{
			for (auto block : blocksToMove)
			{
				if (IsValid(block))
				{
					if (!block->CheckObstacles(Direc))
					{
						return false;
					}
				}
			}

			for (auto block : blocksToMove)
			{
				if (IsValid(block))
				{
					block->MoveDirection(Direc);
				}
			}
			return Super::MoveDirection(Direc);
		}
		else
		{
			return Super::MoveDirection(Direc);
		}
	}
	else
	{
		SetActorLocation(LocationByDirection(Direc));
		return true;
		//Super::MoveDirection(Direc);
	}
}

void AConstructPlatformPart::MoveToLocationFloor(APSPlatformPart* Box)
{
	Super::MoveToLocationFloor(Box);	
}

void AConstructPlatformPart::SetConstractBlocksArray(TArray<AConstructPlatformPart*> blocks)
{
	constructBlocks = blocks;
	constructBlocks.Remove(this);
}

TArray<AConstructPlatformPart*> AConstructPlatformPart::GetConstractBlocksArray()
{
	return constructBlocks;
}

void AConstructPlatformPart::DetectConbstructedBlock(TArray<AConstructPlatformPart*> DetectedBlocks)
{
	blocksToMove.Empty();
	FVector location = GetActorLocation();
	for (auto block : constructBlocks)
	{
		if (location.Equals(block->GetActorLocation(),151))
		{
			blocksToMove.Add(block);
		}
	}

	for (auto block : blocksToMove)
	{
		if (location.Equals(block->GetActorLocation(), 151))
		{
			blocksToMove.AddUnique(block);
		}
	}
}

FVector AConstructPlatformPart::LocationByDirection(EMoveCharacterDirection moveDirection)
{
	FVector location = GetActorLocation();
	switch (moveDirection)
	{
	case EMoveCharacterDirection::Top:
		return location + FVector(150.f, 0.f, 0.f);
		break;
	case EMoveCharacterDirection::Down:
		return location - FVector(150.f, 0.f, 0.f);
		break;
	case EMoveCharacterDirection::Left:
		return location - FVector(0.f, 150.f, 0.f);
		break;
	case EMoveCharacterDirection::Right:
		return location + FVector(0.f, 150.f, 0.f);
		break;
	case EMoveCharacterDirection::None:
		return location;
		break;
	default:
		break;
	}
	return location;
}

bool AConstructPlatformPart::CheckObstacles(EMoveCharacterDirection directionToCheck)
{
	FVector location = LocationByDirection(directionToCheck);
	FVector buttomLocation = location;
	buttomLocation.Z = 0;
	TArray<AActor*> actToIngnore;
	actToIngnore.Add(this);
	UObject* world = GetWorld();

	FHitResult frontHit;
	FHitResult buttomHit;

	UKismetSystemLibrary::BoxTraceSingle(world, location, location, FVector(5.f, 5.f, 5.f),FRotator::ZeroRotator, ETraceTypeQuery::TraceTypeQuery1, false, 
		actToIngnore, EDrawDebugTrace::ForDuration, frontHit, true);
	UKismetSystemLibrary::BoxTraceSingle(world, buttomLocation, buttomLocation, FVector(5.f, 5.f, 5.f), FRotator::ZeroRotator, ETraceTypeQuery::TraceTypeQuery1, false, 
		actToIngnore, EDrawDebugTrace::ForDuration, buttomHit, true);

	if(buttomHit.bBlockingHit)
	{
		if (frontHit.bBlockingHit)
		{
			if (LevelType == ELevelType::UnderCover)
			{
				AWallPlatformPart* wall = Cast<AWallPlatformPart>(frontHit.Actor);
				if (!IsValid(wall))
				{
					wall->Destroy();
					return true;
				}
			}
			AConstructPlatformPart* constract = Cast<AConstructPlatformPart>(frontHit.Actor);
			if (!IsValid(constract))
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}
