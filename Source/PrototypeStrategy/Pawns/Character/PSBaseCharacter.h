
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <PSTypes.h>
#include <Components/LedgeDetectorComponent.h>
#include <Actors/Platforms/PSPlatformPart.h>
#include "../../GameInstance/PSGameInstance.h"
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

private:
	UPSGameInstance* gameInstance;

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
	bool IsPlayerStep = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flipbook")
	UPaperFlipbookComponent* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
		FConstructedBlockData constructionData;

	EMoveCharacterDirection CharacterDirection = EMoveCharacterDirection::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* moveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* moveBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* moveLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* moveRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* pushForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* pushBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* pushLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* pushRight;



#pragma region Move
	virtual void MoveTop();
	virtual void MoveDown();
	virtual void MoveRight();
	virtual void MoveLeft();
	
	void MovementDirection(EMoveCharacterDirection Direction);
	void MoveToLocationType();
	void MoveToPosition(APSPlatformPart* Box);	
	void MoveToPosition(FVector Location);
	void MoveToPositionStart(APSPlatformPart* Box);

	FTimerHandle spriteTimer;

	void Step(int Index);
	void FullStep();
	void SetFullSteps(int32 Step);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float moveTimerSpeed = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float scaleChangeSpeed = 0.05f;

	FTimerHandle moveTimerHandle;
	FVector deltaLocation;
	FVector targetLocation;
	FVector startLocation;
	bool bIsMoveFinished = true;
	void MoveCharacterOnTimer();

	FTimerHandle playFlipbookAnimHandle;

#pragma endregion

#pragma region Magntic
	void AddActualMagnetics(AMagneticPlatformPart* part);
	void FindNearestMagnetic();

	bool IsMagneticFindStarted = false;
	FTimerHandle startFindMagnetic;

	TArray<AMagneticPlatformPart*> ActiveMagnetics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPolarizationType polarizationType = EPolarizationType::None;

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

	UFUNCTION(BlueprintImplementableEvent)
		void CharacterMoved();

	int FullStepIndex = 1;

	EBoxType BoxType = EBoxType::None;
	ELevelType LevelType = ELevelType::Level;


	FName NameMap;
	FName NextLevel;

	class ULedgeDetectorComponent* LedgeDetertorComponent;

	class APSGameMode* GMode;
	
	TWeakObjectPtr<class APSPlatform>BasePlatform;


	APSPlatformPart* detectedBlock;

	EMoveCharacterDirection lastDirection;
	void PlayAnimation(UAnimationAsset* animToPlay);
	void PushAndMove();
	void MoveWithAnim();
	UAnimationAsset* GetAnimationByDirection(EMoveCharacterDirection direction, bool bIsPushAnim = false);

	FTimerHandle moveTimer;
	FTimerHandle pushTimer;

	float pushDelay = 0.8f;
	float moveDelay = 0.2f;
};
