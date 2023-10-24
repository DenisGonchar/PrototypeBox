// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/WallColorPlatformPart.h"

//#include "DiffUtils.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PSTypes.h"
#include "Components/BoxComponent.h"

void AWallColorPlatformPart::ActivatorCover()
{
	Super::ActivatorCover();
	
}

void AWallColorPlatformPart::NewLevelType()
{
	Super::NewLevelType();

	switch (LevelType)
	{
		case ELevelType::Level:
		{
			BoxComponent->SetBoxExtent(StartBoxExtent);
			BoxComponent->SetWorldLocation(StartBoxLocation);
			
			break;
		}
		case ELevelType::UnderCover:
		{
			StartBoxExtent = BoxComponent->GetScaledBoxExtent();
			StartBoxLocation = BoxComponent->GetComponentLocation();
			
			BoxComponent->SetBoxExtent(FVector(StartBoxExtent.X, StartBoxExtent.Y, StartBoxExtent.Z * 0.5));
			BoxComponent->SetWorldLocation(FVector(StartBoxLocation.X, StartBoxLocation.Y, StartBoxLocation.Z - StartBoxExtent.Z - 15.0f));
			break;
		}
		default: 
			break;
	}
	
}

void AWallColorPlatformPart::SetMoveMaterials(UPaperFlipbook* ActorFlipbook /*= nullptr*/, UPaperSprite* ActorSprite /*= nullptr*/)
{
	if (ActorFlipbook != nullptr)
	{
		PawnFlipbook = ActorFlipbook;
		if (IsValid(PawnFlipbook))
		{
			if (PawnFlipbook == FlipbookCaver)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT(" Flipbook = True")));

			}
		}
	}
	if (ActorSprite != nullptr)
	{
		PawnSprite = ActorSprite;
		if (IsValid(PawnSprite))
		{
			if (PawnSprite /*== SpiteCover*/ )
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Sprite = True")));
			}
		}
	}
}

void AWallColorPlatformPart::BeginPlay()
{
	Super::BeginPlay();

	
}
