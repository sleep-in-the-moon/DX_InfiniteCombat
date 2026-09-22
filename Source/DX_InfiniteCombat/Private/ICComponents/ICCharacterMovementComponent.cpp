// Copyright DX_IC


#include "ICComponents/ICCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "KismetTraceUtils.h"
#include "Subsystem/ICWorldSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/Character.h"
#include "BasicInputSystem/Public/BasicInputPlayerController.h"
#include "AbilitySystemComponent.h"


UICCharacterMovementComponent::UICCharacterMovementComponent()
{
    ClimbBrakingDeceleration = MaxAcceleration;
    //MaxCustomMovementSpeed = 100.0f;
    TraversalCheckInput.TraceDistance = 60.0f;
    TraversalCheckInput.MaxLedgeHeight = 173.0f;
    TraversalCheckInput.MinLedgeHeight = 40.0f;
}

void UICCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    InitMaxWalkSpeed = MaxWalkSpeed;
}

bool UICCharacterMovementComponent::IsClimbing() const
{
    return /*ClimbSurface.IsClimbableSurface && */(MovementMode == MOVE_Custom) && (CustomMovementMode == static_cast<uint8>(ECusMovementMode::MOVE_Climb)) && UpdatedComponent;
}

float UICCharacterMovementComponent::GetMaxSpeed() const
{
    switch (MovementMode)
    {
    case MOVE_Walking:
    case MOVE_NavWalking:
        return IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed;
    case MOVE_Falling:
        return MaxWalkSpeed;
    case MOVE_Swimming:
        return MaxSwimSpeed;
    case MOVE_Flying:
        return MaxFlySpeed;
    case MOVE_Custom:
        switch (CustomMovementMode)
        {
        case static_cast<uint8>(ECusMovementMode::MOVE_Traversal):
            return MaxFlySpeed;
        case static_cast<uint8>(ECusMovementMode::MOVE_Climb):
            return MaxClimbSpeed;
        default:
            return MaxCustomMovementSpeed;
        }
    case MOVE_None:
    default:
        return 0.f;
    }
}

void UICCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
    switch (CustomMovementMode)
    {
    case static_cast<uint8>(ECusMovementMode::MOVE_Traversal):
        PhysFlying(deltaTime, Iterations);
        break;
    case static_cast<uint8>(ECusMovementMode::MOVE_Climb):
        PhysClimbing(deltaTime, Iterations);
        break;
    }

    Super::PhysCustom(deltaTime, Iterations);
}

