// Copyright DX_IC


#include "GAS/GA/GA_Death.h"
#include "ICComponents/CombatCharacterComponent.h"

void UGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	if (UCombatCharacterComponent* CombatComp = GetAvatarActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>())
	{
		CombatComp->CharacterDied();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UCombatCharacterComponent* CombatComp = GetAvatarActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>())
	{
		CombatComp->DiedEnded();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
