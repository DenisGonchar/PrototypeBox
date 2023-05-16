
#include "Actors/Platforms/PSPlatformPart.h"

APSPlatformPart::APSPlatformPart()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxMesh);

}

void APSPlatformPart::BeginPlay()
{
	Super::BeginPlay();
	
}

void APSPlatformPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EBoxType APSPlatformPart::GetBoxType() const
{
	return BoxType;
}

