// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ICDamageExecution.generated.h"


/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UICDamageExecution();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
