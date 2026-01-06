// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ContinuousTriggerAction.generated.h"


/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UContinuousTriggerAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Async")
	static UContinuousTriggerAction* ContinuousTriggerAction(bool& Stop);

	virtual void Activate() override;

private:
	bool End;
};
