// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackTraceShape : uint8
{
	Line,
	Box
};

USTRUCT(BlueprintType)
struct FAttackTraceParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTraceComplex = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoreSelf = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMultiTrace = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> IgnoreActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackTraceShape AttackTraceShape = EAttackTraceShape::Line;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AttackTraceShape!=EAttackTraceShape::Line", EditConditionHides))
	FVector ShapeExtent;

	FAttackTraceParam() 
		:bTraceComplex(false), bIgnoreSelf(true), bMultiTrace(true), IgnoreActors(TArray<AActor*>{}), TraceObjectTypes(TArray<TEnumAsByte<EObjectTypeQuery> >{}), AttackTraceShape(EAttackTraceShape::Line), ShapeExtent(FVector(100, 100, 100)) {}

	FAttackTraceParam(EAttackTraceShape InAttackTraceShape, const FVector& InShapeExtent, const TArray<TEnumAsByte<EObjectTypeQuery> >& InTraceObjectTypes)
		:bTraceComplex(false), bIgnoreSelf(true), bMultiTrace(true), IgnoreActors(TArray<AActor*>{}), TraceObjectTypes(InTraceObjectTypes), AttackTraceShape(InAttackTraceShape), ShapeExtent(InShapeExtent) {}

};

class UICAbilitySystemComponent;
class UICWorldSubsystem;
class UMotionWarpingComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DX_INFINITECOMBAT_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TraceBySocketsForTick(USceneComponent* SocketOwner, const TArray<FName>& TraceSockets, TMap<FName, FVector>& PreSocketLoc, TArray<struct FHitResult>& OutHitArray, const FAttackTraceParam& TraceParam);

	UFUNCTION(BlueprintCallable)
	void HitFeedback(UAnimMontage* CurAttackAnimMontage, bool bSlowdown, float SlowdownScale, float SlowdownTime, bool bSkipFrames, float SkipSeconds);
	UFUNCTION(BlueprintCallable)
	void ApplyHitResToTargetActor(const FHitResult& HitRes, float DamageATKCoefficient = 1.0f, float KnockbackDist = 0.0f, bool bCanBeBlock = true);
	
	UFUNCTION(BlueprintCallable)
	void AddOrUpdateAttackFollowWarp(const FVector& NewFollowLocation);
	UFUNCTION(BlueprintCallable)
	void RemoveAttackFollowWarp();

	UFUNCTION(BlueprintCallable)
	void CreateClashWindow();
	UFUNCTION(BlueprintCallable)
	bool HasClashWindow() const;
	UFUNCTION(BlueprintCallable)
	void ConsumeClashWindow();
	UFUNCTION(BlueprintCallable)
	void CloseClashWindow();

private:
	UPROPERTY()
	TWeakObjectPtr<UICAbilitySystemComponent> OwnerASC;
	UPROPERTY()
	UICWorldSubsystem* ICSubSystem = nullptr;
	UPROPERTY()
	USkeletalMeshComponent* MeshComp = nullptr;
	UPROPERTY()
	UMotionWarpingComponent* MotionWarpingComp;

	bool ClashWindow = false;
};
