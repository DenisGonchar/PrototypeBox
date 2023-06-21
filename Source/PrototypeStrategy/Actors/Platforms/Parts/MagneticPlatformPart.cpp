
#include "Actors/Platforms/Parts/MagneticPlatformPart.h"
#include <Components/ActivatorClientComponent.h>

#include "DrawDebugHelpers.h"
#include "Pawns/Character/PSBaseCharacter.h"
#include "Utils/PSTraceUtils.h"


AMagneticPlatformPart::AMagneticPlatformPart()
{
}

void AMagneticPlatformPart::BeginPlay()
{
	Super::BeginPlay();

	if(polarization != EPolarizationType::None)
	{
		polarization = FMath::RandBool() ? EPolarizationType::Negative : EPolarizationType::Positive;
	}
}


void AMagneticPlatformPart::UpdateStatus(bool newStatus)
{
	bIsActive = newStatus;
	ChangeMaterial(bIsActive);
	
	if(bIsActive && MagneticType == EMagneticType::Magnetic)
	{
		GetWorldTimerManager().SetTimer(magneticTimer, this, &AMagneticPlatformPart::CheckPlayer, 0.2f, true);
	}
	else
	{
		if(!GetWorldTimerManager().TimerExists(magneticTimer))
		{
			GetWorldTimerManager().ClearTimer(magneticTimer);
		}
	}
}

void AMagneticPlatformPart::ChangeMagneticsStatus(TArray<AMagneticPlatformPart*> parts, bool newStatus)
{
	UpdateStatus(newStatus);
	for(auto part : MagneticParts)
	{
		part->UpdateStatus(newStatus);
	}
}

void AMagneticPlatformPart::SwitchActivator()
{
	ChangeMagneticsStatus(MagneticParts,!bIsActive);
}

void AMagneticPlatformPart::Magnetic(APSBaseCharacter* player)
{
	float distance = GetDistanceTo(player);
	int distanceInTiles = distance /150;
	if(polarization == EPolarizationType::Negative)
	{
		
	}
	else if(polarization == EPolarizationType::Positive)
	{
		
	}
}

void AMagneticPlatformPart::CheckPlayer()
{
	FVector traceEndLocation, startLocation;
	startLocation = FVector(GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z + 250);
	traceEndLocation = GetActorLocation() + (GetActorForwardVector() * 800);
	PSTraceUtils::LineTraceSingleByChannel(GetWorld() ,traceResult ,startLocation ,traceEndLocation, ECC_Visibility);
	if(traceResult.Actor != nullptr)
	{
		APSBaseCharacter* player = Cast<APSBaseCharacter>(traceResult.Actor);
		if(IsValid(player))
		{
			player->AddActualMagnetics(this);
		}
	}
}


