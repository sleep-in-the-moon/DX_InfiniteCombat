// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ICTypes.h"

#include "CombatCharacterComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDied);

class UAnimMontage;
class UStaticMeshComponent;

/*
	战斗角色组件，主要包含武器装备，死亡，受击动画等逻辑
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API UCombatCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatCharacterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(BlueprintCallable)
	bool SwitchWeaponByTag(FGameplayTag Tag);

	UFUNCTION()
	void CharacterDied();

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetWeaponMeshComponent() const
	{
		return GetOwner()->FindComponentByTag<UStaticMeshComponent>(TEXT("Weapon"));
	}

	UFUNCTION(BlueprintCallable)
	FWeaponStruct GetCurrentWeapon() const
	{
		return CurWeapon;
	}

//private:
	UFUNCTION(BlueprintCallable)
	bool EquipWeapon();
	UFUNCTION(BlueprintCallable)
	bool UnequipWeapon();
	UFUNCTION(BlueprintCallable)
	bool AttachWeaponToSocket(FName socketName);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IC|Weapon")
	TMap<FGameplayTag, FWeaponStruct> WeaponList;
	UPROPERTY(EditDefaultsOnly, Category = "IC|Weapon")
	FGameplayTag InitWeaponTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IC|Combat|Montage")
	UAnimMontage* GetHurtMontage= nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "IC|Combat|Montage")
	UAnimMontage* DiedMontage=nullptr;
	
	UPROPERTY(BlueprintAssignable)
	FCharacterDied DG_CharacterDied;

private:
	FWeaponStruct CurWeapon;
	
};
