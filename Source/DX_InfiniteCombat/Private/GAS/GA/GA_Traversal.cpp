// Copyright DX_IC


#include "GAS/GA/GA_Traversal.h"
#include "ICComponents/ICCharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "KismetTraceUtils.h"
#include "Subsystem/ICWorldSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/Character.h"

bool UGA_Traversal::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Traversal::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    bool bSuccess = false;
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	
	AActor* const AvatarActor = ActorInfo->AvatarActor.Get();
    UICCharacterMovementComponent* MoveComp = AvatarActor->FindComponentByClass<UICCharacterMovementComponent>();
    if (!MoveComp)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (UCapsuleComponent* Capsule = AvatarActor->FindComponentByClass<UCapsuleComponent>())
    {
        FHitResult OutHit;
        // Start:取胶囊体底部加最高最低的中点Z
        FVector Start = (Capsule->GetComponentLocation() + Capsule->GetUpVector() * ((MoveComp->TraversalCheckInput.MaxLedgeHeight - MoveComp->TraversalCheckInput.MinLedgeHeight) / 2 - Capsule->GetScaledCapsuleHalfHeight()));
        FVector End = Start + MoveComp->TraversalCheckInput.TraceDirection * MoveComp->TraversalCheckInput.TraceDistance;

        //ECollisionChannel CollisionChannel = UDX_StaticFunlib::GetCustomTraceChannelByName(TEXT("Traversable"));
        ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel1;

        FCollisionQueryParams Params = UDX_StaticFunlib::MakeCollisionQueryParam(AvatarActor, TArray<AActor*>(), TEXT("TraversalAction1"));

        EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::None;
        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
        if (ICSubSystem && ICSubSystem->GetShowDebug())
        {
            DebugType = EDrawDebugTrace::Type::ForDuration;
            DrawDebugCapsuleTraceSingle(AvatarActor->GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight(), DebugType, false, OutHit, FLinearColor::Blue, FLinearColor::Green, 4.0f);
        }

        if (CollisionChannel != ECollisionChannel::ECC_MAX &&
            GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight()), Params)
            && !MoveComp->IsWalkable(OutHit) && !OutHit.bStartPenetrating)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction1 Success")));
            UE_LOG(LogTemp, Warning, TEXT("TraversalAction1 Success"));


            End = FVector(OutHit.ImpactPoint.X, OutHit.ImpactPoint.Y, (Capsule->GetComponentLocation() + Capsule->GetUpVector() * -Capsule->GetScaledCapsuleHalfHeight()).Z);
            End += OutHit.ImpactNormal * -Capsule->GetScaledCapsuleRadius() / 2;
            Start = End + FVector(0, 0, MoveComp->TraversalCheckInput.MaxLedgeHeight + 1);
            FHitResult OutHit2;

            if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(CollisionChannel), false
                , TArray<AActor*>(), DebugType, OutHit2, true, FLinearColor::Blue, FLinearColor::Green, 4.0f) && MoveComp->IsWalkable(OutHit2))//OutHit2::手抓点
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

                    DrawDebugPoint(GetWorld(), OutHit2.ImpactPoint, 20, FColor::Black, false, 3);
                    DrawDebugPoint(GetWorld(), TargetTransform.GetLocation(), 20, FColor::Green, false, 3);

                    float Height = TargetTransform.GetLocation().Z - AvatarActor->GetActorLocation().Z;

                    // MotionWarping  PlayMontage
                    UMotionWarpingComponent* MotionWarpingComp = AvatarActor->FindComponentByClass<UMotionWarpingComponent>();

                    if (MotionWarpingComp && MoveComp->TraversalMontage)
                    {
                        MotionWarpingComp->AddOrUpdateWarpTargetFromTransform(TEXT("TraversalHandPointZ"), FTransform(FRotator(0, -172, 0), FVector(Capsule->GetComponentLocation().X, Capsule->GetComponentLocation().Y, OutHit2.ImpactPoint.Z)));
                        //MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(TEXT("TraversalHandPoint"), OutHit2.ImpactPoint);
                        //MotionWarpingComp->AddOrUpdateWarpTargetFromTransform(TEXT("TraversalHandPoint"), FTransform(AvatarActor->GetActorRotation(), OutHit2.ImpactPoint));

                        //MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(TEXT("TraversalStandPoint"), FVector(FVector2D(TargetTransform.GetLocation()), TargetTransform.GetLocation().Z+2));
                        MotionWarpingComp->AddOrUpdateWarpTargetFromTransform(TEXT("TraversalStandPoint"), FTransform(FRotator(0, -172, 0), TargetTransform.GetLocation()));

                        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("TraversalMontage"), MoveComp->TraversalMontage, false);
                       
                        MontageTask->Activate();
                        MontageTask->OnCompleted.AddUniqueDynamic(this, &UGA_Traversal::TraversalEnd);
                        MontageTask->OnInterrupted.AddUniqueDynamic(this, &UGA_Traversal::TraversalEnd);

                        RawMovementMode = MoveComp->MovementMode;
                        RawCustomMovementMode = MoveComp->CustomMovementMode;
                        MoveComp->SetMovementMode(EMovementMode::MOVE_Custom, static_cast<uint8>(ECusMovementMode::MOVE_Traversal));

                        bSuccess = true;
                    }

                }
            }
        }
    }

	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    if (!bSuccess)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
}
        

void UGA_Traversal::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Traversal::TraversalEnd()
{
    if (UICCharacterMovementComponent* MoveComp = GetAvatarActorFromActorInfo()->FindComponentByClass<UICCharacterMovementComponent>())
    {
        MoveComp->SetMovementMode(RawMovementMode, RawCustomMovementMode);
    }

	K2_EndAbility();
}
