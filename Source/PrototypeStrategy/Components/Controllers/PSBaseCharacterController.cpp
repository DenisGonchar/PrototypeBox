
#include "Components/Controllers/PSBaseCharacterController.h"

void APSBaseCharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedBaseCharacter = Cast<APSBaseCharacter>(InPawn);
}

void APSBaseCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveTop", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveTop);
	InputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveDown);
	InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveRight);
	InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveLeft);

	InputComponent->BindAction("ResetTop", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveTop);
	InputComponent->BindAction("ResetDown", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveDown);
	InputComponent->BindAction("ResetRight", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveRight);
	InputComponent->BindAction("ResetLeft", EInputEvent::IE_Pressed, this, &APSBaseCharacterController::MoveLeft);
}

void APSBaseCharacterController::MoveTop()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveTop();
	}
}

void APSBaseCharacterController::MoveDown()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveDown();
	}
}

void APSBaseCharacterController::MoveRight()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight();
	}
}

void APSBaseCharacterController::MoveLeft()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveLeft();
	}
}
