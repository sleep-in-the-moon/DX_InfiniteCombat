#include "EditorExtensionModule.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "InternationalizationSettingsModel.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

IMPLEMENT_MODULE(FEditorExtension, EditorExtension);

void FEditorExtension::StartupModule()
{
    //ExtendByFExtend();
    ExtendByToolMenu();
}

void FEditorExtension::ShutdownModule()
{
}

void FEditorExtension::ExtendByFExtend()
{
    TSharedPtr<FExtender> MainExtender = MakeShareable(new FExtender);
    MainExtender->AddMenuBarExtension("Help", EExtensionHook::After, nullptr, FMenuBarExtensionDelegate::CreateRaw(this, &FEditorExtension::MakeButton));

    FLevelEditorModule& LevelEditorModule =
        FModuleManager::Get().LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    TSharedPtr<FExtensibilityManager> MenuExtensibilityManager =
        LevelEditorModule.GetMenuExtensibilityManager();

    MenuExtensibilityManager->AddExtender(MainExtender);
}

void FEditorExtension::MakeButton(FMenuBarBuilder& MenuBarBiulder)
{
    MenuBarBiulder.AddMenuEntry(FText::FromString("Zhong"), FText::FromString("Lang switch"), FSlateIcon(), FUIAction(FExecuteAction::CreateRaw(this, &FEditorExtension::ButtonAction)),"trans");
}

void FEditorExtension::ButtonAction()
{
    UE_LOG(LogTemp, Warning, TEXT("Hollo"));
    
    FString CurrentLang,NewLang,Locale;
    UInternationalizationSettingsModel* SettingsModel = GetMutableDefault<UInternationalizationSettingsModel>();
    if (!SettingsModel)
        return;

    //SettingsModel->GetEditorLanguage(CurrentLang);
    FCultureRef CurCulture = FInternationalization::Get().GetCurrentLanguage();
    CurrentLang = CurCulture->GetName();

    if (CurrentLang.Equals("en"))
    {
        NewLang = "zh-Hans";
        Locale = "zh";
    }
    else
    {
        NewLang = "en";
        Locale = "en";
    }

    SettingsModel->SetEditorLanguage(NewLang);
    SettingsModel->SetEditorLocale(Locale);
    FInternationalization::Get().SetCurrentLanguage(NewLang);
    FInternationalization::Get().SetCurrentLocale(Locale);
    
}

void FEditorExtension::ExtendByToolMenu()
{
    UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("MainFrame.MainMenu");

    FToolMenuSection& Section = Menu->FindOrAddSection(NAME_None); 

    /*FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton("LangTrans", FUIAction(FExecuteAction::CreateRaw(this, &FEditorExtension::ButtonAction)),
                            FText::FromString("Trans"), FText::FromString("ch en"), FSlateIcon()));
    FToolMenuEntry& MenuEntry = Section.AddMenuEntry("Zhong", FText::FromString("Lang switch"), FText::FromString("ch/en"), FSlateIcon(),
                                                     FToolUIActionChoice(FExecuteAction::CreateRaw(this, &FEditorExtension::ButtonAction)));*/

    FToolMenuEntry& MakeEntry = Section.AddSubMenu
    (
        "LangSwitch",
        FText::FromString(TEXT("LanguageSwitch")),
        FText::FromString(TEXT("LanguageSelect")),
        FNewToolMenuDelegate::CreateRaw(this, &FEditorExtension::AddMenuButton)
    );

    // 设置位置
    MakeEntry.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);
}

void FEditorExtension::AddMenuButton(UToolMenu* Menu)
{
    FToolMenuSection& Section = Menu->AddSection("zh");
    Section.AddMenuEntry(
        "zh",
        FText::FromString(TEXT("中文")),
        FText::FromString(TEXT("switch to zh")),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateRaw(this, &FEditorExtension::SwitchToChinese))
    );
    Section.AddMenuEntry(
        "en",
        FText::FromString(TEXT("英文")),
        FText::FromString(TEXT("switch to en")),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateRaw(this, &FEditorExtension::SwitchToEnglish))
    );
}

void FEditorExtension::SwitchEditorLang(const FString& NewLang, const FString& Locale)
{
    FCultureRef CurCulture = FInternationalization::Get().GetCurrentLanguage();
    if (CurCulture->GetName().Equals("NewLang"))
        return;

    UInternationalizationSettingsModel* SettingsModel = GetMutableDefault<UInternationalizationSettingsModel>();
    if (!SettingsModel)
        return;

    SettingsModel->SetEditorLanguage(NewLang);
    SettingsModel->SetEditorLocale(Locale);
    FInternationalization::Get().SetCurrentLanguage(NewLang);
    FInternationalization::Get().SetCurrentLocale(Locale);
}

void FEditorExtension::SwitchToChinese()
{
    SwitchEditorLang("zh-Hans", "zh");
}

void FEditorExtension::SwitchToEnglish()
{
    SwitchEditorLang("en", "en");
}