void UICCharacterMovementComponent::PhysClimbing(float deltaTime, int32 Iterations)
{
    if (deltaTime < MIN_TICK_TIME)
    {
        return;
    }

    if (!UpdatedComponent->IsQueryCollisionEnabled())
    {
        SetMovementMode(MOVE_Custom, static_cast<uint8>(ECusMovementMode::MOVE_Climb));
        return;
    }

    bJustTeleported = false;
    float remainingTime = deltaTime;
    FVector RawAcceleration = Acceleration;
    while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController 
        || HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity() || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
    {
        ++Iterations;
        //bJustTeleported = false;
        float TimeStep = GetSimulationTimeStep(remainingTime, Iterations);
        remainingTime -= TimeStep;

        if (!FindAndUpdateClimbSurface() && IsClimbing())
        {
            SetMovementMode(MOVE_Falling);
            StartNewPhysics(remainingTime + TimeStep, Iterations-1);
            return;
        }

        const FVector OldLocation = UpdatedComponent->GetComponentLocation();

        //将上一次模拟时叠加的叠加型 RootMotion 清除，避免重复叠加
        RestorePreAdditiveRootMotionVelocity();

#if WITH_EDITOR
        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
        if (ICSubSystem && ICSubSystem->GetShowDebug())
        {
            FVector NormalAcceleration = Acceleration.GetSafeNormal();
            DrawDebugDirectionalArrow(GetWorld(), OldLocation, OldLocation + NormalAcceleration * 35, 1.0f, FColor::Purple, false, 10.0f, 0U, 1.0f);
            DrawDebugString(GetWorld(), OldLocation + NormalAcceleration * 20, "RawAcceleration", 0, FColor::Green, 10.0f, false, 1);
        }
#endif

        UpdateClimbingAcceleration();

#if WITH_EDITOR
        /*FVector NormalAcceleration = Acceleration.GetSafeNormal();
        DrawDebugDirectionalArrow(GetWorld(), OldLocation, OldLocation + NormalAcceleration * 35, 1.0f, FColor::Green, false, 10.0f, 0U, 1.0f);
        DrawDebugString(GetWorld(), OldLocation + NormalAcceleration * 20, "Acceleration", 0, FColor::Green, 10.0f, false, 1);*/
#endif

        if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
        {
            CalcVelocity(TimeStep, ClimbFriction, false, ClimbBrakingDeceleration);
        }

        //应用 RootMotion，动画 RootMotion 或 非动画 RootMotion: 覆盖型和叠加型
        ApplyRootMotionToVelocity(TimeStep);

#if WITH_EDITOR
        if (ICSubSystem && ICSubSystem->GetShowDebug())
        {
            FVector NormalVelocity = Velocity.GetSafeNormal();
            DrawDebugDirectionalArrow(GetWorld(), OldLocation, OldLocation + NormalVelocity * 35, 1.0f, FColor::Orange, false, 10.0f, 0U, 1.0f);
            DrawDebugString(GetWorld(), OldLocation + NormalVelocity * 20, "Velocity", 0, FColor::Green, 10.0f, false, 1);
        }
#endif

        if (!IsClimbing())
        {
            StartNewPhysics(remainingTime + TimeStep, Iterations - 1);
            return;
        }

        const FVector ClimbVelocity = Velocity;
        ClimbAlongSurface(ClimbVelocity, TimeStep, remainingTime, Iterations);
        
        if (IsClimbing())
        {
            if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && TimeStep >= MIN_TICK_TIME)
            {
                Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / TimeStep;
                // 不把吸附修正积累为下一步的法线速度。
                Velocity = FVector::VectorPlaneProject(Velocity, ClimbSurface.SurfaceNormal);
                Acceleration = RawAcceleration;
            }
        }

        // If we didn't move at all this iteration then abort (since future iterations will also be stuck).
        if (UpdatedComponent->GetComponentLocation() == OldLocation)
        {
            remainingTime = 0.f;
            break;
        }
    }

    if (IsClimbing())
    {
        Velocity = FVector::VectorPlaneProject(Velocity, ClimbSurface.SurfaceNormal);
    }
}

