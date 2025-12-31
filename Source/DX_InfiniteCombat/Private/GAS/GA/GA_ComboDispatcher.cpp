// Copyright DX_IC


#include "GAS/GA/GA_ComboDispatcher.h"
#include "GAS/ICAbilitySystemComponent.h"

UGA_ComboDispatcher::UGA_ComboDispatcher()
{
	SetAssetTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Combo.ComboDispatcher"), false)));

	/*ActivationOwnedTags.AddTag();
	ActivationRequiredTags.AddTag();
	ActivationBlockedTags.AddTag();*/

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
}

void UGA_ComboDispatcher::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}

	UICAbilitySystemComponent* ICASC = Cast<UICAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());

}

void UGA_ComboDispatcher::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}
