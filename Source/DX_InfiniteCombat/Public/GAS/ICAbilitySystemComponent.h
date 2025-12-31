// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/ICTypes.h"
#include "ICAbilitySystemComponent.generated.h"

struct FInstancedStruct;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInputReleaseDelegate, const FGameplayTag&, AbilityInputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImmunityGECallback, const FGameplayTagContainer&, ImmunityGETags);
DECLARE_DELEGATE_OneParam(FInputComboPress, const FInstancedStruct& /**/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHelthChangeBPDG, float, NewHelth);

USTRUCT(BlueprintType)
struct FGAGive
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 0;
};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UICAbilitySystemComponent();
	void AbilityInputTagPressed(const FGameplayTag& InputTag, EComboSource InputComboSource);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagClick(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetFirstChildTag(FGameplayTag ParentTag) const;

	UFUNCTION(BlueprintCallable)
	void ApplySetByCallerGEToTarget(UAbilitySystemComponent* TargetASC, TSubclassOf<UGameplayEffect> ApplyGE, FGameplayTag DataTag, float Magnitude, float Level=0.0f);

	FInputComboPress DG_InputComboPress;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<FGAGive> GAArray;

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	FGameplayTag ReleasedTag;

	void ICOnImmunityBlockGameplayEffect(const FGameplayEffectSpec& Spec, const FActiveGameplayEffect* ImmunityGE);

	UPROPERTY(BlueprintAssignable)
	FImmunityGECallback ImmunityGECallback;

	UPROPERTY(BlueprintAssignable)
	FHelthChangeBPDG HelthChangeBPDG;

public:
	UPROPERTY()
	FAbilityInputReleaseDelegate AbilityInputReleaseDelegate;

private:
	void OnHelthChange(const FOnAttributeChangeData& Data);

};
