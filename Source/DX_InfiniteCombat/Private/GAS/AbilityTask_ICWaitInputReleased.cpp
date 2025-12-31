// Copyright DX_IC


#include "AbilityTask_ICWaitInputReleased.h"
#include "ICAbilitySystemComponent.h"

void UAbilityTask_ICWaitInputReleased::OnReleaseCallback(const FGameplayTag& Tag)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("OnReleaseCallback")));
	if (Timer.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(Timer);
	if (!Ability)
	{
		return;
	}

	if (AbilityTags.HasTag(Tag))
	{
		float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnReleased.Broadcast(ElapsedTime);
		}
		EndTask();
	}
	
}

void UAbilityTask_ICWaitInputReleased::Activate()
{
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Activate")));
	StartTime = GetWorld()->GetTimeSeconds();

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UAbilityTask_ICWaitInputReleased::CallFixTime, FixTime, false,1);

	UICAbilitySystemComponent* ASC = Cast<UICAbilitySystemComponent>(AbilitySystemComponent.Get());
	if (ASC && Ability)
	{
		//if (IsLocallyControlled())
		{
			/*UE_LOG(LogTemp, Warning, TEXT("Spec->InputPressed::%d"), Spec->InputPressed);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Spec->InputPressed::%d"), Spec->InputPressed));*/
			ASC->AbilityInputReleaseDelegate.AddDynamic(this, &UAbilityTask_ICWaitInputReleased::OnReleaseCallback);
		}
	
	}
}

void UAbilityTask_ICWaitInputReleased::OnDestroy(bool bInOwnerFinished)
{
	//bInOwnerFinished 表示是否是因为 Ability 正常结束而销毁任务,这里不用
	UICAbilitySystemComponent* ASC = Cast<UICAbilitySystemComponent>(AbilitySystemComponent.Get());
	if (ASC)
	{
		ASC->AbilityInputReleaseDelegate.RemoveDynamic(this, &UAbilityTask_ICWaitInputReleased::OnReleaseCallback);
	}
	if (Timer.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(Timer);
}

UAbilityTask_ICWaitInputReleased* UAbilityTask_ICWaitInputReleased::ICWaitInputReleased(UGameplayAbility* OwningAbility, float FixTime)
{
	UAbilityTask_ICWaitInputReleased* Task = NewAbilityTask<UAbilityTask_ICWaitInputReleased>(OwningAbility);
	Task->AbilityTags = OwningAbility->AbilityTags;
	Task->FixTime = FixTime;
	return Task;
}

void UAbilityTask_ICWaitInputReleased::CallFixTime()
{
	FixTimeEvent.Broadcast();
}
