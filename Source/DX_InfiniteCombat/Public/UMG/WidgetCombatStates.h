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
	void SpawnTransientWidgetByWorldLoc(const FVector& WorldLoc, TSubclassOf<UUserWidget> WidgetClass, float LifeTime = 1.0f, const FString& ShowInfos="");

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvasPanel;

private:
	TUniquePtr<DX_ObjectPool<UUserWidget> > WidgetPool;
	FTimerHandle LifeTimer;
};
