// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PrototypeStrategy : ModuleRules
{
	public PrototypeStrategy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateIncludePaths.AddRange(new string[] { Name });

    }
}
