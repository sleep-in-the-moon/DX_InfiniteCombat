// Copyright DX_IC


#include "GAS/ICDamageExecution.h"
#include "GameplayEffectTypes.h"
#include "CharacterAttributeSet.h"
#include "GAS/ICGameplayEffectTypes.h"


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
//#if WITH_SERVER_CODE

	FGameplayEffectSpec* OwningSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();

	const FGameplayTagContainer* SourceTags = OwningSpec->CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = OwningSpec->CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float ATK = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ATKDef, EvaluateParameters, ATK);
	float Defense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluateParameters, Defense);
	float DamageReduce = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageReduceDef, EvaluateParameters, DamageReduce);
	DamageReduce = FMath::Clamp(DamageReduce, 0.0f, 1.0f);

	//ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("SetByCaller.ATKBonusCoefficient")));
	FICGameplayEffectContext* EffectContext = static_cast<FICGameplayEffectContext*>(OwningSpec->GetContext().Get());

	FAttackInfo AttackInfo = EffectContext->GetAttackInfo();

	float FinalDamage = (ATK* AttackInfo.GetATKBonusCoefficient() - Defense) * (1 - DamageReduce);
	FinalDamage = FMath::Clamp(FinalDamage, 1.0f, FinalDamage);

	AttackInfo.SetFinalDamage(FinalDamage);
	EffectContext->SetAttackInfo(AttackInfo);

	FGameplayEffectContextHandle ContextHandle = FGameplayEffectContextHandle(EffectContext);

	OwningSpec->SetContext(ContextHandle);//更新EffectContext信息以提供给GameplayCues使用
	
	//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetHPAttribute(), EGameplayModOp::Additive, -FinalDamage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCharacterAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage));

//#endif 
}
