#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ICTypes.generated.h"


UENUM(BlueprintType)
enum class EComboSource : uint8
{
	Ignore,
	Interrupt,
	NormalAttack UMETA(DisplayName = "Mele"), //UMETA(DisplayName = "普通攻击"),
	Input1,
	Input2,
	Input3,
	Input4,
	Input5,
	Input6,
	Input7,
};

class UInputAction;

USTRUCT(BlueprintType)
struct FAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EComboSource InputComboSource = EComboSource::Ignore;

};

/*将EComboSource封装给FInstancedStruct使用*/
USTRUCT(BlueprintType)
struct FInputComboInfo
{
	GENERATED_BODY()

public:
	FInputComboInfo():FInputComboInfo(EComboSource::Ignore){}
	FInputComboInfo(EComboSource InputComboSource):InputComboSource(InputComboSource){}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EComboSource InputComboSource;
};


/*---------------------------------------------------------------------------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

public:
	FAttackInfo(float ATKBonusCoefficient) : ATKBonusCoefficient(ATKBonusCoefficient){}
	FAttackInfo() :FAttackInfo(0.0) {}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << ATKBonusCoefficient;
		Ar << FinalDamage;

		// 简单健壮性：防 NaN / 非法值
		if (Ar.IsLoading())
		{
			if (!FMath::IsFinite(ATKBonusCoefficient))
			{
				ATKBonusCoefficient = 1.0f;
			}
		}

		bOutSuccess = true;
		return true;
	}

	float GetATKBonusCoefficient() const
	{
		return ATKBonusCoefficient;
	}
	void SetATKBonusCoefficient(float NewValue)
	{
		ATKBonusCoefficient = NewValue;
	}

	float GetFinalDamage() const
	{
		return FinalDamage;
	}
	void SetFinalDamage(float NewValue)
	{
		FinalDamage = NewValue;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ATKBonusCoefficient = 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FinalDamage = 0.0;

	//AttackDiretion;
};
template<>
struct TStructOpsTypeTraits<FAttackInfo> : public TStructOpsTypeTraitsBase2<FAttackInfo>
{
	enum { WithNetSerializer = true };
};


/*---------------------------------------------------------------------------------------------------------------------------------*/
//class UStaticMesh;
//
//USTRUCT(BlueprintType)
//struct FWeaponStruct
//{
//	GENERATED_BODY()
//
//public:
//	
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	FGameplayTag WeaponTag; // 唯一标识
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	UStaticMesh* WeaponMesh;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	FName EquipSocket = "R_Hand_Socket";
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	FName UnequipSocket = "Belt_Socket";
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	UAnimMontage* AM_EquipWeapon;
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	UAnimMontage* AM_UnequipWeapon;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	TSubclassOf<UAnimInstance> LinkAnimClass;
//};

//using FWeaponList = TMap<FGameplayTag, FWeaponStruct>;
//USTRUCT(BlueprintType)
//struct FWeaponList
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(BlueprintReadWrite)
//	TMap<FGameplayTag, FWeaponStruct> WeaponList;
//};
