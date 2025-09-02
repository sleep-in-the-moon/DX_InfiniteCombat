#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ICTypes.generated.h"


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


class UStaticMesh;
class UICGameplayAbilityBase;

USTRUCT(BlueprintType)
struct FWeaponStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WeaponMesh;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HeldSocket = "R_Hand_Socket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SecondSocket = "Belt_Socket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> LinkAnimClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AttackGATriggerTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, UAnimMontage*> TagLinkMontage;

};

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

UENUM(BlueprintType)
enum class EComboSource : uint8
{
	NormalAttack UMETA(DisplayName = "普通攻击"),
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	Skill5,
	Skill6,
	Skill7,
};
