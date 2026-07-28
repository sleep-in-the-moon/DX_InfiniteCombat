// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ICCharacterMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FTraversalCheckInput
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector TraceDirection=FVector::Zero();
	UPROPERTY(BlueprintReadWrite)
	float TraceDistance=0.0f;
	UPROPERTY(BlueprintReadWrite)
	float MaxLedgeHeight=0.0f;
	UPROPERTY(BlueprintReadWrite)
	float MinLedgeHeight=0.0f;
};

UENUM(BlueprintType)
enum class ECusMovementMode : uint8
{
	MOVE_Traversal UMETA(DisplayName = "Traversal"),
	MOVE_Climb UMETA(DisplayName = "Climb")
};

USTRUCT(BlueprintType)
struct FClimbSurfaceInfo 
{
	GENERATED_BODY()

	//多个 Probe 结果的平均法线
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FVector SurfaceNormal;
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FVector SurfacePoint;
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FHitResult PrimaryHitResult;
	//沿法线方向到表面距离
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	float NormalDistance=0.0f;
};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	UAnimMontage* TraversalMontage = nullptr;

public:
	//bool TryTraversalAction(const FTraversalCheckInput& CheckInput);

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysClimbing(float deltaTime, int32 Iterations);

	virtual bool FindClimbSurface(FClimbSurfaceInfo& OutSurface);

private:
	/*UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);*/

public:
	FTraversalCheckInput TraversalCheckInput;

private:
	/*FOnMontageEnded MontageEndedDelegate;

	EMovementMode TempMode;*/

};
