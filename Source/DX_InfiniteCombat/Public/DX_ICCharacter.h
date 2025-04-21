// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "BasicCharacter.h"
#include "AbilitySystemInterface.h"
#include "AttackCharacterInterface.h"

#include "DX_ICCharacter.generated.h"

class UAbilitySystemComponent;
class UICAbilitySystemComponent;
class UGameplayAbility;
class UStaticMeshComponent;
struct FGameplayTag;
class UCombatCharacterComponent;

//UENUM()
//enum class EExecNodePin : uint8z
//{
//	Success,
//	Fail
//};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API ADX_ICCharacter : public ABasicCharacter, public IAbilitySystemInterface, public IAttackCharacterInterface
{
	GENERATED_BODY()

public:
	ADX_ICCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (ExpandEnumAsExecs = "Result"))
	//void LerpActorToTargetRot(EExecNodePin& Result, const FRotator& TargetRot);

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	void Input_AbilityInputTagClick(FGameplayTag InputTag);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IC|GA")
	TObjectPtr<UICAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCombatCharacterComponent* CombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMeshComponent;

private:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION()
	void ShowDebugChange(bool showDebug);
};
