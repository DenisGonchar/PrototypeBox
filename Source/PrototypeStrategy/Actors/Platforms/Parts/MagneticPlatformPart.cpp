
#include "Actors/Platforms/Parts/MagneticPlatformPart.h"
#include <Components/ActivatorClientComponent.h>

#include "DrawDebugHelpers.h"
#include "Pawns/Character/PSBaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


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


void AMagneticPlatformPart::SwitchSprite(EPolarizationType newPolarization)
{
	switch (newPolarization)
	{
		case EPolarizationType::None:
			ChangeBlockSprite(PositivePolarizationSprite);
			break;
		case EPolarizationType::Positive:
			ChangeBlockSprite(NegativePolarizationSprite);
			break;
		case EPolarizationType::Negative:
			ChangeBlockSprite(NonPolarizationSprite);
			break;
		default:
			break;
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
	if (IsValid(this))
	{
		EMoveCharacterDirection moveDirection = FindMagneticDirection(player);

		player->IsPlayerStep = false;
		
		if (player->polarizationType == EPolarizationType::Negative)
		{
			if (polarization == EPolarizationType::Positive)
			{
				moveDirection = ReversDirection(moveDirection);
				player->MovementDirection(moveDirection);
			}
			else if (polarization == EPolarizationType::Negative)
			{
				player->MovementDirection(moveDirection);
			}
		}
		else if (player->polarizationType == EPolarizationType::Positive)
		{
			if (polarization == EPolarizationType::Positive)
			{				
				player->MovementDirection(moveDirection);
			}
			else if (polarization == EPolarizationType::Negative)
			{
				moveDirection = ReversDirection(moveDirection);
				player->MovementDirection(moveDirection);
			}
		}
	}
}

EMoveCharacterDirection AMagneticPlatformPart::FindMagneticDirection(APSBaseCharacter* player)
{
	FVector playerLocation = player->GetActorLocation();
	FVector magneticLocation = GetActorLocation();
	
	if (playerLocation.X == magneticLocation.X)
	{
		if (magneticLocation.Y < playerLocation.Y)
		{
			return EMoveCharacterDirection::Left;
		}
		else
		{
			return EMoveCharacterDirection::Right;
		}
	}
	else if(playerLocation.Y == magneticLocation.Y)
	{
		if (magneticLocation.X < playerLocation.X)
		{
			return EMoveCharacterDirection::Down;
		}
		else
		{
			return EMoveCharacterDirection::Top;
		}
	}
	return EMoveCharacterDirection::None;
}

EMoveCharacterDirection AMagneticPlatformPart::ReversDirection(EMoveCharacterDirection direction)
{
	switch (direction)
	{
		case EMoveCharacterDirection::Top:
			return EMoveCharacterDirection::Down;
			break;
		case EMoveCharacterDirection::Down:
			return EMoveCharacterDirection::Top;
			break;
		case EMoveCharacterDirection::Right:
			return EMoveCharacterDirection::Left;
			break;
		case EMoveCharacterDirection::Left:
			return EMoveCharacterDirection::Right;
			break;
		default:
			break;
	}
	return EMoveCharacterDirection::None;
}

void AMagneticPlatformPart::CheckPlayer()
{
	FVector traceEndLocation, startLocation;
	startLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	traceEndLocation = GetActorLocation() + (GetActorForwardVector() * 650);
	TArray<AActor*> actToIgnore;
	actToIgnore.Add(this);
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),startLocation,traceEndLocation,ETraceTypeQuery::TraceTypeQuery1,false, actToIgnore, EDrawDebugTrace::ForDuration, traceResult, true))
	{
		if (traceResult.Actor != nullptr)
		{
			APSBaseCharacter* player = Cast<APSBaseCharacter>(traceResult.Actor);
			if (IsValid(player))
			{
				player->AddActualMagnetics(this);
			}
		}
	}
}


