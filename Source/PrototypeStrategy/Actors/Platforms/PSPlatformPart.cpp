
#include "Actors/Platforms/PSPlatformPart.h"
#include "Components/ActivatorCoverComponent.h"
#include "Parts/CoverPlatformPart.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

APSPlatformPart::APSPlatformPart()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SceneComponent);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f), false);
	BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(SceneComponent);
	Flipbook->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	ActivatorCoverComponent = CreateDefaultSubobject<UActivatorCoverComponent>(TEXT("ActivatorCoverComponent"));

}

void APSPlatformPart::BeginPlay()
{
	Super::BeginPlay();
	
	BaseFlipbook = Flipbook->GetFlipbook();

}

void APSPlatformPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

UActivatorCoverComponent* APSPlatformPart::GetActivatorCoverComponent() const
{
	return ActivatorCoverComponent;
}

EBoxType APSPlatformPart::GetBoxType() const
{
	return BoxType;
}

void APSPlatformPart::SetCoverPart(ACoverPlatformPart* Actor)
{
	if (IsValid(Actor))
	{
		BaseCover = Actor;

		GetActivatorCoverComponent()->SetCover();
	}

}

ACoverPlatformPart* APSPlatformPart::GetCoverPart() const
{
	return BaseCover;
}

void APSPlatformPart::SetLevelType(ELevelType World)
{
	LevelType = World;

	//TODO
	//add new virtual void 

	NewLevelType();

}

ELevelType APSPlatformPart::GetLevelType() const
{
	return LevelType;
}

UPaperFlipbook* APSPlatformPart::GetBaseFlibook()
{
	return BaseFlipbook;
}

void APSPlatformPart::NewLevelType()
{
	switch (LevelType)
	{
		case ELevelType::Level:
		{
			if (IsValid(BaseFlipbook))
			{
				Flipbook->SetFlipbook(BaseFlipbook);

			}
			break;

		}
		case ELevelType::UnderCover:
		{
			if (IsValid(FlipbookCaver))
			{
				Flipbook->SetFlipbook(FlipbookCaver);
			}

			break;
		}
	}
}

void APSPlatformPart::AcceptConstructionData()
{
	SetActorLocation(constructionData.location);
	SetActorRotation(constructionData.rotation);
}

