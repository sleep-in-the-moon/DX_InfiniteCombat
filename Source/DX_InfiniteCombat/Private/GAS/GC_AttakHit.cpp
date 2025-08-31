// Copyright DX_IC


#include "GAS/GC_AttakHit.h"
#include "GAS/ICGameplayEffectTypes.h"
#include "UMG/WidgetCombatStates.h"
#include "Kismet/GameplayStatics.h"

UGC_AttakHit::UGC_AttakHit(const FObjectInitializer& ObjectInit): Super(ObjectInit)//CDO
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.AttackHit"));
}

bool UGC_AttakHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const FICGameplayEffectContext* EffectContext = static_cast<const FICGameplayEffectContext*>(Parameters.EffectContext.Get());

	if (!EffectContext)
		return false;

	//Parameters.GetSourceObject();
	if(!EffectContext->GetSourceObject())
		return false;

	if (APawn* SourcePawn = Cast<APawn>(EffectContext->GetSourceObject()))
	{
		APlayerController* playerControl = Cast<APlayerController>(SourcePawn->GetController());
		if (playerControl)
		{
			FProperty* CombatStatesWidgetPro = playerControl->GetClass()->FindPropertyByName(TEXT("CombatStatesWidget"));
			if (CombatStatesWidgetPro)
			{

				FClassProperty* CombatStatesWidgetClassPro = static_cast<FClassProperty*>(CombatStatesWidgetPro);
				const void* ValuePtr = CombatStatesWidgetClassPro->ContainerPtrToValuePtr<void>(playerControl);
				TObjectPtr<UObject> CombatStatesWidgetObj = CombatStatesWidgetClassPro->GetPropertyValue(ValuePtr);

				if (UWidgetCombatStates* CombatStatesWidget = Cast<UWidgetCombatStates>(CombatStatesWidgetObj))
					CombatStatesWidget->SpawnTransientWidgetByActor(MyTarget, WidgetClass, 1.4, FString::FromInt(FMath::Floor(EffectContext->GetAttackInfo().GetFinalDamage())));
			}
		}
	}

	//EffectContext->GetAttackInfo().GetFinalDamage();

	if(HitSound)
		UGameplayStatics::PlaySoundAtLocation(MyTarget->GetWorld(), HitSound, MyTarget->GetActorLocation(), MyTarget->GetActorRotation(), 3.0f);

	return true;
}
