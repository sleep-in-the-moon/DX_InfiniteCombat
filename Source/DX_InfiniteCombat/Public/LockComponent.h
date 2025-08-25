// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockComponent.generated.h"

class UWidgetComponent;
class UWidgetCombatStates;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API ULockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool GetIsAutoLockEnable() const { return bAutoTrace; }
	UFUNCTION(BlueprintCallable)
	void SetAutoLockEnable(bool Enable);

	UFUNCTION(BlueprintCallable)
	bool GetIsLockingOnTarget();
	UFUNCTION(BlueprintCallable)
	void SetIsLockingOnTarget(bool IsLockOn);

	UFUNCTION(BlueprintCallable)
	void AddToCandidateActors(AActor* AddActor, float score=1.f);
	UFUNCTION(BlueprintCallable)
	void AppendCandidateActors(const TMap<AActor*, float>& AddActors);

	UFUNCTION(BlueprintCallable)
	AActor* GetCurLockActor() const
	{
		return CurLockActor;
	}

//	UFUNCTION(BlueprintCallable)
//	void EndLock();

private:
	bool DoOnceTrace();
	void WhenLockingOnActor(AActor* LockedActor);
	void UpdateLockState();

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes = { TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECC_Pawn)) };

	//被锁定者身上显示的UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LockedWidgetClass;

private:
	UPROPERTY()
	UUserWidget* LockedWidget= nullptr;
	FName LockedWidgetPersistentID = TEXT("LockedWidget");
	TWeakObjectPtr<UWidgetCombatStates> CombatStatesWidget;

	UPROPERTY()
	AActor* CurLockActor= nullptr;
	UPROPERTY()
	TMap<AActor*, float> CandidateActors;

	bool bAutoTrace = false;
	float AutoTraceInterval = 0.4f;
	FTimerHandle AutoTraceTimer;

};
