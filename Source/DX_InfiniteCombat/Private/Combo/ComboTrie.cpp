// Copyright DX_IC


#include "Combo/ComboTrie.h"
#include "AbilitySystemComponent.h"


void UComboTrie::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentCombo.IsEmpty())
		return;

	CurrentDuration += DeltaTime;
	if (CurrentDuration >= ComboDuration)
	{
		EndCombo();
	}
}

bool UComboTrie::TryActiveCombo(EComboSource ComboSource)
{
	if (ComboSource == EComboSource::Ignore)
		return true;
	if (ComboSource == EComboSource::Interrupt)
	{
		EndCombo();
		return false;
	}

	CurrentCombo.Add(ComboSource);
	if (ComboTrie.Search(CurrentCombo))
	{
		CurrentDuration = 0.0f;

		if (GetOwner())
		{
			UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
			if (ASC && ComboStateTag.IsValid() && ComboDispatcherGATag.IsValid())
			{
				if(!ASC->HasMatchingGameplayTag(ComboStateTag))
					ASC->AddLooseGameplayTag(ComboStateTag);

				return ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(ComboDispatcherGATag));
			}
		}
	}
	else
	{
		EndCombo();
	}

	return false;
}

void UComboTrie::EndCombo()
{
	CurrentCombo.Empty();
	CurrentDuration = 0.0f;
	if (GetOwner())
	{
		UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC && ComboStateTag.IsValid())
		{
			ASC->RemoveLooseGameplayTag(ComboStateTag);
		}
	}
}

TArray<FText> UComboTrie::GetCurrentComboText() const
{
	TArray<FText> Res;
	FText ComboText;

	UEnum* ComboSourceUEnum = StaticEnum<EComboSource>();
	for(EComboSource ComboSource: CurrentCombo)
	{
		if (ComboSourceUEnum->FindDisplayNameTextByValue(ComboText, static_cast<int64>(ComboSource)))//优先获取DisplayName
			Res.Add(ComboText);
		else
			Res.Add(FText::FromString(ComboSourceUEnum->GetNameStringByValue(static_cast<int64>(ComboSource))));
	}
	return Res;
}

void UComboTrie::InitialComboTrie(const TArray<TArray<EComboSource> >& TotalCombo)
{
	for (const TArray<EComboSource>& comboBranch : TotalCombo)
	{
		ComboTrie.Insert(comboBranch);
	}
}
