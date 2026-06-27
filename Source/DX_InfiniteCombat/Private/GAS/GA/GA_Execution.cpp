// Copyright DX_IC


#include "GAS/GA/GA_Execution.h"
#include "ICComponents/ExecuteComponent.h"
#include "LockSystem/WeakLockComponent.h"

bool UGA_Execution::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	AActor* const AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	if (AvatarActor == nullptr || !ShouldActivateAbility(AvatarActor->GetLocalRole()))
	{
		return false;
	}

	UWeakLockComponent* WeakLockComponent = AvatarActor->FindComponentByClass<UWeakLockComponent>();
	if (WeakLockComponent && WeakLockComponent->IsLockOn())
	{
		UExecuteComponent* ExecuteComponent = WeakLockComponent->GetLockActor()->FindComponentByClass<UExecuteComponent>();
		if (ExecuteComponent && ExecuteComponent->bCanBeExecuted)
		{
			return true;//被锁定且可被处决，TODO::添加距离判断，受击判断
		}
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Execution::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	
	AActor* const AvatarActor = ActorInfo->AvatarActor.Get();
	if (UWeakLockComponent* WeakLockComponent = AvatarActor->FindComponentByClass<UWeakLockComponent>())
	{
		BeExecutedActor = WeakLockComponent->GetLockActor();
		//播放LevelSeq，应用致死伤害。地形，碰撞如何处理？
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Execution::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
