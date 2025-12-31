// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"

#include "DX_ICAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UDX_ICAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="IC|TagPorpertyBind")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	virtual void NativeInitializeAnimation() override;
};
