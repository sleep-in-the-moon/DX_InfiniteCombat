// Copyright DX_IC


#include "GAS/ICDamageExecution.h"
#include "GameplayEffectTypes.h"
#include "CharacterAttributeSet.h"


struct FDamageCalculateAttributes
{
	FGameplayEffectAttributeCaptureDefinition ATKDef;
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
	FGameplayEffectAttributeCaptureDefinition DamageReduceDef;

	FDamageCalculateAttributes()
	{
		ATKDef = FGameplayEffectAttributeCaptureDefinition(UCharacterAttributeSet::GetATKAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		DefenseDef = FGameplayEffectAttributeCaptureDefinition(UCharacterAttributeSet::GetDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		DamageReduceDef = FGameplayEffectAttributeCaptureDefinition(UCharacterAttributeSet::GetDamageReduceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	}

};

static FDamageCalculateAttributes& DamageStatics()
{
	static FDamageCalculateAttributes Statics;
	return Statics;
}

UICDamageExecution::UICDamageExecution()
{
	RelevantAttributesToCapture.Append(TArray<FGameplayEffectAttributeCaptureDefinition>({
		DamageStatics().ATKDef, DamageStatics().DefenseDef, DamageStatics().DamageReduceDef
		}));
}

void UICDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("SetByCaller.ATKBonusCoefficient")));
}
