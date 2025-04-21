// Copyright DX_IC


#include "ICAbilitySystemComponent.h"
#include "ICGameplayAbilityBase.h"
#include "GameplayTagsManager.h"

UICAbilitySystemComponent::UICAbilitySystemComponent()
{
	ClearAbilityInput();
}

void UICAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.Ability->AbilityTags.HasTag(InputTag))	//AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)??
			{
				//TryActivateAbility(AbilitySpec.Handle);
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
				/*UE_LOG(LogTemp, Warning, TEXT("AbilityInputTagPressed"));
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AbilityInputTagPressed"));*/
			}
		}
	}
}

void UICAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.Ability->AbilityTags.HasTag(InputTag))
			{
				ReleasedTag = InputTag;
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);

				UE_LOG(LogTemp, Warning, TEXT("AbilityInputTagReleased"));
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AbilityInputTagReleased"));
			}
		}
	}
}

void UICAbilitySystemComponent::AbilityInputTagClick(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && AbilitySpec.Ability->AbilityTags.HasTag(InputTag))	//AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)??
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UICAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& SpecHandel : InputHeldSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandel))
		{
			//if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			if (AbilitySpec->Ability)
			{
				UICGameplayAbilityBase* ICAbility = Cast<UICGameplayAbilityBase>(AbilitySpec->Ability);
				if (ICAbility && ICAbility->GetAbilityInputTriggerMode() == EAbilityInputTriggerMode::OnHeldActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					/*UE_LOG(LogTemp, Warning, TEXT("OnHeldActive"));
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("OnHeldActive"));*/
				}
				
			}
		}

	}
	for (const FGameplayAbilitySpecHandle& SpecHandel : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandel))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;  //??

				//if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);  //??
				}
				//else
				{
					UICGameplayAbilityBase* ICAbility = Cast<UICGameplayAbilityBase>(AbilitySpec->Ability);
					if (ICAbility && ICAbility->GetAbilityInputTriggerMode() == EAbilityInputTriggerMode::OnClickOnce)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}

			}
		}

	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		/*UE_LOG(LogTemp, Warning, TEXT("InActiveAbility"));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("InActiveAbility"));*/
		TryActivateAbility(AbilitySpecHandle);
	}

	for (const FGameplayAbilitySpecHandle& SpecHandel : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandel))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				AbilityInputReleaseDelegate.Broadcast(ReleasedTag);

				//if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}

			}
		}

	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();

}

void UICAbilitySystemComponent::ClearAbilityInput()
{
	InputReleasedSpecHandles.Reset();
	InputPressedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UICAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const FGAGive& FGA : GAArray)
	{
		if (FGA.GA)
			GiveAbility(FGameplayAbilitySpec(FGA.GA.GetDefaultObject(), FGA.Level));
	}
	InitAbilityActorInfo(GetOwner(), GetOwner());

	OnImmunityBlockGameplayEffectDelegate.AddUObject(this, &UICAbilitySystemComponent::ICOnImmunityBlockGameplayEffect);
}

FGameplayTag UICAbilitySystemComponent::GetFirstChildTag(FGameplayTag ParentTag) const
{
	TArray<FGameplayTag> ChildTags;
	ChildTags.Empty();
	ChildTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(ParentTag).GetGameplayTagArray();
	for (FGameplayTag& tag : ChildTags)
	{
		if (HasMatchingGameplayTag(tag))
			return tag;
	}

	return FGameplayTag::EmptyTag;
}

void UICAbilitySystemComponent::ApplySetByCallerGEToTarget(UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> ApplyGE, FGameplayTag DataTag, float Magnitude, float Level)
{
	if (!TargetASC || !ApplyGE)
		return;

	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(ApplyGE, Level, TargetASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(DataTag, Magnitude);
		ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}

}

void UICAbilitySystemComponent::ICOnImmunityBlockGameplayEffect(const FGameplayEffectSpec& Spec, const FActiveGameplayEffect* ImmunityGE)
{
	FGameplayTagContainer OutContainer;
	ImmunityGE->Spec.GetAllAssetTags(OutContainer);
	
	ImmunityGECallback.Broadcast(OutContainer);
}
