// Copyright DX_IC


#include "UMG/WidgetCombatStates.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Animation/WidgetAnimation.h"

void UWidgetCombatStates::SpawnTransientWidgetByActor(AActor* AttachActor, TSubclassOf<UUserWidget> WidgetClass, float TransientTime, const FString& ShowInfos)
{
	if (!WidgetClass || !AttachActor ||!MainCanvasPanel)
		return;

	FVector2D CanvasPosition = GetCanvasPositionByWorldLoc(AttachActor->GetActorLocation());
	TransientWidgetAttachActor = MakeWeakObjectPtr(AttachActor);

	if (!WidgetPool)
		return;

	UUserWidget* Widget = WidgetPool->AccessObject(this, WidgetClass);
	UCanvasPanelSlot* CanvasSlot = MainCanvasPanel->AddChildToCanvas(Widget);
	if (!CanvasSlot)
		return;

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

void UWidgetCombatStates::RegisterPersistentWidget(FName UniqueID, FPersistentWidgetHandle RegisteredWidgetHandle)
{
	if (UniqueID.IsNone() || !RegisteredWidgetHandle.PersistentWidget.IsValid() || !MainCanvasPanel)
		return;

	MainCanvasPanel->AddChildToCanvas(RegisteredWidgetHandle.PersistentWidget.Get());
	PersistentWidgets.Add(UniqueID, RegisteredWidgetHandle);
}

void UWidgetCombatStates::UnRegisterPersistentWidget(FName UniqueID)
{
	if (PersistentWidgets.Contains(UniqueID))
	{
		MainCanvasPanel->RemoveChild(PersistentWidgets.Find(UniqueID)->PersistentWidget.Get());
		PersistentWidgets.Remove(UniqueID);
	}
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

	//Update TransientWidget's position
	if (WidgetPool && WidgetPool->GetAllActives().Num() > 0 && TransientWidgetAttachActor.IsValid())
	{
		for (TObjectPtr<UUserWidget> widget : WidgetPool->GetAllActives())
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(widget->Slot))
				CanvasSlot->SetPosition(GetCanvasPositionByWorldLoc(TransientWidgetAttachActor.Get()->GetActorLocation()));
		}
	}
	//Update PersistentWidget's position
	for (auto& map : PersistentWidgets)
	{
		if (!map.Value.PersistentWidget.IsValid())
			continue;

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(map.Value.PersistentWidget->Slot);
		if (!CanvasSlot)
			continue;

		switch (map.Value.WidgetAttachMode)
		{
		case EWidgetAttachMode::AttachToLocation:
			CanvasSlot->SetPosition(GetCanvasPositionByWorldLoc(map.Value.AttachedLocation));
			break;
		case EWidgetAttachMode::AttachToActor:
			if(map.Value.AttachedActor)
				CanvasSlot->SetPosition(GetCanvasPositionByWorldLoc(map.Value.AttachedActor->GetActorLocation()));
			break;
		default:
			break;
		}
	}
}

FVector2D UWidgetCombatStates::GetCanvasPositionByWorldLoc(const FVector& WorldLoc)
{
	if (!GetOwningPlayer())
		return FVector2D();

	FVector2D ScreenPosition;
	if (!GetOwningPlayer()->ProjectWorldLocationToScreen(WorldLoc, ScreenPosition))
		return FVector2D();

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	if (!(ScreenPosition.X >= 0 && ScreenPosition.X <= ViewportSize.X &&
		ScreenPosition.Y >= 0 && ScreenPosition.Y <= ViewportSize.Y))
		return FVector2D();

	if (!MainCanvasPanel)
		return FVector2D();

	FVector2D CanvasSize = MainCanvasPanel->GetCachedGeometry().GetLocalSize();
	FVector2D CanvasPosition = FVector2D(ScreenPosition.X / ViewportSize.X * CanvasSize.X,
		ScreenPosition.Y / ViewportSize.Y * CanvasSize.Y);

	return CanvasPosition;
}
