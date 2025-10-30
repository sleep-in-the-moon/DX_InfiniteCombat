#include "Utils/IC_Utils.h"
#include "GameplayEffect.h"
#include "GAS/ICGameplayEffectTypes.h"
#include "AbilitySystemComponent.h"


FGameplayEffectSpecHandle AttackUtils::MakeAttackGESpecHandle(AActor* SourceActor, TSubclassOf<UGameplayEffect> GEClass, const FHitResult& HitRes, float DamageATKCoefficient)
{
	if (!SourceActor || !GEClass)
		return FGameplayEffectSpecHandle();

	if (UAbilitySystemComponent* ASC = SourceActor->FindComponentByClass<UAbilitySystemComponent>())
	{
		FAttackInfo AttackInfo(DamageATKCoefficient);

		FICGameplayEffectContext* ICEffectContext = new FICGameplayEffectContext(AttackInfo);
		ICEffectContext->AddInstigator(SourceActor, SourceActor);
		ICEffectContext->AddSourceObject(SourceActor);
		ICEffectContext->AddHitResult(HitRes);

		FGameplayEffectContextHandle ContextHandle = FGameplayEffectContextHandle(ICEffectContext);
		FGameplayEffectSpecHandle GESpecHandle = ASC->MakeOutgoingSpec(GEClass, 0, ContextHandle);
		return GESpecHandle;
	}

	return FGameplayEffectSpecHandle();
}
