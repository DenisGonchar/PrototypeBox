// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/PSPlatform.h"
#include "PSPlatformPart.h"
#include "Parts/BlockPlatformPart.h"
#include <PSTypes.h>
#include "Parts/TeleportPlatformPart.h"
#include <Pawns/Character/PSBaseCharacter.h>
#include <Kismet/GameplayStatics.h>
#include "Parts/CoverPlatformPart.h"
#include "Parts/MagneticPlatformPart.h"
#include "Parts/PathPlatformPart.h"
#include "GameInstance/PSGameInstance.h"
#include "Parts/ExitPlatformPart.h"
#include "GameMode/PSGameMode.h"
#include "Parts/MirroredPlatformPart.h"

APSPlatform::APSPlatform()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void APSPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (levelName != "")
	{
		UPSGameInstance* gameInstance = Cast<UPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		SpawnBlocks(gameInstance->LoadLevel(levelName).blocks);
		SpawnPlatformPartFloor(spawnedBlocks);
	}

	APSBaseCharacter* BSCharacter = Cast<APSBaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (IsValid(BSCharacter))
	{
		BSCharacter->SetFullSteps(FullSteps);
		BSCharacter->SetNameMap(NameMap);
	}

	APSGameMode* gm = Cast<APSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gm->SetLevelPlatform(this);
}

void APSPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void APSPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
TArray<ACoverPlatformPart*> APSPlatform::GetCoverPartsArray()
{
	return CoverParts;
}
void APSPlatform::SpawnAndAssignPathPart(FVector spawnLocation, bool IsUndecover)
{
	APathPlatformPart* newPathPart;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, pathPartClass->GetName());
	newPathPart = Cast<APathPlatformPart>(GetWorld()->SpawnActor<AActor>(pathPartClass, spawnLocation, FRotator::ZeroRotator));
	if (IsValid(newPathPart))
	{
		if (IsUndecover)
		{
			newPathPart->SetLevelType(ELevelType::UnderCover);
		}
		if (CoverParts.Num() > 0)
		{
			newPathPart->SetCoverPart(CoverParts);
		}
	}
}
/*
TArray<TSubclassOf<APSPlatformPart>> APSPlatform::GetGridParts() const
{
	return GridParts;
}*/

TArray<AActor*> APSPlatform::SpawnBlocks(TArray<FConstructedBlockData> blocks)
{	
	if (spawnedBlocks.Num() > 0)
	{
		for (auto block : spawnedBlocks)
		{
			if (IsValid(block))
			{
				block->Destroy();
			}
		}
		spawnedBlocks.Empty();
	}

	for (auto block : blocks)
	{
		spawnedBlocks.Add(GetWorld()->SpawnActor<AActor>(block.Block, block.location, block.rotation));
	}
	return spawnedBlocks;
}

