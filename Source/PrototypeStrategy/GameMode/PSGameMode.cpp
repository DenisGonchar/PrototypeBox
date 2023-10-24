// Copyright Epic Games, Inc. All Rights Reserved.

#include "PSGameMode.h"
#include <GameState/PSGameState.h>
#include <Kismet/GameplayStatics.h>
#include "Pawns/Character/PSBaseCharacter.h"
#include <PSTypes.h>


APSGameMode::APSGameMode()
{
	
}

void APSGameMode::BeginPlay()
{
	Super::BeginPlay();
	WState = Cast<APSGameState>(GetWorld()->GetGameState());
	BaseCharacter = Cast<APSBaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

}

int APSGameMode::GetFullSteps() const
{
	return FullSteps;
}

bool APSGameMode::GetWin() const
{
	return bIsWin;
}

void APSGameMode::SetFullSteps(int Full)
{
	FullSteps = Full;
}

void APSGameMode::MadeStep(int Index)
{

	if (IsValid(WState))
	{
		WState->IncrementSteps(Index);
		/*if (WState->GetSteps() >= FullSteps)
		{
			if (BaseCharacter->GetBoxType() != EBoxType::Exit)
			{
				CheckWin();
			}				
		}*/
	}
}

void APSGameMode::CheckWin()
{	
	bIsWin = false;
	FName CurrentName = (FName)UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), CurrentName);
}

void APSGameMode::SetLevelPlatform(APSPlatform* platf)
{
	levelPlatform = platf;
}

APSPlatform* APSGameMode::GetLevelPlatform()
{
	return levelPlatform;
}
