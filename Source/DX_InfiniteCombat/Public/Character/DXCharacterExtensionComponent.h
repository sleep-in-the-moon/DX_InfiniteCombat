// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"

#include "DXCharacterExtensionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FLerpToControlRotFinish);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API UDXCharacterExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDXCharacterExtensionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	UFUNCTION(BlueprintCallable)
	void LerpSpringArmEndOffsetToTarget(const FVector& TargetOffset, float Time=0.7f);
	UFUNCTION(BlueprintCallable)
	void LerpActorRotToControlRot(float Time=0.7f, bool JustYaw=true);

	UFUNCTION(BlueprintCallable)
	void SetYawByControl(bool YawByControl);

	FLerpToControlRotFinish DG_LerpToControlRotFinish;

private:
	FVector RawSpringOffset;
	FVector TargetSpringOffset;
	FTimeline TL_LerpSpringArm;
	void LerpSpringArmInter(float Alpha);

	FRotator RawActorRot;
	FRotator TargetActorRot;
	FTimeline TL_LerpActorRot;
	void LerpActorRotInter(float Alpha);
	void LerpActorRotFinish();
};
