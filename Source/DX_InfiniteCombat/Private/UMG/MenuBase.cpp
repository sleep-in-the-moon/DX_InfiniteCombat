// Copyright DX_IC


#include "UMG/MenuBase.h"

void UMenuBase::PushMenu(TSubclassOf<UUserWidget> NewMenuClass)
{
	if (!NewMenuClass)
		return;

	UUserWidget* NewMenu = CreateWidget<UUserWidget>(GetOwningPlayer(), NewMenuClass);

	WidgetStack.Push(NewMenu);
}

void UMenuBase::PopMenu()
{
	UUserWidget* PopWidget = WidgetStack.Pop();
}
