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
#include "../../GameInstance/PSGameInstance.h"
#include "Parts/ExitPlatformPart.h"

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

}

void APSPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void APSPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
			block->Destroy();
		}
		spawnedBlocks.Empty();
	}

	for (auto block : blocks)
	{
		spawnedBlocks.Add(GetWorld()->SpawnActor<AActor>(block.Block, block.location, FRotator::ZeroRotator));
	}
	return spawnedBlocks;
}

void APSPlatform::SpawnPlatformPartFloor(TArray<AActor*> parts)
{
	/*
	for (int i = 0; i <= MaxIndex - 1; i++)
	{
		Floor.Add(FloorPart);

	}

	for (int j = 0; j < Floor.Num(); j++)
	{	
		if (GridParts.Num() - 1 < j)
		{
			GridParts.Add(Floor[j]);
		}

		if (GridParts[j] == nullptr)
		{
			GridParts[j] = Floor[j];

		}
		
	}

	int X = 0;
	int Y = 0;
	*/
	for (auto part : parts)
	{
		/*
		if (!(Y < HorizontalIndex))
		{
			Y = 0;
			X += 1;
		}

		FVector SpawnLocation = GetRootComponent()->GetComponentLocation();
		SpawnLocation.X = SpawnLocation.X + (-X * Distance);
		SpawnLocation.Y += SpawnLocation.Y + (Y * Distance);
		
		APSPlatformPart* SpawnActors = GetWorld()->SpawnActor<APSPlatformPart>(GridParts[l], SpawnLocation, FRotator::ZeroRotator);
		
		if (IsValid(SpawnActors))
		{
			ArrayPlatformPart.Add(SpawnActors);
			
			ABlockPlatformPart* Block = Cast<ABlockPlatformPart>(SpawnActors);
			if (IsValid(Block->MovePlatform))
			{
				FVector SpawnLocationBlock = Block->GetActorLocation();
				SpawnLocationBlock.Z += Block->GetSpawnDistance();

				APSPlatformPart* SpawnBlock = GetWorld()->SpawnActor<APSPlatformPart>(Block->MovePlatform, SpawnLocationBlock, FRotator::ZeroRotator);
				if (IsValid(SpawnBlock))
				{
					ArrayPlatformPart.Add(SpawnBlock);
				}
			}
		}*/
		APSPlatformPart* Actor = Cast<APSPlatformPart>(part);


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
			if (GetType == EBoxType::Magnetic)
			{
				if (MagneticPart->MagneticType == EMagneticType::Magnetic)
				{
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Emerald,"Array");
					MagneticArray.Add(MagneticPart);
				}
				else if (MagneticPart->MagneticType == EMagneticType::Activator)
				{
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Emerald,"Activator1");
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
				CoverPart = Cover;
			}

		}	
	}
	if(IsValid(MagneticActivator))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Emerald,"Activator12");
		MagneticActivator->MagneticParts = MagneticArray;
	}
	
	for (int p = 0; p < ArrayPlatformPart.Num(); p++)
	{
		if (IsValid(CoverPart))
		{
			ArrayPlatformPart[p]->SetCoverPart(CoverPart);

			UE_LOG(LogTemp, Warning, TEXT("Type Level %i = %s"), p, *UEnum::GetValueAsString(ArrayPlatformPart[p]->GetLevelType()));
		}
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
}

