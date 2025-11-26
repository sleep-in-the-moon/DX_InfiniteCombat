// Copyright DX_IC


#include "LockSystem/WeakLockComponent.h"
#include "ICWorldSubsystem.h"
#include "KismetTraceUtils.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "UMG/WidgetCombatStates.h"

// Sets default values for this component's properties
UWeakLockComponent::UWeakLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeakLockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeakLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bControllerFollow && GetOwner() && LockActor)
	{
		if (APawn* ownerP = Cast<APawn>(GetOwner()))
		{
			FRotator LookRot = FRotationMatrix::MakeFromX(LockActor->GetActorLocation() - GetOwner()->GetActorLocation()).Rotator();
			ownerP->GetController()->SetControlRotation(FMath::RInterpTo(ownerP->GetController()->GetControlRotation()
				, FRotator(LookRot.Pitch, LookRot.Yaw, ownerP->GetController()->GetControlRotation().Roll), DeltaTime, 10.0f));
		}
	}
}

void UWeakLockComponent::Trigger()
{
	if (DoOnceTrace())
	{
		bControllerFollow = true;
		GetWorld()->GetTimerManager().SetTimer(ControllerTimer, [this]() {
			bControllerFollow = false;
			//Widget
		}, false, ControllerFollowTime);

		FRotator LookRot = FRotationMatrix::MakeFromX(LockActor->GetActorLocation() - GetOwner()->GetActorLocation()).Rotator();
		GetOwner()->SetActorRotation(FRotator(GetOwner()->GetActorRotation().Pitch, LookRot.Yaw, GetOwner()->GetActorRotation().Roll));
	}
}

bool UWeakLockComponent::DoOnceTrace()
{
	TArray<FHitResult> OutHits;
	float TraceHalf = 500.f, TraceDist = 7000.f;

	if (APawn* pawn = Cast<APawn>(GetOwner()))
	{
		APlayerController* playerController = Cast<APlayerController>(pawn->GetController());
		if (!playerController)
			return false;

		FVector StartLoc = playerController->PlayerCameraManager->GetCameraLocation();
		FRotator CameraYaw = FRotator(0, playerController->PlayerCameraManager->GetCameraRotation().Yaw, 0);
		FVector EndLoc = StartLoc + FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::X) * TraceDist;

		FCollisionShape TraceShape = FCollisionShape::MakeSphere(TraceHalf);

		UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
		if (ICSubSystem && ICSubSystem->GetShowDebug())
			DrawDebugSphereTraceMulti(GetOwner()->GetWorld(), StartLoc, EndLoc, TraceHalf, EDrawDebugTrace::Type::ForDuration, false, OutHits, FLinearColor::Blue, FLinearColor::Green, 10.0f);

		FCollisionQueryParams CollisionQueryParams;
		FCollisionObjectQueryParams CollisionObjectQueryParams;
		UDX_StaticFunlib::MakeCollisionParam(GetOwner(), CollisionQueryParams, CollisionObjectQueryParams, TArray<AActor*>(), TraceObjectTypes);

		bool const bHit = GetOwner()->GetWorld()->SweepMultiByObjectType(OutHits, StartLoc, EndLoc, FQuat::Identity, CollisionObjectQueryParams, TraceShape, CollisionQueryParams);

		if (bHit)
		{
			LockActor = OutHits[0].GetActor();
		}

		return bHit;
	}

	return false;
}
