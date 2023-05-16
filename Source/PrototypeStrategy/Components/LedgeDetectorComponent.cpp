// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LedgeDetectorComponent.h"
#include <Pawns/Character/PSBaseCharacter.h>
#include <Components/BoxComponent.h>
#include "Subsystems/DebugSubsystem.h"
#include "PSTypes.h"
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include <Utils/PSTraceUtils.h>
#include <Actors/Platforms/PSPlatformPart.h>
#include "Actors/Platforms/Parts/MovePlatformPart.h"


void ULedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedPawnOwner = StaticCast<APSBaseCharacter*>(GetOwner());
	
	CachedActorOwner = StaticCast<AMovePlatformPart*>(GetOwner());

}

bool ULedgeDetectorComponent::StartDetectDown(OUT FLedgeDescription& LedgeDescription)
{
	FHitResult VecticalCheckHitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

	FVector Start = CachedPawnOwner->GetActorLocation();
	FVector End = Start + CachedPawnOwner->GetMoveDistance() * FVector::DownVector;
	if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), VecticalCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, false, DrawTime, DrawColor))
	{
		return false;
	}

	LedgeDescription.BoxMesh = Cast<APSPlatformPart>(VecticalCheckHitResult.Actor);
	
	return true;
}

bool ULedgeDetectorComponent::StartBoxDetectDown(OUT FLedgeDescription& LedgeDescription)
{
	FHitResult VecticalCheckHitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());
	
	if (!CachedActorOwner.IsValid())
	{
		return false;
	}

	FVector Start = CachedActorOwner->GetActorLocation();
	FVector End = Start + CachedActorOwner->GetMoveDistance() * FVector::DownVector;
	if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), VecticalCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, false, DrawTime, DrawColor))
	{
		return false;
	}

	LedgeDescription.BoxMesh = Cast<APSPlatformPart>(VecticalCheckHitResult.Actor);

	return true;
}

bool ULedgeDetectorComponent::DetectLedge(OUT FLedgeDescription& LedgeDescription, OUT EMoveCharacterDirection Direction)
{
	CharacterDirection = Direction;
	
	UBoxComponent* BaxComponent = CachedPawnOwner->BoxComponent;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else
	bool bIsDebugEnabled = false;
#endif
	
	//Forward
	FVector OwnerLocation = CachedPawnOwner->GetActorLocation();
	FVector OwnerForwardVector = CachedPawnOwner->GetActorForwardVector();
	FVector OwnerRightVector = CachedPawnOwner->GetActorRightVector();

	float OwnerDirection = CachedPawnOwner->GetMoveDistance();

	bool ForwardCkeck = true;
	FHitResult ForwardCheckHitResult;
	
	FVector Start = FVector::ZeroVector;
	FVector End = FVector::ZeroVector;

	switch(Direction)
	{
		case EMoveCharacterDirection::Top:
		{
			Start = OwnerLocation;
			End = Start + OwnerForwardVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;

			}
			else
			{
				if (DetectHitBlock(ForwardCheckHitResult))
				{
					ForwardCkeck = false;
				}
			
			}
			
			break;
		}

		case EMoveCharacterDirection::Down:
		{
			Start = OwnerLocation;
			End = Start + -OwnerForwardVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;
			}
			else
			{
				if (DetectHitBlock(ForwardCheckHitResult))
				{
					ForwardCkeck = false;
				}

			}

		
			break;
		}
		case EMoveCharacterDirection::Right:
		{
			Start = OwnerLocation;
			End = Start + OwnerRightVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;
			}
			else
			{
				if (DetectHitBlock(ForwardCheckHitResult))
				{
					ForwardCkeck = false;
				}

			}


			break;
		}
		case EMoveCharacterDirection::Left:
		{
			Start = OwnerLocation;
			End = Start + -OwnerRightVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;
			}
			else
			{
				if (DetectHitBlock(ForwardCheckHitResult))
				{
					ForwardCkeck = false;
				}

			}


			break;
		}
	}
	
	//Downaward ceck
	FHitResult VecticalCheckHitResult;
	if (!ForwardCkeck)
	{
		Start = End;
		End = Start + OwnerDirection * FVector::DownVector;
		if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), VecticalCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
		{
			return false;
		}
		
		APSPlatformPart* BoxFloor = Cast<APSPlatformPart>(VecticalCheckHitResult.Actor);
		if (!IsValid(BoxFloor))
		{
			return false;
		}

		LedgeDescription.BoxMesh = BoxFloor;
		LedgeDescription.Type = BoxFloor->GetBoxType();

		return true;
	}
	

	return false;


}

bool ULedgeDetectorComponent::BoxDetectLedge(OUT FLedgeDescription& LedgeDescription, OUT EMoveCharacterDirection Direction)
{
	CharacterDirection = Direction;

	//UBoxComponent* BaxComponent = CachedPawnOwner->BoxComponent;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else
	bool bIsDebugEnabled = false;
#endif

	//Forward
	FVector OwnerLocation = CachedActorOwner->GetActorLocation();
	FVector OwnerForwardVector = CachedActorOwner->GetActorForwardVector();
	FVector OwnerRightVector = CachedActorOwner->GetActorRightVector();

	float OwnerDirection = CachedActorOwner->GetMoveDistance();

	bool ForwardCkeck = true;
	FHitResult ForwardCheckHitResult;

	FVector Start = FVector::ZeroVector;
	FVector End = FVector::ZeroVector;

	switch (Direction)
	{
		case EMoveCharacterDirection::Top:
		{
			Start = OwnerLocation;
			End = Start + OwnerForwardVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;

			}
		
			break;
		}

		case EMoveCharacterDirection::Down:
		{
			Start = OwnerLocation;
			End = Start + -OwnerForwardVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;
			}

			break;
		}
		case EMoveCharacterDirection::Right:
		{
			Start = OwnerLocation;
			End = Start + OwnerRightVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;
			}

			break;
		}
		case EMoveCharacterDirection::Left:
		{
			Start = OwnerLocation;
			End = Start + -OwnerRightVector * OwnerDirection;

			if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), ForwardCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
			{
				ForwardCkeck = false;
			}


			break;
		}
	}

	//Downaward ceck
	FHitResult VecticalCheckHitResult;
	if (!ForwardCkeck)
	{
		Start = End;
		End = Start + OwnerDirection * FVector::DownVector;
		if (!PSTraceUtils::LineTraceSingleByChannel(GetWorld(), VecticalCheckHitResult, Start, End, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime, DrawColor))
		{
			return false;
		}

		APSPlatformPart* BoxFloor = Cast<APSPlatformPart>(VecticalCheckHitResult.Actor);
		if (!IsValid(BoxFloor))
		{
			return false;
		}

		LedgeDescription.BoxMesh = BoxFloor;
		LedgeDescription.Type = BoxFloor->GetBoxType();

		return true;
	}


	return false;

}

bool ULedgeDetectorComponent::DetectHitBlock(FHitResult Hit)
{
	AMovePlatformPart* Box = Cast<AMovePlatformPart>(Hit.Actor);
	
	if (IsValid(Box))
	{

		if (Box->GetBoxType() == EBoxType::Wall)
		{
			return false;
		}

		if (Box->MoveDirection(CharacterDirection))
		{
			return true;
		}

	}

	return false;
}


