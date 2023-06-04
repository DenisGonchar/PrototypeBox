#pragma once

#include "PSTypes.generated.h"


const FName DebugCategoryLedgeDetection = FName("LedgeDetection");

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	None,
	Level,
	UnderCover

};

UENUM(BlueprintType)
enum class EMoveCharacterDirection : uint8
{
	None,
	Top,
	Down,
	Right,
	Left

};

UENUM(BlueprintType)
enum class EBoxType : uint8
{
	None,
	Path,
	Wall, 
	CrackedWall,
	Dynamic,
	Exit,
	Cover,
	Teleport,
	Magnetic,
};

UENUM(BlueprintType)
enum class EDynamic : uint8
{
	None,
	Passive,
	Active

};

UENUM(BlueprintType)
enum class ETeleport : uint8
{
	None,
	Activator,
	Teleport
};

UENUM(BlueprintType)
enum class EMagneticType : uint8
{
	None,
	Activator,
	Polarizer,
	Magnetic,
};

UENUM(BlueprintType)
enum class EPolarizationType : uint8
{
	None,
	Positive,
	Negative,
};

UENUM(BlueprintType)
enum class EWallType : uint8
{
	None,
	DefaultWall,
	CrackedWall

};

USTRUCT(BlueprintType)
struct FTeleportInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	ETeleport TeleportType = ETeleport::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	FName NameTeleport;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teleport")
	bool bIsTeleportType = false;
};

USTRUCT(BlueprintType)
struct FMagneticData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magnetic")
	AActor* MagneticActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magnetic")
	FVector location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magnetic")
	EPolarizationType polarization;
};