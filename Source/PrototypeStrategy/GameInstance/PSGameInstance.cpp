// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PSGameInstance.h"

#include "Actors/Platforms/Parts/MagneticPlatformPart.h"
#include "Actors/Platforms/Parts/TeleportPlatformPart.h"
#include "Actors/Platforms/Parts/WallPlatformPart.h"


void UPSGameInstance::SaveLevel(FLevelData levelData)
{
	if (!UGameplayStatics::DoesSaveGameExist(levelsSaveSlot,0))
	{
		gameSave = Cast<UPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSSaveGame::StaticClass()));

		if (IsValid(gameSave))
		{
			gameSave->levels.Add(levelData);
			UGameplayStatics::SaveGameToSlot(gameSave, levelsSaveSlot, 0);
		}
	}
	else
	{
		gameSave = Cast<UPSSaveGame>(UGameplayStatics::LoadGameFromSlot(levelsSaveSlot, 0));

		if (IsValid(gameSave))
		{
			gameSave->levels.Add(levelData);
			UGameplayStatics::SaveGameToSlot(gameSave, levelsSaveSlot, 0);
		}
	}	
}

FLevelData UPSGameInstance::LoadLevel(FString levelName)
{
	if (UGameplayStatics::DoesSaveGameExist(levelsSaveSlot, 0))
	{
		gameSave = Cast<UPSSaveGame>(UGameplayStatics::LoadGameFromSlot(levelsSaveSlot, 0));

		if (IsValid(gameSave))
		{
			for (auto level : gameSave->levels)
			{
				if (level.levelName == levelName)
				{
					return level;
				}
			}
		}
	}
	return FLevelData();
}

bool UPSGameInstance::LoadLevels(TArray<FLevelData>& levels)
{
	if (UGameplayStatics::DoesSaveGameExist(levelsSaveSlot, 0))
	{		
		gameSave = Cast<UPSSaveGame>(UGameplayStatics::LoadGameFromSlot(levelsSaveSlot, 0));

		if (IsValid(gameSave))
		{
			levels = gameSave->levels;
			return true;
		}
	}
	return false;
}

void UPSGameInstance::ValidateLevel(TArray<AActor*> actors)
{
	FVector traceEndLocation, startLocation;
	TArray<AActor*> actToIgnore;
	FHitResult hitResult;

	for(int i = 0; i < actors.Num(); i++)
	{		
		AActor* iActor = actors[i];
		for (int j = 0; j < actors.Num(); j++)
		{
			if(i == j) continue;
			if(iActor == actors[j]) continue;
			if(iActor->GetActorLocation().Equals(actors[j]->GetActorLocation()))
			{				
				actors[j]->Destroy();
				actors.RemoveAt(j);
			}
		}


		startLocation = FVector(iActor->GetActorLocation().X, iActor->GetActorLocation().Y,0);
		traceEndLocation = FVector(iActor->GetActorLocation().X, iActor->GetActorLocation().Y, 0);
		if(iActor->GetActorLocation().Z == 150.f)
		{
			UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, traceEndLocation, ETraceTypeQuery::TraceTypeQuery1, false, actToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);			
		}

		AMagneticPlatformPart* Magnetic = Cast<AMagneticPlatformPart>(iActor);
		if (IsValid(Magnetic))
		{
			if (Magnetic->GetActorLocation().Z == 150 && (Magnetic->MagneticType == EMagneticType::Activator || Magnetic->MagneticType == EMagneticType::Polarizer))
			{
				if(IsValid(hitResult.GetActor()))
				{
					hitResult.GetActor()->Destroy();
				}
				Magnetic->AddActorWorldOffset(FVector(0.f, 0.f, -150));
			}
		}
		AWallPlatformPart* Wall = Cast<AWallPlatformPart>(iActor);
		if (IsValid(Wall))
		{
			if (Wall->GetActorLocation().Z == 150)
			{
				if (IsValid(hitResult.GetActor()))
				{
					hitResult.GetActor()->Destroy();
				}
			}
		}

	}
}

void UPSGameInstance::RemoveSaveInSlot(FString slotName)
{
	if (slotName != "")
	{
		UGameplayStatics::DeleteGameInSlot(slotName, 0);
	}
	else
	{
		UGameplayStatics::DeleteGameInSlot(levelsSaveSlot, 0);
	}
}


