// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttackCharacterInterface.h"
#include "AbilitySystemInterface.h"

#include "EnemyCharacterBase.generated.h"

class UICAbilitySystemComponent;
class UCombatCharacterComponent;
class UWidgetComponent;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHelthChangeBPDG, float, NewHelth);

UCLASS()
class DX_INFINITECOMBAT_API AEnemyCharacterBase : public ACharacter, public IAttackCharacterInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UICAbilitySystemComponent* ASC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IC|Combat")
	UCombatCharacterComponent* CombatComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* StateBar;
	UPROPERTY(BlueprintAssignable)
	FHelthChangeBPDG HelthChangeBPDG;

	void OnHelthChange(const FOnAttributeChangeData& Data);

private:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION()
	void ShowDebugChange(bool showDebug);
};
