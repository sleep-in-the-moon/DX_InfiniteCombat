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
struct FPersistentWidgetInfos
{
	GENERATED_BODY()

	TWeakObjectPtr<UUserWidget> PersistentWidget = nullptr;
	EWidgetAttachMode WidgetAttachMode = EWidgetAttachMode::AttachToLocation;
	AActor* AttachedActor = nullptr;
	FVector AttachedLocation = FVector::ZeroVector;
	FVector2D Alignment = FVector2D(0.5, 0.5);
	int32 ZOrder = 0;

	FPersistentWidgetInfos(): FPersistentWidgetInfos(nullptr, FVector::ZeroVector) {}

	FPersistentWidgetInfos(UUserWidget* PersistentWidget, AActor* AttachActor, const FVector2D& Alignment = FVector2D(0.5, 0.5), int32 ZOrder = 0)
		:PersistentWidget(PersistentWidget), AttachedActor(AttachActor), Alignment(Alignment), ZOrder(ZOrder)
		, WidgetAttachMode(EWidgetAttachMode::AttachToActor)
	{}

	FPersistentWidgetInfos(UUserWidget* PersistentWidget, const FVector& AttachToLocation, const FVector2D& Alignment = FVector2D(0.5, 0.5), int32 ZOrder = 0)
		:PersistentWidget(PersistentWidget), AttachedLocation(AttachToLocation), Alignment(Alignment), ZOrder(ZOrder)
		, WidgetAttachMode(EWidgetAttachMode::AttachToLocation)
	{}
};

class UCanvasPanel;
class UCapsuleComponent;
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
	void RegisterPersistentWidget(FName UniqueID, const FPersistentWidgetInfos& PersistentWidgetInfos);
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
	TWeakObjectPtr<AActor> TransientWidgetAttachActor = nullptr;
	TWeakObjectPtr<UCapsuleComponent> AttachActorCapsuleCompo=nullptr;

	UPROPERTY()
	TMap<FName, FPersistentWidgetInfos> PersistentWidgets;
};
