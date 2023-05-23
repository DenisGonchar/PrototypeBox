// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/PSPlatform.h"
#include "PSPlatformPart.h"
#include "Parts/BlockPlatformPart.h"
#include <PSTypes.h>
#include "Parts/TeleportPlatformPart.h"
#include <Pawns/Character/PSBaseCharacter.h>
#include <Kismet/GameplayStatics.h>

APSPlatform::APSPlatform()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void APSPlatform::BeginPlay()
{
	Super::BeginPlay();

	SpawnPlatformPartFloor();

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

TArray<TSubclassOf<APSPlatformPart>> APSPlatform::GetGridParts() const
{
	return GridParts;
}

void APSPlatform::SpawnPlatformPartFloor()
{
	
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

	for (int l = 0; l < GridParts.Num(); l++)
	{
		if (!(Y < HorizontalIndex))
		{
			Y = 0;
			X += 1;
		}

		FVector SpawnLocation = GetRootComponent()->GetComponentLocation();
		SpawnLocation.X = SpawnLocation.X + (X * Distance);
		SpawnLocation.Y += SpawnLocation.Y + (Y * Distance);
		
		APSPlatformPart* SpawnActors = GetWorld()->SpawnActor<APSPlatformPart>(GridParts[l], SpawnLocation, FRotator::ZeroRotator);
		
		ATeleportPlatformPart* Part = Cast<ATeleportPlatformPart>(SpawnActors);
		if (IsValid(Part))
		{
			UE_LOG(LogTemp, Warning, TEXT("Teleport %.2f"), l);

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
		
		ABlockPlatformPart* BlockPart = Cast<ABlockPlatformPart>(SpawnActors);
		if (IsValid(BlockPart))
		{
			EBoxType GetType = BlockPart->GetBoxType();

			if (GetType == EBoxType::Exit)
			{
				BlockPart->SetOpenLevel(ExitMap);
			}
		}

		Y++;
	
	}
	
	for (int t = 0; t < ArrayTeleport.Num(); t++)
	{
		if( t ==  ArrayTeleport.Num())
		{
			ArrayTeleport[t]->SetTeleport(ArrayTeleport[0]);
			ArrayTeleport[t]->SetActivator(ActivatorTeleport);
			break;
		}
		
		ArrayTeleport[t]->SetTeleport(ArrayTeleport[t + 1]);
		ArrayTeleport[t]->SetActivator(ActivatorTeleport);
	}
}

