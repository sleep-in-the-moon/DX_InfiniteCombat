// Copyright DX_IC


#include "GAS/GA/GA_Block.h"
#include "GAS/AbilityTask_ICWaitInputReleased.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "GameFramework/Character.h"

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	AActor* const AvatarActor = ActorInfo->AvatarActor.Get();
	UCombatCharacterComponent* CombatComp = AvatarActor->FindComponentByClass<UCombatCharacterComponent>();
	if (!CombatComp || !CombatComp->BlockMontages.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("BlockMontage"), CombatComp->BlockMontages.BlockStartMontage);
	CurBlockMontage = CombatComp->BlockMontages.BlockStartMontage;
	MontageTask->OnCompleted.AddUniqueDynamic(this, &UGA_Block::BlockStartMontageEnd);
	MontageTask->Activate();

	UAbilityTask_ICWaitInputReleased* InputReleasedTask = UAbilityTask_ICWaitInputReleased::ICWaitInputReleased(this);
	InputReleasedTask->OnReleased.AddUniqueDynamic(this, &UGA_Block::BlockReleased);
	InputReleasedTask->Activate();
	
	//TODO::¼õËÙ

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Block::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Block::BlockReleased(float PressTime)
{
	AActor* const AvatarActor = GetAvatarActorFromActorInfo();
	ACharacter* character = Cast<ACharacter>(AvatarActor);
	if (!character)
	{
		K2_EndAbility();
		return;
	}

	character->StopAnimMontage(CurBlockMontage);
	if (UCombatCharacterComponent* CombatComp = GetAvatarActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>())
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("BlockMontage"), CombatComp->BlockMontages.BlockEndMontage, 1.0f, NAME_None, false);
		MontageTask->Activate();
	}

	K2_EndAbility();
}

void UGA_Block::BlockStartMontageEnd()
{
	if (UCombatCharacterComponent* CombatComp = GetAvatarActorFromActorInfo()->FindComponentByClass<UCombatCharacterComponent>())
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("BlockMontage"), CombatComp->BlockMontages.BlockLoopMontage);
		CurBlockMontage = CombatComp->BlockMontages.BlockLoopMontage;
		MontageTask->Activate();
	}
}

bool UGA_Block::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	//TODO::ÎäÆ÷ÏÞÖÆ
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
