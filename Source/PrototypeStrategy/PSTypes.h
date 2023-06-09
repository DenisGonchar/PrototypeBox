#pragma once
#include "Engine/DataTable.h"

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

UENUM(BlueprintType)
enum class EBlockConstructionType : uint8
{
	Floor,
	OnFloor,
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
struct FConstructionData : public FTableRowBase //для использования в DataTable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		int countLimit = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		EBlockConstructionType BlockConstructionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		bool bIsMovable = false;
};

USTRUCT(BlueprintType)
struct FConstructedBlockData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		UClass* Block;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		FVector location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		FRotator rotation;
};

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		FString levelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constructor")
		TArray<FConstructedBlockData> blocks;
};


