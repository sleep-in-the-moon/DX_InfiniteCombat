// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_AttakHit.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UGC_AttakHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
protected:
	bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

};
