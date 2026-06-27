// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExecuteComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCanBeExecutedStateChange, bool, NewState);

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

protected:
	UFUNCTION()
	void CheckExecuteLine(float NewHelth);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExecuteLine = 0.20;
	UPROPERTY(BlueprintReadWrite)
	bool bCanBeExecuted = false;
	UPROPERTY(BlueprintAssignable)
	FCanBeExecutedStateChange DG_CanBeExecutedStateChange;

};
