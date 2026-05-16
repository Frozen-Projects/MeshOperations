using System;
using System.IO;
using UnrealBuildTool;

public class MeshOperations : ModuleRules
{
	public MeshOperations(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;
        bEnableExceptions = true;

        if (UnrealTargetPlatform.Win64 == Target.Platform)
        {
            bUseRTTI = true;
        }

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
				"Core",
                "MeshDescription",
                "StaticMeshDescription",
                "MeshConversion",
                "ProceduralMeshComponent",
                "GLTFExporter",
            });
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "RHI",
                "RenderCore",
			});
	}
}
