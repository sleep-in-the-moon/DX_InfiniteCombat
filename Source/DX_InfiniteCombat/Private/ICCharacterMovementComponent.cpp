// Copyright DX_IC


#include "ICCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "KismetTraceUtils.h"
#include "ICWorldSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/Character.h"

bool UICCharacterMovementComponent::TryTraversalAction(const FTraversalCheckInput& CheckInput)
{
    if (UCapsuleComponent* Capsule = GetOwner()->FindComponentByClass<UCapsuleComponent>())
    {
        FHitResult OutHit;
        // Start:取胶囊体底部加最高最低的中点Z
        FVector Start = (Capsule->GetComponentLocation() + Capsule->GetUpVector()* ((CheckInput.MaxLedgeHeight - CheckInput.MinLedgeHeight)/2 - Capsule->GetScaledCapsuleHalfHeight()));
        FVector End = Start + CheckInput.TraceDirection* CheckInput.TraceDistance;

        //ECollisionChannel CollisionChannel = UDX_StaticFunlib::GetCustomTraceChannelByName(TEXT("Traversable"));
        ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel1;
        
        FCollisionQueryParams Params = UDX_StaticFunlib::MakeCollisionQueryParam(GetOwner(), TArray<AActor*>(), TEXT("TraversalAction1"));

        EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::None;
        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
        if (ICSubSystem && ICSubSystem->GetShowDebug())
        {
            DebugType = EDrawDebugTrace::Type::ForDuration;
            DrawDebugCapsuleTraceSingle(GetOwner()->GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight(), DebugType, false, OutHit, FLinearColor::Blue, FLinearColor::Green, 4.0f);
        }

        if (CollisionChannel != ECollisionChannel::ECC_MAX && 
            GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight()), Params)
            && !IsWalkable(OutHit) && !OutHit.bStartPenetrating)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction1 Success")));
            UE_LOG(LogTemp, Warning, TEXT("TraversalAction1 Success"));


            End = FVector(OutHit.ImpactPoint.X, OutHit.ImpactPoint.Y, (Capsule->GetComponentLocation() + Capsule->GetUpVector()*-Capsule->GetScaledCapsuleHalfHeight()).Z);
            End += OutHit.ImpactNormal * -Capsule->GetScaledCapsuleRadius() / 2;
            Start = End + FVector(0, 0, CheckInput.MaxLedgeHeight+1);
            FHitResult OutHit2;

            if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(CollisionChannel), false
                , TArray<AActor*>(), DebugType, OutHit2, true, FLinearColor::Blue, FLinearColor::Green, 4.0f) && IsWalkable(OutHit2))
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction2 Success")));
                UE_LOG(LogTemp, Warning, TEXT("TraversalAction2 Success"));


                Start = FVector(OutHit2.Location.X, OutHit2.Location.Y, OutHit2.ImpactPoint.Z +
                        Capsule->GetScaledCapsuleHalfHeight() + Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere());
                End = FVector(OutHit2.Location.X, OutHit2.Location.Y, OutHit2.ImpactPoint.Z +
                        Capsule->GetScaledCapsuleHalfHeight() - Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere());
                FHitResult OutHit3;

                if (!(UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(CollisionChannel), false
                    , TArray<AActor*>(), DebugType, OutHit3, true, FLinearColor::Red, FLinearColor::Yellow, 4.0f) || OutHit3.bStartPenetrating))
                {
                    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction3 Success")));
                    UE_LOG(LogTemp, Warning, TEXT("TraversalAction3 Success"));

                    FTransform TargetTransform = FTransform((OutHit.ImpactNormal * FVector(-1, -1, 0)).ToOrientationRotator(),
                        FVector(OutHit2.Location.X, OutHit2.Location.Y, OutHit2.ImpactPoint.Z + Capsule->GetScaledCapsuleHalfHeight()));

                    DrawDebugPoint(GetWorld(), TargetTransform.GetLocation(), 20, FColor::Green, false, 3);

                    float Height = TargetTransform.GetLocation().Z - GetOwner()->GetActorLocation().Z;

                    // MotionWarping  PlayMontage
                    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
                    UMotionWarpingComponent* MotionWarpingComp = GetOwner()->FindComponentByClass<UMotionWarpingComponent>();

                    if (MotionWarpingComp && OwnerCharacter && ClimbMontage)
                    {
                        MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(TEXT("ClimbHandPoint"), OutHit2.ImpactPoint);
                        MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(TEXT("ClimbStandPoint"), TargetTransform.GetLocation());
                        OwnerCharacter->PlayAnimMontage(ClimbMontage);
                    }

                    return true;
                }
            }
        }
    }

    return false;
}
