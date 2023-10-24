// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/ConstructPlatformPart.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "GameMode/PSGameMode.h"
#include "WallPlatformPart.h"
#include "EmptyWallPlatformPart.h"

bool AConstructPlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	if (IsMovedByCharacter)
	{
		if (CheckObstacles(Direc))
		{
			IsMovedByCharacter = false;
			DetectConstructedBlock(constructBlocks);
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
				blocksToMove.Add(this);
				for (auto block : blocksToMove)
				{
					if (IsValid(block))
					{
						block->MoveDirection(Direc);
					}
				}
				//return Super::MoveDirection(Direc);
				return true;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "else");
				return Super::MoveDirection(Direc);
			}
		}	
		else
		{
			return false;
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

void AConstructPlatformPart::DetectConstructedBlock(TArray<AConstructPlatformPart*> DetectedBlocks)
{
	blocksToMove.Empty();
	FVector location = GetActorLocation();
	for (auto block : constructBlocks)
	{
		if (location.Equals(block->GetActorLocation(),151))
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
	APSGameMode* gm = Cast<APSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

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
			//construct wall
			if (LevelType == ELevelType::UnderCover)
			{
				
				AWallPlatformPart* wall = Cast<AWallPlatformPart>(frontHit.Actor);
				if (IsValid(wall))
				{
					FVector spawnLocation = wall->GetActorLocation();
					spawnLocation.Z = 0;
					if (wall->GetWallType() == EWallType::CrackedWall)
					{
						wall->StartDeadBox();
						gm->GetLevelPlatform()->SpawnAndAssignPathPart(spawnLocation, true);
					}
					else if (wall->GetWallType() == EWallType::HiddenWall)
					{
						wall->Destroy();
					}
					else
					{
						wall->Destroy();
						gm->GetLevelPlatform()->SpawnAndAssignPathPart(spawnLocation, true);
					}					
					return true;
				}
				else
				{
					return false;
				}
			}
			//
			AConstructPlatformPart* constract = Cast<AConstructPlatformPart>(frontHit.Actor);
			if (IsValid(constract))
			{
				return true;
			}
			return false;
		}
		else
		{
			APSPlatformPart* floorPart = Cast<APSPlatformPart>(buttomHit.Actor);
			if (IsValid(floorPart))
			{
				TArray<AEmptyWallPlatformPart*> emptyBlocks;
				AEmptyWallPlatformPart* currEmptyBlock = Cast<AEmptyWallPlatformPart>(buttomHit.Actor);
				AEmptyWallPlatformPart* emptyW;
				int count = 0;
				switch (floorPart->GetBoxType())
				{
					case EBoxType::Empty:
						emptyW = Cast<AEmptyWallPlatformPart>(buttomHit.Actor);
						if (IsValid(emptyW))
						{
							emptyBlocks = emptyW->GetNearEmptyWallParts();
							if ((emptyBlocks.Num() == blocksToMove.Num()) && (emptyBlocks.Num() > 0))
							{
								for (auto block : blocksToMove)
								{
									buttomLocation = block->GetActorLocation() * FVector::DownVector;
									UKismetSystemLibrary::BoxTraceSingle(world, buttomLocation, buttomLocation, FVector(5.f, 5.f, 5.f), FRotator::ZeroRotator, ETraceTypeQuery::TraceTypeQuery1, false,
										actToIngnore, EDrawDebugTrace::ForDuration, buttomHit, true);

									AEmptyWallPlatformPart* emptyBlock = Cast<AEmptyWallPlatformPart>(buttomHit.Actor);
									if (IsValid(emptyBlock))
									{
										count++;
									}
									else
									{
										break;
									}
								}

								if (count == blocksToMove.Num())
								{
									for (int i = 0; i < blocksToMove.Num() - 1; i++)
									{
										gm->GetLevelPlatform()->SpawnAndAssignPathPart(emptyBlocks[i]->GetActorLocation(), this->LevelType == ELevelType::UnderCover ? true : false);
										emptyBlocks[i]->Destroy();
										blocksToMove[i]->Destroy();
									}
								}
							}
							else if ((emptyBlocks.Num() == 0) && ( blocksToMove.Num() == 0))
							{
								gm->GetLevelPlatform()->SpawnAndAssignPathPart(emptyW->GetActorLocation(), this->LevelType == ELevelType::UnderCover ? true : false);
								emptyW->Destroy();
								this->Destroy();
							}
						}
						break;
					case EBoxType::Path:
						return true;
						break;
					default:
						return false;
						break;
				}
			}
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}
