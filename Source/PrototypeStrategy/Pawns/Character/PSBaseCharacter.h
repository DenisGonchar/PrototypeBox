
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

class UPaperFlipbookComponent;

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

	virtual  void NotifyActorBeginOverlap(AActor* OtherActor) override;

	float GetMoveDistance() const;

	EMoveCharacterDirection GetCharacterDirection() const;

public:		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	//class UStaticMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbookComponent* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
		FConstructedBlockData constructionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPolarizationType PolarizationType = EPolarizationType::None;

	EMoveCharacterDirection CharacterDirection = EMoveCharacterDirection::None;

#pragma region Move
	virtual void MoveTop();
	virtual void MoveDown();
	virtual void MoveRight();
	virtual void MoveLeft();
	
	void MovementDirection(EMoveCharacterDirection Direction);
	void MoveToLocationType(APSPlatformPart* Box);
	void MoveToPosition(APSPlatformPart* Box);	
	void MoveToPosition(FVector Location);
	void MoveToPositionStart(APSPlatformPart* Box);

	void NormalizePlayerFlipbook();
	FTimerHandle spriteTimer;

	void Step(int Index);
	void FullStep();
	void SetFullSteps(int32 Step);

#pragma endregion

#pragma region Magntic
	void AddActualMagnetics(AMagneticPlatformPart* part);
	void FindNearestMagnetic();

	bool IsMagneticFindStarted = false;
	FTimerHandle startFindMagnetic;

#pragma endregion


	EBoxType GetBoxType();
	ELevelType GetLevelType();

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

	EBoxType BoxType = EBoxType::None;
	ELevelType LevelType = ELevelType::Level;

	TArray<AMagneticPlatformPart*> ActiveMagnetics;

	FName NameMap;
	FName NextLevel;

	class ULedgeDetectorComponent* LedgeDetertorComponent;

	class APSGameMode* GMode;
	
	TWeakObjectPtr<class APSPlatform>BasePlatform;
};
