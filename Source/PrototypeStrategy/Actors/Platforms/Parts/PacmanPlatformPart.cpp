// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Platforms/Parts/PacmanPlatformPart.h"
#include "Kismet/KismetSystemLibrary.h"

void APacmanPlatformPart::SetSteps(FPacmanSteps newSteps)
{
	stepsStruct = newSteps;
	UpdateMoveData();
}

void APacmanPlatformPart::BeginPlay()
{
	Super::BeginPlay();	
	actToIgnore.Add(this);
	worldRef = GetWorld();
}

bool APacmanPlatformPart::MoveDirection(EMoveCharacterDirection Direc)
{
	FVector traceLocation = FindTraceLocationByDirection(Direc, 150.f);
	UKismetSystemLibrary::LineTraceSingle(worldRef, GetActorLocation(), traceLocation, ETraceTypeQuery::TraceTypeQuery1, true, actToIgnore, EDrawDebugTrace::ForDuration, traceResult, true);
	if (LevelType == ELevelType::UnderCover)
	{			
		APSPlatformPart* hitActor = Cast<APSPlatformPart>(traceResult.Actor);
		if (IsValid(hitActor))
		{
			EBoxType boxType;
			boxType = hitActor->GetBoxType();
			switch (boxType)
			{
				case EBoxType::None:
					break;
				/*case EBoxType::Path:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Wall:
					hitActor->Destroy();
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Dynamic:
					AMovePlatformPart* movePlatform = Cast<AMovePlatformPart>(hitActor);
					if (IsValid(movePlatform))
					{
						if (!movePlatform->MoveDirection(Direc))
						{
							movePlatform->Destroy();
							SetActorLocation(traceLocation);
							return true;
						}
					}
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Exit:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Cover:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Teleport:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Magnetic:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Polarizator:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Mirrored:
					AMirroredPlatformPart* mirroredPlatform = Cast<AMirroredPlatformPart>(hitActor);
					if (IsValid(mirroredPlatform))
					{
						if (!mirroredPlatform->MoveDirection(Direc))
						{
							mirroredPlatform->Destroy();
							SetActorLocation(traceLocation);
							return true;
						}
					}
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::MirroredClone:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::MirroredBorder:
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::Construct:
					AConstructPlatformPart* constrPlatform = Cast<AConstructPlatformPart>(hitActor);
					if (IsValid(constrPlatform))
					{
						constrPlatform->IsMovedByCharacter = true;
						if (!constrPlatform->MoveDirection(Direc))
						{
							constrPlatform->Destroy();
							SetActorLocation(traceLocation);
							return true;
						}
					}
					SetActorLocation(traceLocation);
					return true;
					break;
				case EBoxType::PacmanTrap:
					SetActorLocation(traceLocation);
					this->Destroy();
					return false;
					break;*/
				default:
					break;
			}
		}
		else
		{
			SetActorLocation(traceLocation);
			return true;
		}
	}
	else
	{
		SetActorLocation(traceLocation);
		return true;
	}
	return false;
	//return Super::MoveDirection(Direc);
}

void APacmanPlatformPart::UpdateMoveData()
{
	directions = stepsStruct.directions;
	directionCount = stepsStruct.directionCount;
}

void APacmanPlatformPart::NextStep()
{
	if (directionCount.Num() > 0)
	{
		for (int i = 0; i <= directionCount.Num() - 1; i++ )
		{
			if (directions.IsValidIndex(i))
			{
				if (directionCount[i] > 0)
				{
					MoveDirection(directions[i]);
					directionCount[i] -= 1;
					//currentStep = i;
					break;
				}
			}
		}
		if (directionCount.Last() == 0)
		{
			UpdateMoveData();
		}
	}
}

FVector APacmanPlatformPart::FindTraceLocationByDirection(EMoveCharacterDirection direction, float traceDistance)
{
	FVector currentLocation = this->GetActorLocation();
	switch (direction)
	{
		case EMoveCharacterDirection::Top:
			return FVector(currentLocation.X + traceDistance, currentLocation.Y, currentLocation.Z);
			break;
		case EMoveCharacterDirection::Down:
			return FVector(currentLocation.X - traceDistance, currentLocation.Y, currentLocation.Z);
			break;
		case EMoveCharacterDirection::Right:
			return FVector(currentLocation.X, currentLocation.Y + traceDistance, currentLocation.Z);
			break;
		case EMoveCharacterDirection::Left:
			return FVector(currentLocation.X, currentLocation.Y - traceDistance, currentLocation.Z);
			break;
		default:
			break;
	}
	return FVector();
}


bool APacmanPlatformPart::CheckObstacles(EMoveCharacterDirection directionToCheck)
{
	FVector location = FindTraceLocationByDirection(directionToCheck, 150.f);
	FVector buttomLocation = location;
	buttomLocation.Z = 0;
	TArray<AActor*> actToIngnore;
	actToIngnore.Add(this);
	UObject* world = GetWorld();

	FHitResult frontHit;
	FHitResult buttomHit;

	UKismetSystemLibrary::BoxTraceSingle(world, location, location, FVector(5.f, 5.f, 5.f), FRotator::ZeroRotator, ETraceTypeQuery::TraceTypeQuery1, false,
		actToIngnore, EDrawDebugTrace::ForDuration, frontHit, true);
	UKismetSystemLibrary::BoxTraceSingle(world, buttomLocation, buttomLocation, FVector(5.f, 5.f, 5.f), FRotator::ZeroRotator, ETraceTypeQuery::TraceTypeQuery1, false,
		actToIngnore, EDrawDebugTrace::ForDuration, buttomHit, true);

	if (buttomHit.bBlockingHit)
	{
		if (frontHit.bBlockingHit)
		{
			
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}
