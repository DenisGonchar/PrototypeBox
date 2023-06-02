
#include "Actors/Platforms/PSPlatformPart.h"
#include "Components/ActivatorCoverComponent.h"
#include "Parts/CoverPlatformPart.h"
#include "Components/SceneComponent.h"

APSPlatformPart::APSPlatformPart()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComponent);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxComponent"));
	BoxMesh->SetupAttachment(SceneComponent);
	
	ActivatorCoverComponent = CreateDefaultSubobject<UActivatorCoverComponent>(TEXT("ActivatorCoverComponent"));

}

void APSPlatformPart::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMaterial = BoxMesh->GetMaterial(0);

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

UMaterialInterface* APSPlatformPart::GetBaseMaterial()
{
	return BaseMaterial;
}

void APSPlatformPart::NewLevelType()
{
	switch (LevelType)
	{
		case ELevelType::Level:
		{
			if (IsValid(BaseMaterial))
			{
				BoxMesh->SetMaterial(0, BaseMaterial);

			}
			break;

		}
		case ELevelType::UnderCover:
		{
			if (IsValid(MaterialCaver))
			{
				BoxMesh->SetMaterial(0, MaterialCaver);
			}

			break;
		}
	}
}

