// Copyright DX_IC


#include "UMG/WidgetCombatStates.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
//#include "Components/Widget.h"
#include "DX_ReusableTool/Public/DataStructure/DX_ObjectPool.h"

void UWidgetCombatStates::SpawnTransientWidgetByWorldLoc(const FVector& WorldLoc, float LifeTime, TSubclassOf<UUserWidget> WidgetClass)
{
	if (!GetOwningPlayer() || !WidgetClass)
		return;

	FVector2D ScreenPosition;
	if (!GetOwningPlayer()->ProjectWorldLocationToScreen(WorldLoc, ScreenPosition))
		return;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	if (!(ScreenPosition.X >= 0 && ScreenPosition.X <= ViewportSize.X &&
		ScreenPosition.Y >= 0 && ScreenPosition.Y <= ViewportSize.Y))
		return;

	if (!MainCanvasPanel)
		return;

	FVector2D CanvasSize = MainCanvasPanel->GetCachedGeometry().GetLocalSize();
	FVector2D CanvasPosition = FVector2D(ScreenPosition.X / ViewportSize.X * CanvasSize.X,
										 ScreenPosition.Y / ViewportSize.Y * CanvasSize.Y);

	TSharedPtr<DX_ObjectPool<UUserWidget>> WidgetPool;
	UUserWidget* Widget = WidgetPool->AccessObject();

	MainCanvasPanel->AddChildToCanvas(Widget);
	Widget->Slot; 
}

void UWidgetCombatStates::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!MainCanvasPanel)
	{
		MainCanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("MainCanvas"));
		WidgetTree->RootWidget = MainCanvasPanel;
	}
}
