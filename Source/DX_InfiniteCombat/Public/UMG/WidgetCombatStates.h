// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DX_ReusableTool/Public/DataStructure/DX_ObjectPool.h"
#include "WidgetCombatStates.generated.h"


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
	void SpawnTransientWidgetByActor(AActor* AttachActor, TSubclassOf<UUserWidget> WidgetClass, float TransientTime = 2.0f, const FString& ShowInfos="");

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
};
