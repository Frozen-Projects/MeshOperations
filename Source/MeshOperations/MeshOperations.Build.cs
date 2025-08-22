// Some copyright should be here...

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
                // ... add other public dependencies that you statically link with here ...
				"Core",
			}
			);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "GLTFExporter",
                "RHI",
                "RenderCore",
                "MeshDescription",
                "StaticMeshDescription",
                "MeshConversion",
                "ProceduralMeshComponent",
				// ... add private dependencies that you statically link with here ...	
			}
			);
			
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
