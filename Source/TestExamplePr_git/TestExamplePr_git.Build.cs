// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestExamplePr_git : ModuleRules
{
	public TestExamplePr_git(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Json", "JsonUtilities", "FunctionalTesting", "AITestSuite", "GameplayTasks", "NavigationSystem" });
	}
}
