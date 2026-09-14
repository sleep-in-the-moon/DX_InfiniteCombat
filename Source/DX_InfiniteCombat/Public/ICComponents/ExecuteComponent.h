// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExecuteComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCanBeExecutedStateChange, bool, NewState);

class UGA_Execution;
class ULevelSequence;

USTRUCT(BlueprintType)
struct FExecutionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ExecuteMontageLongDistance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ExecuteMontageInplace = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevelSequence* CameraSequenceLongDistance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevelSequence* CameraSequenceInplace = nullptr;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API UExecuteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExecuteComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//是否可以对其他 actor 执行处决
	UFUNCTION(BlueprintCallable)
	bool CheckCanDoExecution() const;

protected:
	UFUNCTION()
	void CheckExecuteLine(float NewHelth);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExecuteLine = 0.20;
	//是否可被其他 actor 处决
	UPROPERTY(BlueprintReadWrite)
	bool bCanBeExecuted = false;
	UPROPERTY(BlueprintAssignable)
	FCanBeExecutedStateChange DG_CanBeExecutedStateChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FExecutionConfig ExecutionConfig;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGA_Execution> ExecutionAbilityClass;

};
