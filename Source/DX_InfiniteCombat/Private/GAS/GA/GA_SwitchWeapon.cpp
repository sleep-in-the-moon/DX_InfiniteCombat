// Copyright DX_IC


#include "GAS/GA/GA_SwitchWeapon.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_SwitchWeapon::ReceiveArg(const FInstancedStruct& Arg)
{
	if(Arg.IsValid() && Arg.GetPtr<FGameplayTag>())
		SwitchWeaponTag = Arg.Get<FGameplayTag>();
}

void UGA_SwitchWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		//return;
	}
	else
	{
		if (UCombatCharacterComponent* CombatComp = GetOwningActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>())
		{
			CombatComp->SwitchWeaponByTag(SwitchWeaponTag);

			UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("MontageTask"), CombatComp->GetHurtMontage);
			MontageTask->OnCompleted.AddUniqueDynamic(this, &UGA_SwitchWeapon::OnMontageCompleted);
		}

		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}

}

void UGA_SwitchWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_SwitchWeapon::OnMontageCompleted()
{
}

void UGA_SwitchWeapon::OnMontageBlendOut()
{
}

void UGA_SwitchWeapon::OnMontageInterrupted()
{
}

void UGA_SwitchWeapon::OnMontageCancelled()
{
}
