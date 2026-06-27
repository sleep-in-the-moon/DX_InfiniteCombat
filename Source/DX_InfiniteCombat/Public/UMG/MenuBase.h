// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

/**
 * TODO::UIÕ»Ê½µ÷ÓÃ
 */
UCLASS()
class DX_INFINITECOMBAT_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void PushMenu(TSubclassOf<UUserWidget> NewMenuClass);
	UFUNCTION(BlueprintCallable)
	void PopMenu();

private:
	UPROPERTY()
	TArray<UUserWidget*> WidgetStack;

};
