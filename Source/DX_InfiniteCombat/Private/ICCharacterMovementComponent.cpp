// Copyright DX_IC


#include "ICCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "KismetTraceUtils.h"
#include "ICWorldSubsystem.h"

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

        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
        if (ICSubSystem && ICSubSystem->GetShowDebug())
            DrawDebugCapsuleTraceSingle(GetOwner()->GetWorld(), Start, End, Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight(), EDrawDebugTrace::Type::ForDuration, false, OutHit, FLinearColor::Blue, FLinearColor::Green, 5.0f);

        if (CollisionChannel != ECollisionChannel::ECC_MAX && 
            GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight()), Params))
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("TraversalAction1 Success")));
            UE_LOG(LogTemp, Warning, TEXT("TraversalAction1 Success"));
        }
    }

    return false;
}
