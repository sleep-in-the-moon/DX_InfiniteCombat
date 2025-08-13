// Copyright DX_IC


#include "UMG/WidgetCombatStates.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
//#include "Components/Widget.h"
#include "Animation/WidgetAnimation.h"

void UWidgetCombatStates::SpawnTransientWidgetByWorldLoc(const FVector& WorldLoc, TSubclassOf<UUserWidget> WidgetClass, float TransientTime, const FString& ShowInfos)
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

	if (!WidgetPool)
		return;

	UUserWidget* Widget = WidgetPool->AccessObject(this, WidgetClass);
	MainCanvasPanel->AddChildToCanvas(Widget);

	if (WidgetClass.Get())
	{
		if (UFunction* InitFunc = WidgetClass.Get()->FindFunctionByName(FName(TEXT("Init"))))
		{
			struct InitParam
			{
				FString Infos;
			};
			InitParam InitFuncParam;
			InitFuncParam.Infos = ShowInfos;
			Widget->ProcessEvent(InitFunc, &InitFuncParam);
		}

		FProperty* TransientAnimProp = WidgetClass.Get()->FindPropertyByName(FName(TEXT("TransientAnim")));
		if (TransientAnimProp && TransientAnimProp->GetCPPType().Equals("UWidgetAnimation*"))
		{
			FClassProperty* TransientAnimClassProp = static_cast<FClassProperty*>(TransientAnimProp);

			const void* ValuePtr = TransientAnimClassProp->ContainerPtrToValuePtr<void>(Widget);
			TObjectPtr<UObject> TransientAnimObj = TransientAnimClassProp->GetPropertyValue(ValuePtr);

			UWidgetAnimation* TransientAnim = Cast<UWidgetAnimation>(TransientAnimObj);
			Widget->PlayAnimationForward(TransientAnim);
			
		}
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	if (!CanvasSlot)
		return;

	CanvasSlot->SetPosition(CanvasPosition);

	UE_LOG(LogTemp, Warning, TEXT("CanvasPosition::%s"),*CanvasPosition.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CanvasPosition::%s"), *CanvasPosition.ToString()));

	if (TransientTime > 0)
	{
		FTimerHandle LifeTimer;
		GetOwningPlayer()->GetWorld()->GetTimerManager().SetTimer(LifeTimer, [this, Widget, TransientTime]()
			{
				WidgetPool->RecycleObject(Widget);
				Widget->SetVisibility(ESlateVisibility::Collapsed);
				//GetOwningPlayer()->GetWorld()->GetTimerManager().ClearTimer(LifeTimer);
			}, TransientTime, false);
	}
	/*else if (TransientTime == 0)
	{
		GetOwningPlayer()->GetWorld()->GetTimerManager().SetTimerForNextTick([this, Widget]()
			{
				WidgetPool->RecycleObject(Widget);
				Widget->SetVisibility(ESlateVisibility::Collapsed);
			});
	}*/

}

//void UWidgetCombatStates::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//
//	if (!MainCanvasPanel)
//	{
//		MainCanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("MainCanvas"));
//		WidgetTree->RootWidget = MainCanvasPanel;
//	}
//}

void UWidgetCombatStates::NativeConstruct()
{
	Super::NativeConstruct();
	WidgetPool = MakeUnique<DX_ObjectPool<UUserWidget> >();
}

void UWidgetCombatStates::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//TODO::Update TransientWidget's position
}
