// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "Engine/EngineTypes.h"

#include "BowComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArrowCountChange, int32, NewCount);

class AProjectorActorBase;
class UForesightWidgeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API UBowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool TrySetupArrow();
	UFUNCTION(BlueprintCallable)
	bool TryLaunchArrow(float ChargeTime);

	UFUNCTION(BlueprintCallable)
	void EnterAimMode();
	UFUNCTION(BlueprintCallable)
	void ExitAimMode();

	UFUNCTION(BlueprintCallable)
	bool TryHoldingBow();
	UFUNCTION(BlueprintCallable)
	void UnHoldingBow();

private:
	void LerpToControlRotFinish();

public:
	UPROPERTY(BlueprintReadWrite)
	bool bCanLuanch = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bow|Arrow")
	int32 ArrowCount = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Bow|Arrow")
	TSubclassOf<AProjectorActorBase> ArrowClass;
	UPROPERTY(EditDefaultsOnly, Category = "Bow|Arrow")
	FGameplayTag ArrowTag = FGameplayTag::RequestGameplayTag(TEXT("Weapon.Bow.Arrow"), false);
	UPROPERTY(EditDefaultsOnly, Category = "Bow|Arrow")
	FString ArrowTraceSocketPrefix = "DamageLoc";

	UPROPERTY(BlueprintAssignable)
	FArrowCountChange DG_ArrowCountChange;

	UPROPERTY(EditDefaultsOnly, Category = "Bow|SocketsName")
	FName HandArrowSocketName = TEXT("HandArrow_Socket");

	UPROPERTY(EditDefaultsOnly, Category = "Bow|Aim")
	FGameplayTag AimTag = FGameplayTag::RequestGameplayTag(TEXT("State.InAiming"), false);
	UPROPERTY(EditDefaultsOnly, Category = "Bow|Aim")
	FVector AimModeCameraOffset = FVector(277.0f, 0.0f, -7.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Bow|UI")
	TSubclassOf<UForesightWidgeBase> ForesightWidgeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Bow|Trace")
	float TraceDist= 10000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Bow|Trace")
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjTypes =
		{ TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECC_WorldStatic)),
		  TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody)) };

	UPROPERTY(EditDefaultsOnly, Category = "Bow|Sounds")
	USoundBase* ArrowLaunchSound;

private:
	UPROPERTY()
	AProjectorActorBase* CurHoldingArrow = nullptr;

	UPROPERTY()
	UForesightWidgeBase* ForesightWidge = nullptr;

	FTimerHandle HidUITimer;
	FDelegateHandle LerpToControlRotFinishHandle;
};
