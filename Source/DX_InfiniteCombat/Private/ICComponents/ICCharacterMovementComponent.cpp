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
    return (MovementMode == MOVE_Custom) && (CustomMovementMode == static_cast<uint8>(ECusMovementMode::MOVE_Climb)) && UpdatedComponent;
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

        //FClimbSurfaceInfo ClimbSurface;
        if (!FindClimbSurface(ClimbSurface))
        {
            SetMovementMode(MOVE_Falling);
            StartNewPhysics(remainingTime + TimeStep, Iterations-1);
            return;
        }
        //UpdateClimbSurface(ClimbSurface);

        const FVector OldLocation = UpdatedComponent->GetComponentLocation();

        //将上一次模拟时叠加的叠加型 RootMotion 清除，避免重复叠加
        RestorePreAdditiveRootMotionVelocity();

        UpdateClimbingAcceleration();

        if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
        {
            CalcVelocity(TimeStep, ClimbFriction, false, ClimbBrakingDeceleration);
        }

        //应用 RootMotion，动画 RootMotion 或 非动画 RootMotion: 覆盖型和叠加型
        ApplyRootMotionToVelocity(TimeStep);

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

    if (IsClimbing())
    {
        Velocity = FVector::VectorPlaneProject(Velocity, ClimbSurface.SurfaceNormal);
    }
}

bool UICCharacterMovementComponent::FindClimbSurface(FClimbSurfaceInfo& OutSurface)
{
    return false;
}

void UICCharacterMovementComponent::UpdateClimbingAcceleration()
{
    /*if (!ClimbSurface.IsClimbableSurface)
        return;*/

    Acceleration = FVector::VectorPlaneProject(Acceleration, ClimbSurface.SurfaceNormal);

    Acceleration = Acceleration.GetClampedToMaxSize(GetMaxAcceleration());
}

FVector UICCharacterMovementComponent::ComputeAttachVelocity(const FClimbSurfaceInfo& InSurface)
{
    const FVector CurLocation = UpdatedComponent->GetComponentLocation();

    ClimbSurface.NormalDistance = FVector::DotProduct(CurLocation - ClimbSurface.SurfacePoint, ClimbSurface.SurfaceNormal);

    return FVector();
}

void UICCharacterMovementComponent::ClimbAlongSurface(const FVector& InVelocity, float DeltaSeconds, float InRemainingTime, int32 InIterations)
{
    if (!ClimbSurface.IsClimbableSurface)
        return;

    // 速度位于墙面切平面
    FVector MoveDelta = FVector::VectorPlaneProject(InVelocity, ClimbSurface.SurfaceNormal);

    const FVector AttachVelocity = ComputeAttachVelocity(ClimbSurface);
    MoveDelta = (InVelocity + AttachVelocity) * DeltaSeconds;

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
    return FQuat();
}

//void UICCharacterMovementComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//    SetMovementMode(TempMode);
//    MontageEndedDelegate.Unbind();
//}
