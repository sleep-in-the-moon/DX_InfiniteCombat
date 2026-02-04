// Copyright DX_IC


#include "GAS/GA/GA_SwitchWeapon.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Data/WeaponDataAsset.h"
#include "Data/ICAssetManager.h"
#include "AbilitySystemComponent.h"


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
		return;
	}
	
	FInstancedStruct Arg = ConsumArg();
	if (!Arg.IsValid() || !Arg.GetPtr<FGameplayTag>())
	{
		K2_EndAbility();
		return;
	}

	SwitchWeaponTag = Arg.Get<FGameplayTag>();

	CombatComp = GetOwningActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>();
	if (!CombatComp || !CombatComp->WeaponList.Contains(SwitchWeaponTag) || CombatComp->CurWeaponTag == SwitchWeaponTag)
	{
		K2_EndAbility();
		return;
	}

	// CurWeaponValid ? UnequipCur : None  Comleted->  NewWeaponValid ? EquipNew : unarm
	if (CombatComp->CurWeaponTag.IsValid() && CombatComp->GetCurrentWeapon())
	{
		//Unequip
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("UnequipMontage"), UICAssetManager::GetAssetBySoftPtr(CombatComp->GetCurrentWeapon()->AM_UnequipWeapon, false));
		MontageTask->OnCompleted.AddUniqueDynamic(this, &UGA_SwitchWeapon::EquipNewWeapon);
		MontageTask->Activate();
	}
	else
		EquipNewWeapon();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UGA_SwitchWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_SwitchWeapon::EquipNewWeapon()
{
	//CombatComp->CurWeaponToUnequipSocket();//animNotify

	CombatComp->SetToNewWeaponTag(SwitchWeaponTag);

	// LinkAnimLayer
	CombatComp->UpdateAnimLayer();

	if (CombatComp->GetCurrentWeapon())
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("EquipMontage"), UICAssetManager::GetAssetBySoftPtr(CombatComp->GetCurrentWeapon()->AM_EquipWeapon, false));
		MontageTask->OnCompleted.AddUniqueDynamic(this, &UGA_SwitchWeapon::K2_EndAbility);
		MontageTask->Activate();

		//CombatComp->CurWeaponToEquipSocket();//animNotify
	}
	else
		K2_EndAbility();
	
}

void UGA_SwitchWeapon::OnMontageBlendOut()
{
}

void UGA_SwitchWeapon::OnMontageInterrupted()
{
	K2_EndAbility();
}

void UGA_SwitchWeapon::OnMontageCancelled()
{
}
