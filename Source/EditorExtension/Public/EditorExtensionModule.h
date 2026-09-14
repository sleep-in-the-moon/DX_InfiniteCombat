#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FEditorExtension : public FDefaultModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void ExtendByFExtend();

	void MakeButton(FMenuBarBuilder& MenuBarBiulder);
	void ButtonAction();

	void ExtendByToolMenu();
	void AddMenuButton(UToolMenu* Menu);
	void SwitchEditorLang(const FString& NewLang, const FString& Locale);
	void SwitchToChinese();
	void SwitchToEnglish();
};
