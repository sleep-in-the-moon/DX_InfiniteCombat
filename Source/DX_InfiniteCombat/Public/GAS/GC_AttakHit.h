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
	
public:
	UGC_AttakHit(const FObjectInitializer& ObjectInit);

protected:
	//bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetClass;//Temp
	UPROPERTY(EditDefaultsOnly)
	USoundBase* HitSound;

};
