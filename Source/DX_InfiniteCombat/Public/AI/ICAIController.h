// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ICAIController.generated.h"


struct FAIStimulus;
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API AICAIController : public AAIController
{
	GENERATED_BODY()

public:
	AICAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, meta = (Category = "BlackboardKey"))
	void UpdateDeadKey(bool IsDead);
	UFUNCTION(BlueprintCallable, meta = (Category = "BlackboardKey"))
	void UpdateTargetKey(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, meta = (Category = "BlackboardKey"))
	void UpdateSightKey(bool HasLineSight);

private:
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BTAsset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LooseSightTime=7.0f;

private:
	FTimerHandle LoseSightTimer;

};
