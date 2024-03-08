
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
	MoveLocation = TargetLocation;
	StartLocation = GetActorLocation();
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Cyan,UEnum::GetValueAsString(polarization));
}


void AMagneticPlatformPart::MagneticSwitched()
{
	if (!GetWorldTimerManager().IsTimerActive(moveTimer))
	{
		GetWorldTimerManager().PauseTimer(moveTimer);
		GetWorldTimerManager().ClearTimer(moveTimer);
		bIsActive = false;
	}
	else
	{
		GetWorldTimerManager().SetTimer(moveTimer, this, &AMagneticPlatformPart::CallMoveMagnetic,0.1f,true);
		bIsActive = true;
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

void AMagneticPlatformPart::SwitchPolarization()
{
	switch (polarization)
	{
		case EPolarizationType::Positive:
			polarization = EPolarizationType::Negative;
		case EPolarizationType::Negative:
			polarization = EPolarizationType::Positive;
		default:
			break;
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
	for(auto part : MagneticParts)
	{
		part->bIsActivCaver = !part->bIsActivCaver;
		part->MagneticSwitched();
	}
}

void AMagneticPlatformPart::SwitchActivator()
{
	ChangeMagneticsStatus(MagneticParts,!bIsActive);
}

void AMagneticPlatformPart::InitMagnetic(bool IsMagneticActive)
{
	if (IsMagneticActive) GetWorldTimerManager().SetTimer(magneticTimer, this, &AMagneticPlatformPart::CheckPlayer, 0.2f, true);
}

//void AMagneticPlatformPart::MagneticPlayer(APSBaseCharacter* player)
//{
//	if (IsValid(this))
//	{
//		EMoveCharacterDirection moveDirection = FindMagneticDirection(player->GetActorLocation());
//		if (!bIsActive) return;
//
//		player->IsPlayerStep = false;
//		
//		if (player->polarizationType == EPolarizationType::Negative)
//		{
//			if (polarization == EPolarizationType::Positive)
//			{
//				moveDirection = ReversDirection(moveDirection);
//				player->MovementDirection(moveDirection);
//			}
//			else if (polarization == EPolarizationType::Negative)
//			{
//				player->MovementDirection(moveDirection);
//			}
//		}
//		else if (player->polarizationType == EPolarizationType::Positive)
//		{
//			if (polarization == EPolarizationType::Positive)
//			{				
//				player->MovementDirection(moveDirection);
//			}
//			else if (polarization == EPolarizationType::Negative)
//			{
//				moveDirection = ReversDirection(moveDirection);
//				player->MovementDirection(moveDirection);
//			}
//		}
//	}
//}


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
	int distance = bIsActive ? 650 : 500;
	traceForwardEndLocation = GetActorLocation() + (GetActorForwardVector() * distance);
	traceBackEndLocation = GetActorLocation() + (GetActorForwardVector() * -distance);
	traceRightEndLocation = GetActorLocation() + (GetActorRightVector() * distance);
	traceLeftEndLocation = GetActorLocation() + (GetActorRightVector() * -distance);
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
	if (GetActorLocation().Equals(actor->GetActorLocation(), 151.f) && bIsActive)
	{
		return;
	}


	APSBaseCharacter* player = Cast<APSBaseCharacter>(actor);
	if (IsValid(player) && bIsActive)
	{
		player->AddActualMagnetics(this);
		return;
	}
	else
	{
		/*AMagneticPlatformPart* magnetic = Cast<AMagneticPlatformPart>(actor);
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
		}*/
	}
}

void AMagneticPlatformPart::CallMoveMagnetic()
{
	MoveMagnetic(FindMagneticDirection(MoveLocation));
}

void AMagneticPlatformPart::MoveMagnetic(EMoveCharacterDirection moveDirection)
{
	FHitResult hit;
	TArray<AActor*> actToIgnore;
	actToIgnore.Add(this);
	FVector location = GetLocationByDirection(moveDirection);
	if (!UKismetSystemLibrary::BoxTraceSingle(GetWorld(), location, location, FVector(155.f, 155.f, 5.f), FRotator(), ETraceTypeQuery::TraceTypeQuery1, true, actToIgnore, EDrawDebugTrace::ForDuration, hit, true))
	{
		if (hit.Actor == nullptr || hit.Actor == this)
		{
			SetActorLocation(location);
			if (GetActorLocation().Equals(MoveLocation, 5.f))
			{
				SetActorLocation(MoveLocation);
				MoveLocation = MoveLocation == TargetLocation ? StartLocation : TargetLocation;
				GetWorldTimerManager().PauseTimer(moveTimer);
				GetWorldTimerManager().ClearTimer(moveTimer);
			}
		}
		else
		{
			GetWorldTimerManager().PauseTimer(moveTimer);
			GetWorldTimerManager().ClearTimer(moveTimer);
		}
	}
}

FVector AMagneticPlatformPart::GetLocationByDirection(EMoveCharacterDirection Direction)
{
	switch (Direction)
	{
		case EMoveCharacterDirection::Top:
			return GetActorLocation() + FVector(5.f, 0.f, 0.f);
			break;
		case EMoveCharacterDirection::Down:
			return GetActorLocation() - FVector(5.f, 0.f, 0.f);
			break;
		case EMoveCharacterDirection::Left:
			return GetActorLocation() - FVector(0.f, 5.f, 0.f);
			break;
		case EMoveCharacterDirection::Right:
			return GetActorLocation() + FVector(0.f, 5.f, 0.f);
			break;
		case EMoveCharacterDirection::None:
			return GetActorLocation();
			break;
		default:
			break;
	}
	return GetActorLocation();
}


