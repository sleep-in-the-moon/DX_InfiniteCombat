// Copyright DX_IC


#include "GAS/GA/GA_Execution.h"
#include "ICComponents/ExecuteComponent.h"
#include "LockSystem/WeakLockComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/DefaultLevelSequenceInstanceData.h"

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
			return true;//被锁定且可被处决，TODO::添加距离判断，受击判断，地形判断；
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
	UWeakLockComponent* WeakLockComponent = AvatarActor->FindComponentByClass<UWeakLockComponent>();
	UExecuteComponent* ExecuteComponent = AvatarActor->FindComponentByClass<UExecuteComponent>();
	if (WeakLockComponent && ExecuteComponent && ExecuteComponent->ExecutionConfig.ExecuteMontageLongDistance)
	{
		BeExecutedActor = WeakLockComponent->GetLockActor();
		//MotionWarping 做实际表现，运镜用 LevelSequence，应用致死伤害。 AI 状态
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ExecuteMontage"), ExecuteComponent->ExecutionConfig.ExecuteMontageLongDistance);
		MontageTask->OnCompleted.AddUniqueDynamic(this, &UGA_Execution::MontageEnd);
		MontageTask->OnInterrupted.AddUniqueDynamic(this, &UGA_Execution::MontageEnd);
		MontageTask->Activate();

		if (ExecuteComponent->ExecutionConfig.CameraSequenceLongDistance)
		{
			FMovieSceneSequencePlaybackSettings MovieSceneSequencePlaybackSettings;
			ALevelSequenceActor* LevelSequenceActor = nullptr;
			ULevelSequencePlayer* SeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(AvatarActor->GetWorld(), ExecuteComponent->ExecutionConfig.CameraSequenceLongDistance, MovieSceneSequencePlaybackSettings, LevelSequenceActor);
			LevelSequenceActor->bOverrideInstanceData = true;
			if (UDefaultLevelSequenceInstanceData* InsData = Cast<UDefaultLevelSequenceInstanceData>(LevelSequenceActor->DefaultInstanceData))
			{
				InsData->TransformOriginActor = AvatarActor;
			}
			
			SeqPlayer->Play();
		}
		
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Execution::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Execution::MontageEnd()
{
	K2_EndAbility();
}
