using UnrealBuildTool;

public class EditorExtension: ModuleRules
{
    public EditorExtension(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "Slate", "SlateCore", "ToolMenus" , "UnrealEd", "InternationalizationSettings" });
    }
}
