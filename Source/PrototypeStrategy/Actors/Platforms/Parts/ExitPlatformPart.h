// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Platforms/Parts/BlockPlatformPart.h"
#include "ExitPlatformPart.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESTRATEGY_API AExitPlatformPart : public ABlockPlatformPart
{
	GENERATED_BODY()
	
	
public:

	virtual void BeginPlay() override;

	virtual void NewLevelType() override;

	virtual void ActivatorCover() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params | Exit")
	bool bIsActivCaver = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Params | Exit")
	bool bIsActivLevel = false;

	FName GetOpenLevel() const;
	void SetOpenLevel(FName Map);

	bool GetIsActivCaver() const;
	bool GetIsActivLevel() const;

protected:

	FName OpenLevel;
	
	
};
