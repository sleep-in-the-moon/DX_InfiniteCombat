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
	AICAIController();

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, meta = (Category = "BlackboardKey"))
	void UpdateDeadKey(bool IsDead);
	UFUNCTION(BlueprintCallable, meta = (Category = "BlackboardKey"))
	void UpdateTargetKey(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, meta = (Category = "BlackboardKey"))
	void UpdateSightKey(bool HasLineSight);
	
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamId); }

	UFUNCTION(BlueprintCallable)
	FGenericTeamId GetGenericTeamIdBP() const { return FGenericTeamId(TeamId); }
	UFUNCTION(BlueprintCallable)
	ETeamAttitude::Type TeamAttitudeTest(FGenericTeamId B)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("CurTeamId::%d"), GetGenericTeamId().GetId()));
		UE_LOG(LogTemp, Warning, TEXT("CurTeamId::%d"), GetGenericTeamId().GetId());
		return GetGenericTeamId() != B ? ETeamAttitude::Hostile : ETeamAttitude::Friendly;
	}

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

	UPROPERTY(EditAnywhere)
	uint8 TeamId = 1;

private:
	FTimerHandle LoseSightTimer;

};
