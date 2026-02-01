// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ICTypes.h"
#include "StructUtils/InstancedStruct.h"

#include "CombatCharacterComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDied);

class UAnimMontage;
class UStaticMeshComponent;
class UWeaponDataAsset;

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
	const UWeaponDataAsset* GetCurrentWeapon() const;

private:
	UFUNCTION(BlueprintCallable)
	void EquipWeapon();
	UFUNCTION(BlueprintCallable)
	void UnequipWeapon();
	UFUNCTION(BlueprintCallable)
	bool AttachWeaponToSocket(FName socketName);

	UFUNCTION(BlueprintCallable)
	void PlayMontageBySoftPtr(TSoftObjectPtr<UAnimMontage> Montage);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IC|Weapon")
	TSet<FGameplayTag> WeaponList;
	UPROPERTY(EditDefaultsOnly, Category = "IC|Weapon")
	FGameplayTag InitWeaponTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IC|Combat|Montage")
	UAnimMontage* GetHurtMontage=nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "IC|Combat|Montage")
	TSoftObjectPtr<UAnimMontage> Montage_Died;

	UPROPERTY(EditDefaultsOnly, Category = "IC|Combat|Montage")
	FInstancedStruct Test;
	
	UPROPERTY(BlueprintAssignable)
	FCharacterDied DG_CharacterDied;

private:
	FGameplayTag CurWeaponTag;
	
};
