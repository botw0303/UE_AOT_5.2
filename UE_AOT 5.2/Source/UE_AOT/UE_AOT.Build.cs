// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_AOT : ModuleRules
{
	public UE_AOT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "UE_AOT" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
                                                            "NavigationSystem", "AIModule", "GameplayTasks"});
    }
}
