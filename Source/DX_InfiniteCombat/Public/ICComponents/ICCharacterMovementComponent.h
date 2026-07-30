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

	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	bool IsClimbableSurface = false;
	//多个 Probe 结果的平均法线
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FVector SurfaceNormal;
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FVector SurfacePoint;
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FHitResult PrimaryHitResult;
	//沿表面法线反方向到表面距离
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
	UICCharacterMovementComponent();

public:
	//bool TryTraversalAction(const FTraversalCheckInput& CheckInput);
	bool IsClimbing() const;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysClimbing(float deltaTime, int32 Iterations);

	virtual bool FindClimbSurface(FClimbSurfaceInfo& OutSurface);

	void UpdateClimbingAcceleration();

	FVector ComputeAttachVelocity(const FClimbSurfaceInfo& InSurface);

	void ClimbAlongSurface(const FVector& InVelocity, float DeltaSeconds, float InRemainingTime, int32 InIterations);

	FQuat ComputeClimbingRotation(float DeltaTime) const;

private:
	/*UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);*/

public:
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	UAnimMontage* TraversalMontage = nullptr;

	FTraversalCheckInput TraversalCheckInput;

protected:
	FClimbSurfaceInfo ClimbSurface;

	//摩擦力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbFriction = 8.0f;
	//制动减速
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbBrakingDeceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float WallGap = 7.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float AttachStrength = 1.0f;

private:
	/*FOnMontageEnded MontageEndedDelegate;

	EMovementMode TempMode;*/

};
