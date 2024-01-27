// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PSGameInstance.h"


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


