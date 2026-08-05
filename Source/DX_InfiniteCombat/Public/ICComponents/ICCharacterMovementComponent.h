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
	FVector TraceDirection=FVector::ZeroVector;
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
	FVector SurfaceNormal=FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FVector SurfacePoint = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	FHitResult PrimaryHitResult;
	//沿表面法线反方向到表面距离
	UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	float NormalDistance=0.0f;
};

UENUM(BlueprintType)
enum class EClimbProbeLocType: uint8
{
	Bone,
	ZOffset
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

	virtual bool FindAndUpdateClimbSurface();

	void UpdateClimbingAcceleration();

	FVector ComputeAttachVelocity();

	void ClimbAlongSurface(const FVector& InVelocity, float DeltaSeconds, float InRemainingTime, int32 InIterations);

	FQuat ComputeClimbingRotation(float DeltaTime) const;

	bool CheckClimableByHit(const FHitResult& Hit, const FVector& TraceDirection, const FVector& UpDirection);

	virtual TArray<FVector> GetProbeLocations() const;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbRotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbProbeRadius=50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	EClimbProbeLocType ClimbProbeLocType;
	//用于 Climb 物理查询的骨骼名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing", meta = (EditCondition = "ClimbProbeLocType==EClimbProbeLocType::Bone"))
	TArray<FName> ClimbProbeBoneNames;
	//相对于胶囊体中心的 z 偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing", meta = (EditCondition = "ClimbProbeLocType==EClimbProbeLocType::Offset"))
	TArray<float> ClimbProbeZOffsets;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	TEnumAsByte<ECollisionChannel> ClimbTraceChannel = ECC_GameTraceChannel2;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	float ClimbProbeDistance = 200.0f;

private:
	/*FOnMontageEnded MontageEndedDelegate;

	EMovementMode TempMode;*/

};
