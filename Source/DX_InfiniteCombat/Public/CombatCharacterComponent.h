// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ICStructs.h"
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
	UAnimMontage* AM_OutToInCombat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_InToOutCombat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_InCombatToSpecial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_SpecialToIncombat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_SpecialToOutCombat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_OutCombatToSpecial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_MainWeaponToSecond;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AM_SecondWeaponToMain;
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

	UFUNCTION(BlueprintCallable, Category = "IC|LockTarget")
	bool StartLockOntoTarget(ETraceShapeType TraceType= ETraceShapeType::Sphere, float CheckHalf=500, float Dist=7000);
	UFUNCTION(BlueprintCallable, Category = "IC|LockTarget")
	void EndLockOntoTarget();
	UFUNCTION(BlueprintCallable, Category = "IC|LockTarget")
	bool ChangeTarget();
	UFUNCTION(BlueprintCallable, Category = "IC|Weapon")
	UStaticMeshComponent* GetWeaponComponent() const
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
	FGameplayTag InitWeaponTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Unarmed"));

	UFUNCTION()
	void CharacterDied();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IC|Combat|Montage")
	UAnimMontage* GetHurtMontage;

protected:
	UPROPERTY(EditAnywhere, Category = "IC|LockTrace")
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes = { TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECC_Pawn)) };
	UPROPERTY(EditAnywhere, Category = "IC|LockTrace")
	bool bTraceComplex = false;
	UPROPERTY(EditAnywhere, Category = "IC|LockTrace")
	bool bIgnoreSelf = true;
	UPROPERTY(EditAnywhere, Category = "IC|LockTrace", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, Category = "IC|Combat|Montage")
	UAnimMontage* DiedMontage;
	
	UPROPERTY(BlueprintAssignable)
	FCharacterDied DG_CharacterDied;

private:
	bool IsLockOnTarget=false;
	FTimerHandle TimerLockTarget;
	float LockFrequency=0.2f;
	TArray<FHitResult> RawOutHits;
	TArray<FHitResult> ChangeOutHits;
	FCollisionQueryParams Params;
	FCollisionObjectQueryParams ObjectQueryParams;

	FWeaponStruct CurWeapon;
	UPROPERTY()
	AActor* LockObj;

	void DoLockToTarget();
		
};
