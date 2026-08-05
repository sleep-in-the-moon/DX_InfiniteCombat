// Copyright DX_IC


#include "ICComponents/ICCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "KismetTraceUtils.h"
#include "Subsystem/ICWorldSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/Character.h"


//bool UICCharacterMovementComponent::TryTraversalAction(const FTraversalCheckInput& CheckInput)
//{
//    if (UCapsuleComponent* Capsule = GetOwner()->FindComponentByClass<UCapsuleComponent>())
//    {
//        FHitResult OutHit;
//        // Start:取胶囊体底部加最高最低的中点Z
//        FVector Start = (Capsule->GetComponentLocation() + Capsule->GetUpVector()* ((CheckInput.MaxLedgeHeight - CheckInput.MinLedgeHeight)/2 - Capsule->GetScaledCapsuleHalfHeight()));
//        FVector End = Start + CheckInput.TraceDirection* CheckInput.TraceDistance;
//
//        //ECollisionChannel CollisionChannel = UDX_StaticFunlib::GetCustomTraceChannelByName(TEXT("Traversable"));
//        ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel1;
//        
//        FCollisionQueryParams Params = UDX_StaticFunlib::MakeCollisionQueryParam(GetOwner(), TArray<AActor*>(), TEXT("TraversalAction1"));
//
//        EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::None;
//        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
//        if (ICSubSystem && ICSubSystem->GetShowDebug())
//        {
//            DebugType = EDrawDebugTrace::Type::ForDuration;
//            DrawDebugCapsuleTraceSingle(GetOwner()->GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight(), DebugType, false, OutHit, FLinearColor::Blue, FLinearColor::Green, 4.0f);
//        }
//
//        if (CollisionChannel != ECollisionChannel::ECC_MAX && 
//            GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight()), Params)
//            && !IsWalkable(OutHit) && !OutHit.bStartPenetrating)
//        {
//            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction1 Success")));
//            UE_LOG(LogTemp, Warning, TEXT("TraversalAction1 Success"));
//
//
//            End = FVector(OutHit.ImpactPoint.X, OutHit.ImpactPoint.Y, (Capsule->GetComponentLocation() + Capsule->GetUpVector()*-Capsule->GetScaledCapsuleHalfHeight()).Z);
//            End += OutHit.ImpactNormal * -Capsule->GetScaledCapsuleRadius() / 2;
//            Start = End + FVector(0, 0, CheckInput.MaxLedgeHeight+1);
//            FHitResult OutHit2;
//
//            if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(CollisionChannel), false
//                , TArray<AActor*>(), DebugType, OutHit2, true, FLinearColor::Blue, FLinearColor::Green, 4.0f) && IsWalkable(OutHit2))//OutHit2::手抓点
//            {
//                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction2 Success")));
//                UE_LOG(LogTemp, Warning, TEXT("TraversalAction2 Success"));
//
//
//                Start = FVector(OutHit2.Location.X, OutHit2.Location.Y, OutHit2.ImpactPoint.Z +
//                        Capsule->GetScaledCapsuleHalfHeight() + Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere());
//                End = FVector(OutHit2.Location.X, OutHit2.Location.Y, OutHit2.ImpactPoint.Z +
//                        Capsule->GetScaledCapsuleHalfHeight() - Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere());
//                FHitResult OutHit3;
//
//                if (!(UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(CollisionChannel), false
//                    , TArray<AActor*>(), DebugType, OutHit3, true, FLinearColor::Red, FLinearColor::Yellow, 4.0f) || OutHit3.bStartPenetrating))
//                {
//                    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction3 Success")));
//                    UE_LOG(LogTemp, Warning, TEXT("TraversalAction3 Success"));
//
//                    FTransform TargetTransform = FTransform((OutHit.ImpactNormal * FVector(-1, -1, 0)).ToOrientationRotator(),
//                        FVector(OutHit2.Location.X, OutHit2.Location.Y, OutHit2.ImpactPoint.Z + Capsule->GetScaledCapsuleHalfHeight()));
//
//                    DrawDebugPoint(GetWorld(), OutHit2.ImpactPoint, 20, FColor::Black, false, 3);
//                    DrawDebugPoint(GetWorld(), TargetTransform.GetLocation(), 20, FColor::Green, false, 3);
//
//                    float Height = TargetTransform.GetLocation().Z - GetOwner()->GetActorLocation().Z;
//
//                    // MotionWarping  PlayMontage
//                    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
//                    UMotionWarpingComponent* MotionWarpingComp = GetOwner()->FindComponentByClass<UMotionWarpingComponent>();
//
//                    if (MotionWarpingComp && OwnerCharacter && TraversalMontage)
//                    {
//                        MotionWarpingComp->AddOrUpdateWarpTargetFromTransform(TEXT("ClimbHandPointZ"), FTransform(FRotator(0, -172, 0), FVector(Capsule->GetComponentLocation().X, Capsule->GetComponentLocation().Y, OutHit2.ImpactPoint.Z)));
//                        //MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(TEXT("ClimbHandPoint"), OutHit2.ImpactPoint);
//                        //MotionWarpingComp->AddOrUpdateWarpTargetFromTransform(TEXT("ClimbHandPoint"), FTransform(GetOwner()->GetActorRotation(), OutHit2.ImpactPoint));
//                       
//                        //MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(TEXT("ClimbStandPoint"), FVector(FVector2D(TargetTransform.GetLocation()), TargetTransform.GetLocation().Z+2));
//                        MotionWarpingComp->AddOrUpdateWarpTargetFromTransform(TEXT("ClimbStandPoint"), FTransform(FRotator(0, -172, 0), TargetTransform.GetLocation()));
//
//                        UAnimInstance* AnimInstance = (OwnerCharacter->GetMesh()) ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
//                        if (AnimInstance)
//                        {
//                            TempMode = MovementMode;
//                            SetMovementMode(EMovementMode::MOVE_Flying);
//
//                            OwnerCharacter->PlayAnimMontage(TraversalMontage);
//
//                            MontageEndedDelegate.BindUObject(this, &UICCharacterMovementComponent::OnMontageEnded);
//                            AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, TraversalMontage);
//                        }
//                        
//                    }
//
//                    return true;
//                }
//            }
//        }
//    }
//
//    return false;
//}

