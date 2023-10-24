
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
		PlaySound(activateSound);
		GetWorldTimerManager().SetTimer(magneticTimer, this, &AMagneticPlatformPart::CheckPlayer, 0.2f, true);
	}
	else
	{
		PlaySound(deactivateSound);
		if(GetWorldTimerManager().TimerExists(magneticTimer))
		{
			GetWorldTimerManager().ClearTimer(magneticTimer);
		}
	}
}

void AMagneticPlatformPart::NewLevelType()
{
	Super::NewLevelType();

	if (GetLevelType() == ELevelType::UnderCover)
	{
		if (bIsActivCaver && bIsActivLevel)
		{
			if (BaseFlipbook)
			{
				Flipbook->SetFlipbook(BaseFlipbook);
			}

			if (BaseSprite)
			{
				Sprite->SetSprite(BaseSprite);
			}
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

void AMagneticPlatformPart::MagneticPlayer(APSBaseCharacter* player)
{
	if (IsValid(this))
	{
		EMoveCharacterDirection moveDirection = FindMagneticDirection(player->GetActorLocation());

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

void AMagneticPlatformPart::Magnetic(AMagneticPlatformPart* magneticPart)
{
	if (IsValid(this))
	{
		EMoveCharacterDirection moveDirection = FindMagneticDirection(magneticPart->GetActorLocation());

		if (magneticPart->polarization == EPolarizationType::Negative)
		{
			if (polarization == EPolarizationType::Positive)
			{
				moveDirection = ReversDirection(moveDirection);
				MoveMagnetic(moveDirection);
			}
			else if (polarization == EPolarizationType::Negative)
			{
				MoveMagnetic(moveDirection);
			}
		}
		else if (magneticPart->polarization == EPolarizationType::Positive)
		{
			if (polarization == EPolarizationType::Positive)
			{
				MoveMagnetic(moveDirection);
			}
			else if (polarization == EPolarizationType::Negative)
			{
				moveDirection = ReversDirection(moveDirection);
				MoveMagnetic(moveDirection);
			}
		}
	}
}

EMoveCharacterDirection AMagneticPlatformPart::FindMagneticDirection(FVector location)
{
	FVector magneticLocation = GetActorLocation();
	
	if (location.X == magneticLocation.X)
	{
		if (magneticLocation.Y < location.Y)
		{
			return EMoveCharacterDirection::Left;
		}
		else
		{
			return EMoveCharacterDirection::Right;
		}
	}
	else if(location.Y == magneticLocation.Y)
	{
		if (magneticLocation.X < location.X)
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
	FVector traceBackEndLocation, startLocation, traceForwardEndLocation, traceRightEndLocation, traceLeftEndLocation;
	startLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	traceForwardEndLocation = GetActorLocation() + (GetActorForwardVector() * 650);
	traceBackEndLocation = GetActorLocation() + (GetActorForwardVector() * -650);
	traceRightEndLocation = GetActorLocation() + (GetActorRightVector() * 650);
	traceLeftEndLocation = GetActorLocation() + (GetActorRightVector() * -650);
	TArray<AActor*> actToIgnore;
	actToIgnore.Add(this);

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, traceForwardEndLocation, ETraceTypeQuery::TraceTypeQuery1,false, actToIgnore, EDrawDebugTrace::ForDuration, traceResult, true))
	{
		if (traceResult.Actor != nullptr)
		{
			MagneticFinded(Cast<AActor>(traceResult.Actor));
		}
	}
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, traceBackEndLocation, ETraceTypeQuery::TraceTypeQuery1, false, actToIgnore, EDrawDebugTrace::ForDuration, traceResult, true))
	{
		if (traceResult.Actor != nullptr)
		{
			MagneticFinded(Cast<AActor>(traceResult.Actor));
		}
	}
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, traceRightEndLocation, ETraceTypeQuery::TraceTypeQuery1, false, actToIgnore, EDrawDebugTrace::ForDuration, traceResult, true))
	{
		if (traceResult.Actor != nullptr)
		{
			MagneticFinded(Cast<AActor>(traceResult.Actor));
		}
	}
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, traceLeftEndLocation, ETraceTypeQuery::TraceTypeQuery1, false, actToIgnore, EDrawDebugTrace::ForDuration, traceResult, true))
	{
		if (traceResult.Actor != nullptr)
		{
			MagneticFinded(Cast<AActor>(traceResult.Actor));
		}
	}

	
}

void AMagneticPlatformPart::MagneticFinded(AActor* actor)
{
	if (GetActorLocation().Equals(actor->GetActorLocation(), 151.f))
	{
		return;
	}


	APSBaseCharacter* player = Cast<APSBaseCharacter>(actor);
	if (IsValid(player))
	{
		player->AddActualMagnetics(this);
		return;
	}
	else
	{
		AMagneticPlatformPart* magnetic = Cast<AMagneticPlatformPart>(actor);
		if (IsValid(magnetic))
		{
			if (magnetic->MagneticType == EMagneticType::Magnetic)
			{
				if (!magnetic->bIsHaveBadplate)
				{
					magnetic->Magnetic(this);
					return;
				}
			}
		}
	}
}

void AMagneticPlatformPart::MoveMagnetic(EMoveCharacterDirection moveDirection)
{
	FHitResult hit;
	TArray<AActor*> actToIgnore;
	actToIgnore.Add(this);
	FVector location = GetLocationByDirection(moveDirection);
	if (!UKismetSystemLibrary::BoxTraceSingle(GetWorld(), location, location, FVector(25.f,25.f,25.f), FRotator(), ETraceTypeQuery::TraceTypeQuery1, true, actToIgnore, EDrawDebugTrace::ForDuration, hit, true))
	{
		//if (hit.Actor == nullptr)
		//{
			SetActorLocation(location);
		//}
	}
}

FVector AMagneticPlatformPart::GetLocationByDirection(EMoveCharacterDirection Direction)
{
	switch (Direction)
	{
		case EMoveCharacterDirection::Top:
			return GetActorLocation() + FVector(150.f, 0.f, 0.f);
			break;
		case EMoveCharacterDirection::Down:
			return GetActorLocation() - FVector(150.f, 0.f, 0.f);
			break;
		case EMoveCharacterDirection::Left:
			return GetActorLocation() - FVector(0.f, 150.f, 0.f);
			break;
		case EMoveCharacterDirection::Right:
			return GetActorLocation() + FVector(0.f, 150.f, 0.f);
			break;
		case EMoveCharacterDirection::None:
			return GetActorLocation();
			break;
		default:
			break;
	}
	return GetActorLocation();
}


