// Copyright DX_IC


#include "gas/GA/ICGameplayAbilityBase.h"

//void UICGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//
//	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
//	{
//		constexpr bool bReplicateEndAbility = true;
//		constexpr bool bWasCancelled = true;
//		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//	}
//  
//  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//}
//
//void UICGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
//{
//	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

UICGameplayAbilityBase::UICGameplayAbilityBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
}

void UICGameplayAbilityBase::ReceiveArg(const FInstancedStruct& Arg)
{
	//(Arg.IsValid() && Arg.GetPtr<XX>() )
}
