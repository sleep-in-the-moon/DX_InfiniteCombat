#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"


class UGameplayEffect;
struct FGameplayEffectSpecHandle;

namespace AttackUtils
{
	FGameplayEffectSpecHandle MakeAttackGESpecHandle(AActor* SourceActor, TSubclassOf<UGameplayEffect> GEClass, const FHitResult& HitRes, float DamageATKCoefficient);
}
