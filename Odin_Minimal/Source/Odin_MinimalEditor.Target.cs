// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Odin_MinimalEditorTarget : TargetRules
{
	public Odin_MinimalEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        bOverrideBuildEnvironment = true;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.Latest;

        ExtraModuleNames.AddRange( new string[] { "Odin_Minimal" } );
	}
}
