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

UENUM(BlueprintType)
enum class EClimbProbeLayer :uint8
{
	Center,
	Upper,
	Lower,
	Anim
};

USTRUCT(BlueprintType)
struct FClimbProbeBoneName
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClimbProbeBoneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClimbProbeLayer ProbeLayer;
};
USTRUCT(BlueprintType)
struct FClimbProbeZOffset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClimbProbeZOffset=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClimbProbeLayer ProbeLayer;
};
USTRUCT(BlueprintType)
struct FClimbProbeLocation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProbeLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClimbProbeLayer ProbeLayer;
};

USTRUCT(BlueprintType)
struct FClimbSurfaceInfo 
{
	GENERATED_BODY()

	/*UPROPERTY(BlueprintReadOnly, Category = "ClimbSurface")
	bool IsClimbableSurface = false;*/
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

	virtual void BeginPlay() override;

public:
	//bool TryTraversalAction(const FTraversalCheckInput& CheckInput);
	bool IsClimbing() const;

	virtual float GetMaxSpeed() const override;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysClimbing(float deltaTime, int32 Iterations);

	virtual bool FindAndUpdateClimbSurface();

	void UpdateClimbingAcceleration();

	FVector ComputeAttachVelocity();

	void ClimbAlongSurface(const FVector& InVelocity, float DeltaSeconds, float InRemainingTime, int32 InIterations);

	FQuat ComputeClimbingRotation(float DeltaTime) const;

	bool CheckClimableByHit(const FHitResult& Hit, const FVector& TraceDirection, const FVector& UpDirection);

	virtual TArray<FClimbProbeLocation> GetProbeStartLocations() const;

private:
	/*UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);*/

public:
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	UAnimMontage* TraversalMontage = nullptr;

	FTraversalCheckInput TraversalCheckInput;

	UPROPERTY(BlueprintReadOnly)
	float InitMaxWalkSpeed = 0.0;

protected:
	FClimbSurfaceInfo ClimbSurface;

	//摩擦力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbFriction = 12.0f;
	//制动减速
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbBrakingDeceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float MaxClimbSpeed = 50.0f;

	//胶囊体与墙之间的间隙
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float WallGap = 24.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float AttachStrength = 7.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbRotationSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing|Probe")
	float ClimbProbeRadius=7.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing|Probe")
	EClimbProbeLocType ClimbProbeLocType;
	//用于 Climb 物理查询的骨骼名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing|Probe", meta = (EditCondition = "ClimbProbeLocType==EClimbProbeLocType::Bone"))
	TArray<FClimbProbeBoneName> ClimbProbeBoneNames;
	//相对于胶囊体中心的 z 偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing|Probe", meta = (EditCondition = "ClimbProbeLocType==EClimbProbeLocType::Offset"))
	TArray<FClimbProbeZOffset> ClimbProbeZOffsets;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Probe")
	TEnumAsByte<ECollisionChannel> ClimbTraceChannel = ECC_GameTraceChannel2;
	UPROPERTY(EditDefaultsOnly, Category = "Climbing|Probe")
	float ClimbProbeDistance = 80.0f;

private:
	/*FOnMontageEnded MontageEndedDelegate;

	EMovementMode TempMode;*/

};
