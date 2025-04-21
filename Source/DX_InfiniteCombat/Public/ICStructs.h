// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ICStructs.generated.h"

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

