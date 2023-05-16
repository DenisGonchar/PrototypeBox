
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <PSTypes.h>
#include <Components/LedgeDetectorComponent.h>
#include <Actors/Platforms/PSPlatformPart.h>
#include "PSBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenLevel, bool, bIsWin);

USTRUCT(BlueprintType)
struct FGetTeleportInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName GetTeleportName = FName("None");

	UPROPERTY()
	bool bIsTeleportType = false;

};

UCLASS()
class PROTOTYPESTRATEGY_API APSBaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	APSBaseCharacter();
	
	UPROPERTY(BlueprintAssignable)
	FOpenLevel OpenLevel;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	virtual void Tick(float DeltaTime) override;

	float GetMoveDistance() const;

	EMoveCharacterDirection GetCharacterDirection() const;

public:		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class UStaticMeshComponent* CharacterMesh;

	EMoveCharacterDirection CharacterDirection = EMoveCharacterDirection::None;

	virtual void MoveTop();
	virtual void MoveDown();
	virtual void MoveRight();
	virtual void MoveLeft();
	
	void MovementDirection(EMoveCharacterDirection Direction);
	void MoveToLocationType(APSPlatformPart* Box);
	void MoveToPosition(APSPlatformPart* Box);

	void Step(int Index);
	void FullStep();
	void SetFullSteps(int32 Step);

	EBoxType GetBoxType();

	UFUNCTION(BlueprintCallable)
	FName GetNameMap() const;
	void SetNameMap(FName Map);

	UFUNCTION(BlueprintCallable)
	FName GetNestLevel() const;
	void SetNextLevel(FName Map);

	FGetTeleportInfo TelepInfo;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveDistance = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params")
	int StepIndex = 1;
	int FullStepIndex = 1;

	EBoxType BoxType;

	FName NameMap;
	FName NextLevel;

	class ULedgeDetectorComponent* LedgeDetertorComponent;

	class APSGameMode* GMode;
	
	TWeakObjectPtr<class APSPlatform>BasePlatform;
};
