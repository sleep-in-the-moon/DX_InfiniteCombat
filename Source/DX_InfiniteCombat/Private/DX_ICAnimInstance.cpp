// Copyright DX_IC


#include "DX_ICAnimInstance.h"
#include "AbilitySystemGlobals.h"

void UDX_ICAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			GameplayTagPropertyMap.Initialize(this, ASC);
		}
	}
}
