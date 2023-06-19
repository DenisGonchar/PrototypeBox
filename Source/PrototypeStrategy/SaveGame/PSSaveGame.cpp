// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/PSSaveGame.h"

void UPSSaveGame::SaveLevel(FLevelData levelData)
{
   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, levelData.levelName);
   levels.Add(levelData);
   for (auto level : levels)
   {
       GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, level.levelName);
   }
}

FLevelData UPSSaveGame::LoadLevel(FString levelName)
{
    for (auto level : levels)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, level.levelName);
        if (level.levelName == levelName)
        {
            return level;
        }
    }
    return FLevelData();
}

TArray<FLevelData> UPSSaveGame::LoadLevels()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(levels.Num()));
    for (auto level : levels)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, level.levelName); 
    }
    return levels;
}

