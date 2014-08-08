// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRPGGame : ModuleRules
{
    public ActionRPGGame(TargetInfo Target)
    {
        PublicIncludePaths.AddRange(new string[]
            {
                "ActionRPGGame/JsonDataAsset",
                "ActionRPGGame/Public"
            });
        PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", "CoreUObject", "Engine", "InputCore", "GameplayTags", "JsonUtilities"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
		{
            "Slate",
            "SlateCore",
            "EditorStyle"
					// ... add private dependencies that you statically link with here ...
		});
        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", "PropertyEditor" });
        }
    }
}