void APSPlatform::SpawnPlatformPartFloor(TArray<AActor*> parts)
{
	for (auto part : parts)
	{
		APSPlatformPart* Actor = Cast<APSPlatformPart>(part);

		AConstructPlatformPart* ConstructBlock = Cast<AConstructPlatformPart>(Actor);
		if (IsValid(ConstructBlock))
		{
			constructBlocks.AddUnique(ConstructBlock);
		}

		ATeleportPlatformPart* Part = Cast<ATeleportPlatformPart>(Actor);
		if (IsValid(Part))
		{
			EBoxType GetType = Part->GetBoxType();

			if (GetType == EBoxType::Teleport)
			{
				if (Part->GetTeleportInfo().TeleportType == ETeleport::Activator)
				{
					ActivatorTeleport = Part;
				}
				else if (Part->GetTeleportInfo().TeleportType == ETeleport::Teleport)
				{
					ArrayTeleport.Add(Part);
				}
			}
		}

		AMagneticPlatformPart* MagneticPart = Cast<AMagneticPlatformPart>(Actor);
		if (IsValid(MagneticPart))
		{
			EBoxType GetType = MagneticPart->GetBoxType();
			if (GetType == EBoxType::Wall)
			{
				if (MagneticPart->MagneticType == EMagneticType::Magnetic)
				{
					MagneticArray.Add(MagneticPart);
				}
			}
			else if (GetType == EBoxType::Magnetic)
			{
				if (MagneticPart->MagneticType == EMagneticType::Activator)
				{
					MagneticActivator = MagneticPart;
				}
			}
		}
		
		AExitPlatformPart* ExitPart = Cast<AExitPlatformPart>(Actor);
		if (IsValid(ExitPart))
		{
			EBoxType GetType = ExitPart->GetBoxType();

			if (GetType == EBoxType::Exit)
			{
				ExitPart->SetOpenLevel(ExitMap);
			}
		}

		ACoverPlatformPart* Cover = Cast<ACoverPlatformPart>(Actor);
		if (IsValid(Cover))
		{
			EBoxType GetType = Cover->GetBoxType();

			if (GetType == EBoxType::Cover)
			{
				CoverParts.Add(Cover);
			}
		}

		ALimitedMovePlatformPart* Limited = Cast<ALimitedMovePlatformPart>(Actor);
		if (IsValid(Limited))
		{
			limitedBlocks.Add(Limited);
		}

		AMirroredPlatformPart* Mirror = Cast<AMirroredPlatformPart>(Actor);
		if (IsValid(Mirror))
		{
			EBoxType GetType = Mirror->GetBoxType();

			if (GetType == EBoxType::Mirrored)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Mirror->GetName());
				mirroredBlock = Mirror;
			}
			else if (GetType == EBoxType::MirroredClone)
			{
				GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Blue, Mirror->GetName());
				mirroredClones.Add(Mirror);
			}
		}


		APacmanPlatformPart* pacman = Cast<APacmanPlatformPart>(Actor);
		if (IsValid(pacman))
		{
			pacmansArray.AddUnique(pacman);
		}

		AEmptyWallPlatformPart* emptyWall = Cast<AEmptyWallPlatformPart>(Actor);
		if (IsValid(emptyWall))
		{
			emptyWallArray.AddUnique(emptyWall);
		}
	}

	for (auto contr : constructBlocks)
	{
		contr->SetConstractBlocksArray(constructBlocks);
	}

	if (IsValid(mirroredBlock))
	{
		mirroredBlock->clones = mirroredClones;
		for (auto clone : mirroredClones)
		{
			clone->generalMirrored = mirroredBlock;
		}
	}

	if(IsValid(MagneticActivator))
	{
		MagneticActivator->MagneticParts = MagneticArray;
	}
	
	if (CoverParts.Num() > 0)
	{
		for (auto part : parts)
		{
			APSPlatformPart* prt = Cast<APSPlatformPart>(part);
			if(IsValid(prt))
			{
				prt->SetCoverPart(CoverParts);
				//UE_LOG(LogTemp, Warning, TEXT("Type Level %i = %s"), -1, *UEnum::GetValueAsString(prt->GetLevelType()));
			}
		}
	}
	
	if(IsValid(ActivatorTeleport))
	{
		ActivatorTeleport->teleports = ArrayTeleport;
	}
	for (int t = 0; t < ArrayTeleport.Num(); t++)
	{
		if( t ==  ArrayTeleport.Num()-1)
		{
			ArrayTeleport[t]->SetTeleport(ArrayTeleport[0]);
			ArrayTeleport[t]->SetActivator(ActivatorTeleport);
			break;
		}
		
		ArrayTeleport[t]->SetTeleport(ArrayTeleport[t + 1]);
		ArrayTeleport[t]->SetActivator(ActivatorTeleport);		
	}

	for (int t = 0; t < pacmansArray.Num(); t++)
	{
		pacmansArray[t]->SetSteps(PacmanStepsArray[t]);
	}

	for (int t = 0; t < limitedBlocks.Num(); t++)
	{
		if (moveLimits.IsValidIndex(t))
		{
			limitedBlocks[t]->moveLimit = moveLimits[t];
		}
		else
		{
			limitedBlocks[t]->bUseRandomLimit = true;
		}
	}

	FVector emptyWallLocation;
	for (int i = 0; i < emptyWallArray.Num() -1; i++)
	{
		emptyWallLocation = emptyWallArray[i]->GetActorLocation();
		for (int j = 0; j < emptyWallArray.Num() -1; j++)
		{
			if (emptyWallLocation.Equals(emptyWallArray[j]->GetActorLocation(), 151))
			{
				emptyWallArray[i]->GetNearEmptyWallParts().AddUnique(emptyWallArray[j]);
			}
		}
	}
}