bool UICCharacterMovementComponent::FindAndUpdateClimbSurface()
{
    if (!CharacterOwner || !UpdatedComponent || !GetWorld())
    {
        return false;
    }
    const UCapsuleComponent* Capsule = CharacterOwner->GetCapsuleComponent();
    if (!Capsule)
    {
        return false;
    }

    //FClimbSurfaceInfo ClimbSurfaceCp = ClimbSurface;
    FVector CapsulCenter = Capsule->GetComponentLocation();
    float CpasuleRadius = 0.0f;
    float CpasuleHalfHeight = 0.0f;
    Capsule->GetScaledCapsuleSize(CpasuleRadius, CpasuleHalfHeight);

    const TArray<FClimbProbeLocation> ProbeLocs = GetProbeStartLocations();
    if (ProbeLocs.Num() < 1)
        return false;

    FVector TraceDirection;
    if (IsClimbing() && !ClimbSurface.SurfaceNormal.IsNearlyZero())
    {
        TraceDirection = -ClimbSurface.SurfaceNormal;
    }
    else
    {
        TraceDirection = FVector::VectorPlaneProject(UpdatedComponent->GetForwardVector(), -GetGravityDirection());
    }
    TraceDirection = TraceDirection.GetSafeNormal();
    if (TraceDirection.IsNearlyZero())
    {
        return false;
    }
    
    FVector NormalSum = FVector::ZeroVector;
    FVector PointSum = FVector::ZeroVector;
    uint8 UpperCount = 0;
    uint8 CenterCount = 0;
    uint8 LowerCount = 0;
    uint8 HitCount = 0;
    for (const FClimbProbeLocation& ProbeLoc : ProbeLocs)
    {
        FCollisionQueryParams QueryParam(SCENE_QUERY_STAT(FindClimbSurface), false, CharacterOwner);
        const FCollisionShape ProbeShape = FCollisionShape::MakeSphere(ClimbProbeRadius);
        FVector EndLoc = ProbeLoc.ProbeLocation + TraceDirection* ClimbProbeDistance;
        FHitResult HitRes;

#if WITH_EDITOR
        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
        if (ICSubSystem && ICSubSystem->GetShowDebug())
        {
            DrawDebugSphereTraceSingle(GetWorld(), ProbeLoc.ProbeLocation, EndLoc, ClimbProbeRadius, EDrawDebugTrace::Type::ForDuration, false, HitRes, FLinearColor::Blue, FLinearColor::Green, 4.0f);
        }
#endif
        const bool bHit = GetWorld()->SweepSingleByChannel(HitRes, ProbeLoc.ProbeLocation, EndLoc, FQuat::Identity, ClimbTraceChannel, ProbeShape, QueryParam);
        if (!bHit || !CheckClimableByHit(HitRes, TraceDirection, -GetGravityDirection()))
            continue;

        if (ProbeLoc.ProbeLayer != EClimbProbeLayer::Anim)
        {
            //ClimbSurface.PrimaryHitResult = HitRes;
            NormalSum += HitRes.ImpactNormal;
            PointSum += HitRes.ImpactPoint;
            ++HitCount;
        }
        
        switch (ProbeLoc.ProbeLayer)
        {
        case EClimbProbeLayer::Center:
            ++CenterCount;
            break;
        case EClimbProbeLayer::Upper:
            ++UpperCount;
            break;
        case EClimbProbeLayer::Lower:
            ++LowerCount;
            break;
        default:
            break;
        }
    }

    NormalSum = NormalSum.GetSafeNormal();
    if (NormalSum.IsNearlyZero())
    {
        return false;
    }
    ClimbSurface.SurfaceNormal = NormalSum;
    ClimbSurface.SurfacePoint = PointSum / HitCount;
#if WITH_EDITOR
    /*DrawDebugDirectionalArrow(GetWorld(), ClimbSurface.SurfacePoint, ClimbSurface.SurfacePoint + ClimbSurface.SurfaceNormal * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
    DrawDebugString(GetWorld(), ClimbSurface.SurfacePoint, "PointSum", 0, FColor::Green, 10.0f, false, 1);
    DrawDebugString(GetWorld(), ClimbSurface.SurfacePoint + ClimbSurface.SurfaceNormal * 20, "NormalSum", 0, FColor::Green, 10.0f, false, 1);*/
#endif
    ClimbSurface.NormalDistance = FVector::DotProduct(Capsule->GetComponentLocation() - ClimbSurface.SurfacePoint, ClimbSurface.SurfaceNormal);
    if (ClimbSurface.NormalDistance < 0.0f)
    {
        return false;
    }

    if (UpperCount == 0 && CenterCount > 0)
    {
        TraversalCheckInput.TraceDirection = TraceDirection;
        if(UAbilitySystemComponent* ASC = PawnOwner->FindComponentByClass<UAbilitySystemComponent>())
            ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Action.Traversal"), false)));

        return false;
    }

    //ClimbSurface.IsClimbableSurface = true;
    return true;
}

