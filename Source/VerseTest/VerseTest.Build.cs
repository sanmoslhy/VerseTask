// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VerseTest : ModuleRules
{
	public VerseTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"ChaosVehicles",
			"PhysicsCore",
			"UMG",
			"Slate",
            "NavigationSystem"
        });

		PublicIncludePaths.AddRange(new string[] {
			"VerseTest",
			"VerseTest/SportsCar",
			"VerseTest/OffroadCar",
			"VerseTest/Variant_OffRoad",
			"VerseTest/Variant_TimeTrial",
			"VerseTest/Variant_TimeTrial/UI"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
