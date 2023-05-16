// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugSubsystem.h"

bool UDebugSubsystem::IsCategoryEnabled(const FName& CategoryName) const
{
	const bool* bIsEnabled = EnabledDebugCategories.Find(CategoryName);
	
	return bIsEnabled != nullptr && *bIsEnabled;
}

void UDebugSubsystem::EnableDebugCategory(const FName& CategoryName, bool bIsEnabled)
{
	EnabledDebugCategories.FindOrAdd(CategoryName);
	EnabledDebugCategories[CategoryName] = bIsEnabled;
}
