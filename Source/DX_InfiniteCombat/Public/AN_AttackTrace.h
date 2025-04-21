// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Engine/EngineTypes.h"

#include "AN_AttackTrace.generated.h"

class UStaticMeshComponent;
class UICAbilitySystemComponent;
class UGameplayEffect;
struct FCollisionQueryParams;
struct FCollisionObjectQueryParams;
//struct FGameplayEffectContextHandle;

USTRUCT(BlueprintType)
struct FGEApplyArg
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Level=0;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectToTargets;

};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UAN_AttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	/*UPROPERTY(EditAnywhere, Category = "AttackTrace")
	int8 TracePointNum = 5;*/
	
	UPROPERTY(EditAnywhere, Category = "IC|AttackTrace")
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes = { TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECC_Pawn)) };
	UPROPERTY(EditAnywhere, Category = "IC|AttackTrace")
	bool bTraceComplex=false;
	UPROPERTY(EditAnywhere, Category = "IC|AttackTrace")
	bool bIgnoreSelf=true;
	UPROPERTY(EditAnywhere, Category = "IC|AttackTrace", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "IC|HitApply|Effect")
	TArray<FGEApplyArg> GameplayEffectsToApply;
	/*UPROPERTY(EditAnywhere, Category = "HitApply|Effect")
	FGameplayEffectContextHandle GEContextHandle;
	UPROPERTY(EditAnywhere, Category = "HitApply|Effect")*/

private:
	
	TArray<FName> AttackTraceSockets;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> CurWeapon;
	TMap<FName, FVector> PreSocketLoc;

	FCollisionQueryParams Params;
	FCollisionObjectQueryParams ObjectQueryParams;
	UPROPERTY()
	TObjectPtr<UICAbilitySystemComponent> OwnerASC;
	UPROPERTY()
	TArray<AActor*> ApplyedObjs;

};
