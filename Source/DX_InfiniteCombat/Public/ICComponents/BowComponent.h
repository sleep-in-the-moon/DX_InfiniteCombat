// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"

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
	bool TryLaunchArrow();

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ArrowCount = 20;

	UPROPERTY(BlueprintAssignable)
	FArrowCountChange DG_ArrowCountChange;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectorActorBase> ArrowClass;

	UPROPERTY(EditDefaultsOnly, Category = "SocketsName")
	FName HandArrowSocketName = TEXT("HandArrow_Socket");

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTag ArrowTag=FGameplayTag::RequestGameplayTag(TEXT("Weapon.Bow.Arrow"), false);
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTag AimTag = FGameplayTag::RequestGameplayTag(TEXT("State.InAiming"), false);

	UPROPERTY(EditDefaultsOnly)
	FVector AimModeCameraOffset = FVector(277.0f, 0.0f, -7.0f);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UForesightWidgeBase> ForesightWidgeClass;

private:
	UPROPERTY()
	AProjectorActorBase* CurHoldingArrow = nullptr;

	UPROPERTY()
	UForesightWidgeBase* ForesightWidge = nullptr;

	FDelegateHandle LerpToControlRotFinishHandle;
};