void UICCharacterMovementComponent::UpdateClimbingAcceleration()
{
    /*if (!ClimbSurface.IsClimbableSurface)
        return;*/

   /* Acceleration = FVector::VectorPlaneProject(Acceleration, ClimbSurface.SurfaceNormal);
    Acceleration = Acceleration.GetClampedToMaxSize(GetMaxAcceleration());*/

    FVector MoveForward = GetOwner()->GetActorForwardVector();
    FVector MoveRight = GetOwner()->GetActorRightVector();
    if (ABasicInputPlayerController* ICController = Cast<ABasicInputPlayerController>(CharacterOwner->Controller.Get()))
    {
        ICController->GetMoveDirection(MoveForward, MoveRight);
    }
    
    const float ForwardAmount = FVector::DotProduct(Acceleration, MoveForward);// MoveForward 是单位向量，Acceleration 点积 MoveForward 得到 Acceleration 在 MoveForward 方向上的投影长度
    const float RightAmount = FVector::DotProduct(Acceleration, MoveRight);
    const FVector ClimbUp = FVector::VectorPlaneProject(-GetGravityDirection(), ClimbSurface.SurfaceNormal).GetSafeNormal();
    const FVector ClimbRight = FVector::CrossProduct(ClimbUp, -ClimbSurface.SurfaceNormal).GetSafeNormal();
    Acceleration = ClimbUp * ForwardAmount + ClimbRight * RightAmount;
}

FVector UICCharacterMovementComponent::ComputeAttachVelocity()
{
    const FVector CurLocation = UpdatedComponent->GetComponentLocation();

    ClimbSurface.NormalDistance = FVector::DotProduct(CurLocation - ClimbSurface.SurfacePoint, ClimbSurface.SurfaceNormal);

    const float CalibrateDist = ClimbSurface.NormalDistance - (CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + WallGap);

    return -ClimbSurface.SurfaceNormal * (CalibrateDist*AttachStrength);
}

void UICCharacterMovementComponent::ClimbAlongSurface(const FVector& InVelocity, float DeltaSeconds, float InRemainingTime, int32 InIterations)
{
    /*if (!ClimbSurface.IsClimbableSurface)
        return;*/

    // 速度位于墙面切平面
    FVector MoveDelta = FVector::VectorPlaneProject(InVelocity, ClimbSurface.SurfaceNormal);
    const FVector AttachVelocity = ComputeAttachVelocity();

#if WITH_EDITOR
    /*DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + MoveDelta * 35, 1.0f, FColor::Red, false, 10.0f, 0U, 1.0f);
    DrawDebugString(GetWorld(), UpdatedComponent->GetComponentLocation() + MoveDelta * 20, "MoveDeltaBeforeAttach", 0, FColor::Green, 10.0f, false, 1);*/
   /* DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + AttachVelocity * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
    DrawDebugString(GetWorld(), UpdatedComponent->GetComponentLocation() + AttachVelocity * 20, "AttachVelocity", 0, FColor::Green, 10.0f, false, 1);*/
#endif
    
    MoveDelta = (InVelocity + AttachVelocity) * DeltaSeconds;

#if WITH_EDITOR
    UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
    if (ICSubSystem && ICSubSystem->GetShowDebug())
    {
        DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + MoveDelta * 35, 1.0f, FColor::Black, false, 10.0f, 0U, 1.0f);
        DrawDebugString(GetWorld(), UpdatedComponent->GetComponentLocation() + MoveDelta * 20, "MoveDelta", 0, FColor::Green, 10.0f, false, 1);
    }
#endif

    FHitResult Hit(1.f);
    SafeMoveUpdatedComponent(MoveDelta, ComputeClimbingRotation(DeltaSeconds), true, Hit);
    float LastMoveTimeSlice = DeltaSeconds;

    if (Hit.bStartPenetrating)
    {
        HandleImpact(Hit);
        SlideAlongSurface(MoveDelta, 1.f, Hit.Normal, Hit, true);

        if (Hit.bStartPenetrating)
        {
            OnCharacterStuckInGeometry(&Hit);
        }
    }
    else if (Hit.IsValidBlockingHit())
    {
        HandleImpact(Hit, DeltaSeconds, MoveDelta);

        // HandleImpact 中的事件可能修改移动模式。
        if (!IsClimbing())
        {
            const float UnusedTime = DeltaSeconds * (1.0f - Hit.Time);
            InRemainingTime += UnusedTime;

            StartNewPhysics(InRemainingTime, InIterations);
            return;
        }

        SlideAlongSurface(MoveDelta, 1.0f - Hit.Time, Hit.Normal, Hit, true);
    }
}

