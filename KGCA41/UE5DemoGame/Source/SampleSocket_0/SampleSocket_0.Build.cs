// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SampleSocket_0 : ModuleRules
{
	public SampleSocket_0(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","SlateCore", "Slate", "HeadMountedDisplay","UMG", "Sockets", "Networking", "AIModule","NavigationSystem","TBasisCore", "TNetCore" });
	}
}
