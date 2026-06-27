// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ICTypes.h"

#include "CombatCharacterComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDied);

class UAnimMontage;
class UStaticMeshComponent;
class UWeaponDataAsset;

/*
	战斗角色组件，主要包含武器装备，死亡，受击动画配置等逻辑
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API UCombatCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class UGA_SwitchWeapon;

public:	
	// Sets default values for this component's properties
	UCombatCharacterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddNewWeapon(const FGameplayTag& NewWeaponTag);

	UFUNCTION(BlueprintCallable)
	void CurWeaponToUnequipSocket();
	UFUNCTION(BlueprintCallable)
	void CurWeaponToEquipSocket();

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetWeaponMeshComponent() const
	{
		return CurWeaponTag.IsValid() ? GetOwner()->FindComponentByTag<UStaticMeshComponent>(CurWeaponTag.GetTagName()) : nullptr;
	}

	UFUNCTION(BlueprintCallable)
	const UWeaponDataAsset* GetCurrentWeapon() const;

	UFUNCTION()
	void CharacterDied();
	UFUNCTION()
	void DiedEnded();

private:
	UFUNCTION(BlueprintCallable)
	bool AttachWeaponToSocket(FName socketName);

	UFUNCTION(BlueprintCallable)
	void PlayMontageBySoftPtr(TSoftObjectPtr<UAnimMontage> Montage);

	void SetToNewWeaponTag(const FGameplayTag& NewTag);

	void AddOrUpdateStaticMesh();

	void UpdateAnimLayer();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IC|Weapon")
	TSet<FGameplayTag> WeaponList;
	UPROPERTY(EditDefaultsOnly, Category = "IC|Weapon")
	FGameplayTag InitWeaponTag;

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, float> BodyPartDamageATKCoefficient;

	//TODO::改成软引用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IC|Combat|Montage")
	UAnimMontage* GetHurtMontage=nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IC|Combat|Montage")
	UAnimMontage* BlockMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RespawnTime = -1;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "IC|Combat|Montage")
	TSoftObjectPtr<UAnimMontage> Montage_Died;
	
	UPROPERTY(BlueprintAssignable)
	FCharacterDied DG_CharacterDied;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UAnimInstance> UnarmLinkAnimLayer;

private:
	FGameplayTag CurWeaponTag;
	
};
