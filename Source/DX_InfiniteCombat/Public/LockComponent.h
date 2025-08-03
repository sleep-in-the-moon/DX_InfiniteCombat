// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockComponent.generated.h"

class UWidgetComponent;

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
	bool GetLockEnable()const { return bEnableLock; }
	UFUNCTION(BlueprintCallable)
	void SetLockEnable(bool Enable);
	UFUNCTION(BlueprintCallable)
	bool GetIsLockOnTarget();
	UFUNCTION(BlueprintCallable)
	void SetIsLockOnTarget(bool IsLockOn);
	UFUNCTION(BlueprintCallable)
	void AddToCandidateActors(AActor* AddActor, float score=1.f);
	UFUNCTION(BlueprintCallable)
	void AppendCandidateActors(const TMap<AActor*, float>& AddActors);

//	UFUNCTION(BlueprintCallable)
//	void PauseLock();
//	UFUNCTION(BlueprintCallable)
//	void EndLock();
//private:
//	void UpdateActorScore();
//	void UpdateCurLockActor();

private:
	bool bEnableLock=false;
	UPROPERTY()
	TMap<AActor*, float> CandidateActors;
	UPROPERTY()
	AActor* CurLockActor;

	//自动锁消失时间
	float CancelTime;
	//被锁定者的UI组件
	UWidgetComponent* LockWidgetCom;
	
};
