// Copyright DX_IC


#include "CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0, GetMaxHP()));
	}

}
