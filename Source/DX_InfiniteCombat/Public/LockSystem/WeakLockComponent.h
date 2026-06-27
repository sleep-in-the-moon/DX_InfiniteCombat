// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeakLockComponent.generated.h"


class UWidgetCombatStates;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API UWeakLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeakLockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Trigger();

	UFUNCTION(BlueprintCallable)
	void ClearLock();

	UFUNCTION(BlueprintCallable)
	bool IsLockOn() const;

	UFUNCTION(BlueprintCallable)
	AActor* GetLockActor() const
	{
		return LockActor;
	}

private:
	bool DoOnceTrace();
	APlayerController* GetOwnerController();
	FVector2D GetDeltaYawPitch();
	FVector2D GetHalfFOV_VH();
	FVector2D GetMargin_VH();

	void CheckLockActorState();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "WeakLock")
	float ControllerFollowTime = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "WeakLock")
	float MarginPx = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "WeakLock")
	float AttackFollowDist = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "WeakLock")
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes = { TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECC_Pawn)) };

	//被锁定者身上显示的UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LockedWidgetClass;

private:
	bool bControllerFollow=false;
	FTimerHandle ControllerTimer;
	UPROPERTY()
	AActor* LockActor=nullptr;

	UPROPERTY()
	UUserWidget* LockedWidget = nullptr;
	FName LockedWidgetPersistentID = TEXT("LockedWidget");
	TWeakObjectPtr<UWidgetCombatStates> CombatStatesWidget= nullptr;

	bool bIsAttackFollow = false;

};
