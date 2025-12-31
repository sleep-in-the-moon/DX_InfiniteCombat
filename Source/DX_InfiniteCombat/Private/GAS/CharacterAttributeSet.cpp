// Copyright DX_IC


#include "GAS/CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		SetHP(FMath::Clamp(GetHP() - GetIncomingDamage(), 0.0f, GetMaxHP()));
		SetIncomingDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetATKAttribute())
	{
		SetATK(FMath::Clamp(GetATK(), 0.0f, GetATK()));
	}
	else if (Data.EvaluatedData.Attribute == GetDefenseAttribute())
	{
		SetDefense(FMath::Clamp(GetDefense(), 0.0f, GetDefense()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageReduceAttribute())
	{
		SetDamageReduce(FMath::Clamp(GetDamageReduce(), 0.0f, 1.0f));
	}
}
