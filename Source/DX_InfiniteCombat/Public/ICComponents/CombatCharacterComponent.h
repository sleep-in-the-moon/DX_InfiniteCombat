// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ICTypes.h"
#include "CombatCharacterComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterDied);

class UAnimMontage;
class UStaticMeshComponent;
//struct FWeaponList

UENUM(BlueprintType)
enum class ETraceShapeType : uint8
{
	Sphere,
	Capsule,
	Box,
	Line,
	Cone
};

USTRUCT(BlueprintType)
struct FCombatStateSwitchMontages
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_HeldToSecond;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_SecondToHeld;
};

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

	UFUNCTION(BlueprintCallable, Category = "IC|Weapon")
	UStaticMeshComponent* GetWeaponMeshComponent() const
	{
		return GetOwner()->FindComponentByTag<UStaticMeshComponent>(TEXT("Weapon"));
	}
	UFUNCTION(BlueprintCallable, Category = "IC|Weapon")
	FWeaponStruct GetCurWeapon() const
	{
		return CurWeapon;
	}
	UFUNCTION(BlueprintCallable, Category = "IC|Weapon")
	bool SwitchWeaponByTag(FGameplayTag Tag);
	UFUNCTION(BlueprintCallable, Category = "IC|Weapon")
	bool WeaponToHeld();
	UFUNCTION(BlueprintCallable, Category = "IC|Weapon")
	bool WeaponToSecond();
	UFUNCTION(BlueprintCallable, Category = "IC|Weapon")
	bool AttachWeaponToSocket(FName socketName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IC|StateSwitch")
	FCombatStateSwitchMontages CombatStateSwitchMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IC|Weapon")
	TMap<FGameplayTag, FWeaponStruct> WeaponList;
	UPROPERTY(EditDefaultsOnly, Category = "IC|Weapon")
	FGameplayTag InitWeaponTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Unarmed"), false);

	UFUNCTION()
	void CharacterDied();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IC|Combat|Montage")
	UAnimMontage* GetHurtMontage;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "IC|Combat|Montage")
	UAnimMontage* DiedMontage;
	
	UPROPERTY(BlueprintAssignable)
	FCharacterDied DG_CharacterDied;

private:
	FWeaponStruct CurWeapon;
	
};
