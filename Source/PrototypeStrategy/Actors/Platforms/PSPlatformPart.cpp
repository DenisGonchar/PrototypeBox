
#include "Actors/Platforms/PSPlatformPart.h"
#include "Components/ActivatorCoverComponent.h"
#include "Parts/CoverPlatformPart.h"

APSPlatformPart::APSPlatformPart()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxMesh);
	
	ActivatorCoverComponent = CreateDefaultSubobject<UActivatorCoverComponent>(TEXT("ActivatorCoverComponent"));

}

void APSPlatformPart::BeginPlay()
{
	Super::BeginPlay();
	
	
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
	UE_LOG(LogTemp, Warning, TEXT("Type Level = %s"), *UEnum::GetValueAsString(LevelType));
}

ELevelType APSPlatformPart::GetLevelType() const
{
	return LevelType;
}