FQuat UICCharacterMovementComponent::ComputeClimbingRotation(float DeltaTime) const
{
    const FQuat CurrentQuat = UpdatedComponent->GetComponentQuat();

    if (ClimbSurface.SurfaceNormal.IsNearlyZero())
    {
        return CurrentQuat;
    }

    // 将墙面法线的 重力反方向分量 剔除，会使角色保持竖直，不会贴合墙面的 pitch
   /* FVector DesiredForward = FVector::VectorPlaneProject(-ClimbSurface.SurfaceNormal, -GetGravityDirection());
    if (!DesiredForward.Normalize())
    {
        return CurrentRotation;
    }
    FQuat TargetQuat = FRotationMatrix::MakeFromXZ(DesiredForward,  - GetGravityDirection()).ToQuat();*/

    // 将 重力反方向 投影到墙面，得到墙面的上方向，会使角色的 pitch 也贴合墙面
    FVector DesiredUp = FVector::VectorPlaneProject(-GetGravityDirection(), ClimbSurface.SurfaceNormal);
    if (!DesiredUp.Normalize())
    {
        return CurrentQuat;
    }
    FQuat TargetQuat = FRotationMatrix::MakeFromXZ(-ClimbSurface.SurfaceNormal, DesiredUp).ToQuat();

    const float Alpha = 1.0f - FMath::Exp(-ClimbRotationSpeed * DeltaTime);

    return FQuat::Slerp(CurrentQuat, TargetQuat, Alpha).GetNormalized();
}

bool UICCharacterMovementComponent::CheckClimableByHit(const FHitResult& Hit, const FVector& TraceDirection, const FVector& UpDirection)
{
    if(/*!Hit.IsValidBlockingHit() ||*/ !Hit.bBlockingHit)
        return false;

    const FVector Normal = Hit.ImpactNormal.GetSafeNormal();
    if (Normal.IsNearlyZero())
    {
        return false;
    }

    const float NormalDotUp = FMath::Abs(FVector::DotProduct(Normal, UpDirection));
    if (NormalDotUp > 0.5)//cos(倾斜度)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("倾斜度 %d"), NormalDotUp));
        return false;
    }
    
    const float FaceDot = FVector::DotProduct(TraceDirection, -Normal);
    if (FaceDot < 0.8)//相似度
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("相似度 %d"), FaceDot));
        return false;
    }

    //Hit.PhysMaterial;
    return true;
}

TArray<FClimbProbeLocation> UICCharacterMovementComponent::GetProbeStartLocations() const
{
    TArray<FClimbProbeLocation> Res;
    switch (ClimbProbeLocType)
    {
    case EClimbProbeLocType::Bone:
        if (UMeshComponent* Mesh = GetOwner()->FindComponentByClass<UMeshComponent>())
        {
            for (const FClimbProbeBoneName& BoneName : ClimbProbeBoneNames)
            {
                if (BoneName.ClimbProbeBoneName.IsNone() || !Mesh->DoesSocketExist(BoneName.ClimbProbeBoneName))
                    continue;

                Res.Add(FClimbProbeLocation(Mesh->GetSocketLocation(BoneName.ClimbProbeBoneName), BoneName.ProbeLayer));
            }
        }
        break;

    case EClimbProbeLocType::ZOffset:
        if (UCapsuleComponent* CapsulComp = GetOwner()->FindComponentByClass<UCapsuleComponent>())
        {
            for (const FClimbProbeZOffset ZOffset : ClimbProbeZOffsets)
            {
                if(!FMath::IsNearlyEqual(ZOffset.ClimbProbeZOffset, 0.0f))
                    Res.Add(FClimbProbeLocation(CapsulComp->GetComponentLocation() + GetGravityDirection() * ZOffset.ClimbProbeZOffset, ZOffset.ProbeLayer));
            }
        }
        break;
    }

    Res.Add(FClimbProbeLocation(UpdatedComponent->GetComponentLocation(), EClimbProbeLayer::Center));
    return Res;
}

//void UICCharacterMovementComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//    SetMovementMode(TempMode);
//    MontageEndedDelegate.Unbind();
//}
