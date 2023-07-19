// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/WallPlatformPart.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

void AWallPlatformPart::ActivatorCover()
{
	Super::ActivatorCover();

	switch (WallType)
	{
		case EWallType::CrackedWall :
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Cracked Wall")));

			break;
		}
	}
	
}

void AWallPlatformPart::NewLevelType()
{
	Super::NewLevelType();

	if (IsValid(FlipbookCaver))
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, FString::Printf(TEXT("Cover")));

	}
}

EWallType AWallPlatformPart::GetWallType() const
{
	return WallType;
}

void AWallPlatformPart::StartDeadBox()
{
	PlaySound(interactSound);
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	Flipbook->SetFlipbook(FlipbookDead);
	Flipbook->PlayFromStart();
	float Lenght = Flipbook->GetFlipbookLength();

	GetWorldTimerManager().SetTimer(WallTimer, this, &AWallPlatformPart::EndDeadBox, Lenght - 0.1f, false);
}

void AWallPlatformPart::EndDeadBox()
{
	Flipbook->Stop();
	GetWorld()->DestroyActor(this);

}
