using UnrealBuildTool;

public class BachelorEditor : ModuleRules
{
	public BachelorEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd", "FunctionalTesting", "Bachelor", "AIModule"});
	}
}