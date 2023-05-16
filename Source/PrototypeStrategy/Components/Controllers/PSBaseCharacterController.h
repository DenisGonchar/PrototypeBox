
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Pawns/Character/PSBaseCharacter.h"
#include "PSBaseCharacterController.generated.h"


UCLASS()
class PROTOTYPESTRATEGY_API APSBaseCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void SetPawn(APawn* InPawn) override;

protected:
	
	virtual void SetupInputComponent() override;

private:

	void MoveTop();
	void MoveDown();
	void MoveRight();
	void MoveLeft();



	TSoftObjectPtr<class APSBaseCharacter> CachedBaseCharacter;
};
