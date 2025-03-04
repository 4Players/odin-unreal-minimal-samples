// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Odin_MinimalTarget : TargetRules
{
	public Odin_MinimalTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        bOverrideBuildEnvironment = true;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "Odin_Minimal" } );
	}
}
