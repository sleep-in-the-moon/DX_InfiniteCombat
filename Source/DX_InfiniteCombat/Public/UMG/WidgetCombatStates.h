// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DX_ReusableTool/Public/DataStructure/DX_ObjectPool.h"
#include "WidgetCombatStates.generated.h"


UENUM()
enum class EWidgetAttachMode : uint8
{
	AttachToLocation,
	AttachToActor
};

USTRUCT()
struct FPersistentWidgetHandle 
{
	GENERATED_BODY()

	TWeakObjectPtr<UUserWidget> PersistentWidget;
	EWidgetAttachMode WidgetAttachMode = EWidgetAttachMode::AttachToLocation;
	AActor* AttachedActor = nullptr;
	FVector AttachedLocation = FVector::ZeroVector;

	FPersistentWidgetHandle() {};
	FPersistentWidgetHandle(UUserWidget* PersistentWidget, AActor* AttachActor, EWidgetAttachMode WidgetAttachMode = EWidgetAttachMode::AttachToActor) 
		:PersistentWidget(PersistentWidget), AttachedActor(AttachActor), WidgetAttachMode(WidgetAttachMode)
	{}

	FPersistentWidgetHandle(UUserWidget* PersistentWidget, FVector AttachToLocation, EWidgetAttachMode WidgetAttachMode = EWidgetAttachMode::AttachToLocation) 
		:PersistentWidget(PersistentWidget), AttachedLocation(AttachToLocation), WidgetAttachMode(WidgetAttachMode)
	{}
};

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UWidgetCombatStates : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SpawnTransientWidgetByActor(AActor* AttachActor, TSubclassOf<UUserWidget> WidgetClass, float TransientTime = 1.4f, const FString& ShowInfos="");

	//UFUNCTION(BlueprintCallable)
	void RegisterPersistentWidget(FName UniqueID, FPersistentWidgetHandle RegisteredWidgetHandle);
	void UnRegisterPersistentWidget(FName UniqueID);

	/*UFUNCTION(BlueprintCallable)
	void UpdatePersistentWidget(FName UniqueID, );*/

protected:
	//virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	FVector2D GetCanvasPositionByWorldLoc(const FVector& WorldLoc);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvasPanel;

private:
	TUniquePtr<DX_ObjectPool<UUserWidget> > WidgetPool;
	TWeakObjectPtr<AActor> TransientWidgetAttachActor;

	UPROPERTY()
	TMap<FName, FPersistentWidgetHandle> PersistentWidgets;
};
