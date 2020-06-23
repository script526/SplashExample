// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlmazCPP : ModuleRules
{
	public AlmazCPP(ReadOnlyTargetRules Target) : base(Target)
	{
        //Enable IWYU but keep our PrivatePCH in use
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Make sure UBT reminds us of how to keep the project IWYU compliant
        bEnforceIWYU = true;

        // omits subfolders from public include paths to reduce compiler command line length.
        //bLegacyPublicIncludePaths = false;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