UICCharacterMovementComponent::UICCharacterMovementComponent()
{
    ClimbBrakingDeceleration = MaxAcceleration;
}

bool UICCharacterMovementComponent::IsClimbing() const
{
    return /*ClimbSurface.IsClimbableSurface && */(MovementMode == MOVE_Custom) && (CustomMovementMode == static_cast<uint8>(ECusMovementMode::MOVE_Climb)) && UpdatedComponent;
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
    while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController 
        || HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity() || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
    {
        ++Iterations;
        //bJustTeleported = false;
        float TimeStep = GetSimulationTimeStep(remainingTime, Iterations);
        remainingTime -= TimeStep;

        if (!FindAndUpdateClimbSurface())
        {
            SetMovementMode(MOVE_Falling);
            StartNewPhysics(remainingTime + TimeStep, Iterations-1);
            return;
        }

        const FVector OldLocation = UpdatedComponent->GetComponentLocation();

        //将上一次模拟时叠加的叠加型 RootMotion 清除，避免重复叠加
        RestorePreAdditiveRootMotionVelocity();

#if WITH_EDITOR
        DrawDebugDirectionalArrow(GetWorld(), OldLocation, OldLocation + Acceleration * 35, 1.0f, FColor::Black, false, 10.0f, 0U, 1.0f);
        DrawDebugString(GetWorld(), OldLocation + Acceleration * 20, "RawAcceleration", 0, FColor::Green, 10.0f, false, 1);
#endif

        UpdateClimbingAcceleration();

#if WITH_EDITOR
        DrawDebugDirectionalArrow(GetWorld(), OldLocation, OldLocation + Acceleration * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
        DrawDebugString(GetWorld(), OldLocation + Acceleration * 20, "Acceleration", 0, FColor::Green, 10.0f, false, 1);
#endif

        if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
        {
            CalcVelocity(TimeStep, ClimbFriction, false, ClimbBrakingDeceleration);
        }

        //应用 RootMotion，动画 RootMotion 或 非动画 RootMotion: 覆盖型和叠加型
        ApplyRootMotionToVelocity(TimeStep);

#if WITH_EDITOR
        DrawDebugDirectionalArrow(GetWorld(), OldLocation, OldLocation + Velocity * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
        DrawDebugString(GetWorld(), OldLocation + Velocity * 20, "Velocity", 0, FColor::Green, 10.0f, false, 1);
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
            }
        }

        // If we didn't move at all this iteration then abort (since future iterations will also be stuck).
        if (UpdatedComponent->GetComponentLocation() == OldLocation)
        {
            remainingTime = 0.f;
            break;
        }
    }

    /*if (IsClimbing())
    {
        Velocity = FVector::VectorPlaneProject(Velocity, ClimbSurface.SurfaceNormal);
    }*/
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

    const TArray<FVector> ProbeLocs = GetProbeLocations();
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
    for (const FVector& ProbeLoc : ProbeLocs)
    {
        FCollisionQueryParams QueryParam(SCENE_QUERY_STAT(FindClimbSurface), false, CharacterOwner);
        const FCollisionShape ProbeShape = FCollisionShape::MakeSphere(ClimbProbeRadius);
        FVector EndLoc = ProbeLoc + TraceDirection* ClimbProbeDistance;
        FHitResult HitRes;

#if WITH_EDITOR
        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
        if (ICSubSystem && ICSubSystem->GetShowDebug())
        {
            DrawDebugSphereTraceSingle(GetWorld(), ProbeLoc, EndLoc, ClimbProbeRadius, EDrawDebugTrace::Type::ForDuration, false, HitRes, FLinearColor::Blue, FLinearColor::Green, 4.0f);
        }
#endif
        const bool bHit = GetWorld()->SweepSingleByChannel(HitRes, ProbeLoc, EndLoc, FQuat::Identity, ClimbTraceChannel, ProbeShape, QueryParam);
        if (!bHit || !CheckClimableByHit(HitRes, TraceDirection, -GetGravityDirection()))
            continue;

        //ClimbSurface.PrimaryHitResult = HitRes;
        NormalSum += HitRes.ImpactNormal;
        PointSum += HitRes.ImpactPoint;
    }

    NormalSum = NormalSum.GetSafeNormal();
    if (NormalSum.IsNearlyZero())
    {
        return false;
    }
    ClimbSurface.SurfaceNormal = NormalSum;
    ClimbSurface.SurfacePoint = PointSum / ProbeLocs.Num();
#if WITH_EDITOR
    DrawDebugDirectionalArrow(GetWorld(), ClimbSurface.SurfacePoint, ClimbSurface.SurfacePoint + ClimbSurface.SurfaceNormal * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
    DrawDebugString(GetWorld(), ClimbSurface.SurfacePoint, "PointSum", 0, FColor::Green, 10.0f, false, 1);
    DrawDebugString(GetWorld(), ClimbSurface.SurfacePoint + ClimbSurface.SurfaceNormal * 20, "NormalSum", 0, FColor::Green, 10.0f, false, 1);
#endif
    ClimbSurface.NormalDistance = FVector::DotProduct(Capsule->GetComponentLocation() - ClimbSurface.SurfacePoint, ClimbSurface.SurfaceNormal);
    if (ClimbSurface.NormalDistance < 0.0f)
    {
        return false;
    }
    //ClimbSurface.IsClimbableSurface = true;

    return true;
}

void UICCharacterMovementComponent::UpdateClimbingAcceleration()
{
    /*if (!ClimbSurface.IsClimbableSurface)
        return;*/

    Acceleration = FVector::VectorPlaneProject(Acceleration, ClimbSurface.SurfaceNormal);

    Acceleration = Acceleration.GetClampedToMaxSize(GetMaxAcceleration());
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
    if (!ClimbSurface.IsClimbableSurface)
        return;

    // 速度位于墙面切平面
    FVector MoveDelta = FVector::VectorPlaneProject(InVelocity, ClimbSurface.SurfaceNormal);
    const FVector AttachVelocity = ComputeAttachVelocity();

#if WITH_EDITOR
    DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + MoveDelta * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
    DrawDebugString(GetWorld(), UpdatedComponent->GetComponentLocation() + MoveDelta * 20, "MoveDeltaBeforeAttach", 0, FColor::Green, 10.0f, false, 1);
    DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + AttachVelocity * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
    DrawDebugString(GetWorld(), UpdatedComponent->GetComponentLocation() + AttachVelocity * 20, "AttachVelocity", 0, FColor::Green, 10.0f, false, 1);
#endif
    
    MoveDelta = (InVelocity + AttachVelocity) * DeltaSeconds;

#if WITH_EDITOR
    DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + MoveDelta * 35, 1.0f, FColor::Blue, false, 10.0f, 0U, 1.0f);
    DrawDebugString(GetWorld(), UpdatedComponent->GetComponentLocation() + MoveDelta * 20, "MoveDelta", 0, FColor::Green, 10.0f, false, 1);
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
    if(!Hit.IsValidBlockingHit() || !Hit.bBlockingHit)
        return false;

    const FVector Normal = Hit.ImpactNormal.GetSafeNormal();
    if (Normal.IsNearlyZero())
    {
        return false;
    }

    const float NormalDotUp = FMath::Abs(FVector::DotProduct(Normal, UpDirection));
    if (NormalDotUp > 0.5)//cos(倾斜度)
    {
        return false;
    }

    const float FaceDot = FVector::DotProduct(TraceDirection, -Normal);
    if (FaceDot < 0.8)//相似度
    {
        return false;
    }

    //Hit.PhysMaterial;
    return true;
}

TArray<FVector> UICCharacterMovementComponent::GetProbeLocations() const
{
    TArray<FVector> Res;
    switch (ClimbProbeLocType)
    {
    case EClimbProbeLocType::Bone:
        if (UMeshComponent* Mesh = GetOwner()->FindComponentByClass<UMeshComponent>())
        {
            for (const FName& BoneName : ClimbProbeBoneNames)
            {
                if (BoneName.IsNone() || !Mesh->DoesSocketExist(BoneName))
                    continue;

                Res.Add(Mesh->GetSocketLocation(BoneName));
            }
        }
        break;

    case EClimbProbeLocType::ZOffset:
        if (UCapsuleComponent* CapsulComp = GetOwner()->FindComponentByClass<UCapsuleComponent>())
        {
            for (const float ZOffset : ClimbProbeZOffsets)
            {
                if(!FMath::IsNearlyEqual(ZOffset, 0.0f))
                    Res.Add(CapsulComp->GetComponentLocation() + GetGravityDirection() * ZOffset);
            }
        }
        break;
    }

    Res.Add(UpdatedComponent->GetComponentLocation());
    return Res;
}

//void UICCharacterMovementComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//    SetMovementMode(TempMode);
//    MontageEndedDelegate.Unbind();
//}
