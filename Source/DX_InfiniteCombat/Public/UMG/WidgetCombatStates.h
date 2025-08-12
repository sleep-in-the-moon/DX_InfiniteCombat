// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void SpawnTransientWidgetByWorldLoc(const FVector& WorldLoc, float LifeTime, TSubclassOf<UUserWidget> WidgetClass);

protected:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvasPanel;

};
