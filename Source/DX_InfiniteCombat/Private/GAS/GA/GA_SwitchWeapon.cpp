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
		//return;
	}
	else
	{
		if (UCombatCharacterComponent* CombatComp = GetOwningActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>())
		{
			CombatComp->SwitchWeaponByTag(SwitchWeaponTag);

			if (SwitchWeaponTag.IsValid() && CombatComp->WeaponList.Contains(SwitchWeaponTag))
			{
				if (CombatComp->CurWeaponTag.IsValid() && CombatComp->GetCurrentWeapon())
				{
					UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("MontageTask"), UICAssetManager::GetAssetBySoftPtr(CombatComp->GetCurrentWeapon()->AM_UnequipWeapon, false));

					MontageTask->OnCompleted.AddUniqueDynamic(this, &UGA_SwitchWeapon::OnMontageCompleted);
					
				}
			}
			else if (!SwitchWeaponTag.IsValid())
			{

			}
			
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
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	UCombatCharacterComponent* CombatComp = GetOwningActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>();
	if (ASC && CombatComp)
	{
		if (CombatComp->GetCurrentWeapon())
			CombatComp->AttachWeaponToSocket(CombatComp->GetCurrentWeapon()->UnequipSocket);

		if (CombatComp->CurWeaponTag.IsValid())
			ASC->RemoveLooseGameplayTag(CombatComp->CurWeaponTag);
		ASC->AddLooseGameplayTag(SwitchWeaponTag);
	}
	CombatComp->CurWeaponTag = *CombatComp->WeaponList.Find(SwitchWeaponTag);

	if (const UWeaponDataAsset* Asset = CombatComp->GetCurrentWeapon())
	{
		CombatComp->PlayMontageBySoftPtr(Asset->AM_EquipWeapon);
	}
	if (CombatComp->GetCurrentWeapon())
		CombatComp->AttachWeaponToSocket(CombatComp->GetCurrentWeapon()->EquipSocket);

	// ChangeStaticMesh
	if (!CombatComp->GetWeaponMeshComponent())
	{
		GetOwningActorFromActorInfo()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, false)->ComponentTags.Add(TEXT("Weapon"));
		CombatComp->GetWeaponMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (CombatComp->GetCurrentWeapon())
		CombatComp->GetWeaponMeshComponent()->SetStaticMesh(UICAssetManager::GetAssetBySoftPtr(CombatComp->GetCurrentWeapon()->WeaponMesh));

	// LinkAnimLayer
	if (USkeletalMeshComponent* BodyMesh = GetOwningActorFromActorInfo()->FindComponentByClass<USkeletalMeshComponent>())
	{
		UAnimInstance* OwnerAnimIns = BodyMesh->GetAnimInstance();
		if (OwnerAnimIns && CombatComp->GetCurrentWeapon() && UICAssetManager::GetSubclassBySoftPtr(CombatComp->GetCurrentWeapon()->LinkAnimClass))
		{
			OwnerAnimIns->LinkAnimClassLayers(UICAssetManager::GetSubclassBySoftPtr(CombatComp->GetCurrentWeapon()->LinkAnimClass));
		}
	}

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
