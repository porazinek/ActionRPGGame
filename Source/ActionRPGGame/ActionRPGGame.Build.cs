// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRPGGame : ModuleRules
{
    public ActionRPGGame(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", "CoreUObject", "Engine", "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
		{
            "Slate",
            "SlateCore"
					// ... add private dependencies that you statically link with here ...
		});
    }
}
