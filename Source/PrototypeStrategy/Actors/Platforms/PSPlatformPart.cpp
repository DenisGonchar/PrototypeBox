
#include "Actors/Platforms/PSPlatformPart.h"
#include "Components/ActivatorCoverComponent.h"
#include "Parts/CoverPlatformPart.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"

APSPlatformPart::APSPlatformPart()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetupAttachment(SceneComponent);
	BoxComponent->SetBoxExtent(FVector(75.0f, 75.0f, 75.0f), false);
	BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	Flipbook->SetupAttachment(SceneComponent);
	Flipbook->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(SceneComponent);
	Sprite->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(SceneComponent);
	
	ActivatorCoverComponent = CreateDefaultSubobject<UActivatorCoverComponent>(TEXT("ActivatorCoverComponent"));
}

void APSPlatformPart::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Flipbook))
	{
		BaseFlipbook = Flipbook->GetFlipbook();
	}
	if (IsValid(Sprite))
	{
		BaseSprite = Sprite->GetSprite();
	}

}

void APSPlatformPart::PlaySound(USoundBase* soudToPlay)
{
	Audio->SetSound(soudToPlay);
	Audio->Play(1.f);
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

void APSPlatformPart::SetCoverPart(TArray<ACoverPlatformPart*> Actors)
{
	if (Actors.Num() > 0)
	{
		BaseCover = Actors;

		GetActivatorCoverComponent()->SetCover();
	}

}

TArray<ACoverPlatformPart*> APSPlatformPart::GetCoverPart() const
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

			if (IsValid(BaseSprite))
			{
				Sprite->SetSprite(BaseSprite);
			}
				
			break;

		}
		case ELevelType::UnderCover:
		{
			if (IsValid(FlipbookCaver))
			{
				Flipbook->SetFlipbook(FlipbookCaver);
			}
				
			if (IsValid(SpriteCaver))
			{
				Sprite->SetSprite(SpriteCaver);
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

