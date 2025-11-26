// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeakLockComponent.generated.h"


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

private:
	bool DoOnceTrace();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "WeakLock")
	float ControllerFollowTime = 7.0f;

	UPROPERTY(EditDefaultsOnly, Category = "WeakLock")
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes = { TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECC_Pawn)) };

private:
	bool bControllerFollow=false;
	FTimerHandle ControllerTimer;
	AActor* LockActor=nullptr;

};
