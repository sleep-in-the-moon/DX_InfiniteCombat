// Copyright DX_IC


#include "LockSystem/WeakLockComponent.h"
#include "ICWorldSubsystem.h"
#include "KismetTraceUtils.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "UMG/WidgetCombatStates.h"
#include "Kismet/GameplayStatics.h"

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

	if (bControllerFollow && GetOwner() && LockActor && GetOwnerController())
	{
		/*if (APawn* ownerP = Cast<APawn>(GetOwner()))
		{
			FRotator LookRot = FRotationMatrix::MakeFromX(LockActor->GetActorLocation() - GetOwner()->GetActorLocation()).Rotator();
			ownerP->GetController()->SetControlRotation(FMath::RInterpTo(ownerP->GetController()->GetControlRotation()
				, FRotator(LookRot.Pitch, LookRot.Yaw, ownerP->GetController()->GetControlRotation().Roll), DeltaTime, 10.0f));
		}*/

		FVector2D ScreenPos;
		UGameplayStatics::ProjectWorldToScreen(GetOwnerController(), LockActor->GetActorLocation(), ScreenPos);

		int32 SizeX, SizeY;
		GetOwnerController()->GetViewportSize(SizeX, SizeY);

		bool bOutOfView =
			ScreenPos.X < 0 || ScreenPos.X > SizeX ||
			ScreenPos.Y < 0 || ScreenPos.Y > SizeY ;

		if (bOutOfView)
		{
			FVector2D HalfFOV = GetHalfFOV_VH();
			FVector2D Margin = GetMargin_VH();

			float allowedYaw = HalfFOV.Y - Margin.Y;
			float allowedPitch = HalfFOV.X - Margin.X;

			FVector2D Delta = GetDeltaYawPitch();
			float neededYaw = 0.f;
			if (Delta.X > allowedYaw) neededYaw = Delta.X - allowedYaw;
			else if (Delta.X < -allowedYaw) neededYaw = Delta.X + allowedYaw;

			float neededPitch = 0.f;
			if (Delta.Y > allowedPitch) neededPitch = Delta.Y - allowedPitch;
			else if (Delta.Y < -allowedPitch) neededPitch = Delta.Y + allowedPitch;

			FRotator DesiredRot = GetOwnerController()->GetControlRotation();
			DesiredRot.Yaw += neededYaw;
			DesiredRot.Pitch += neededPitch;

			GetOwnerController()->SetControlRotation(FMath::RInterpTo(GetOwnerController()->GetControlRotation()
				, DesiredRot, DeltaTime, 10.0f));
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
			LockActor = nullptr;
			//Widget
		}, ControllerFollowTime, false);

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

APlayerController* UWeakLockComponent::GetOwnerController()
{
	if (APawn* pawn = Cast<APawn>(GetOwner()))
	{
		return Cast<APlayerController>(pawn->GetController());
	}
	return nullptr;
}

FVector2D UWeakLockComponent::GetDeltaYawPitch()
{
	if (LockActor && GetOwnerController())
	{
		FRotator LookAtRot = (LockActor->GetActorLocation() - GetOwnerController()->PlayerCameraManager->GetCameraLocation()).GetSafeNormal().Rotation();
		FRotator DeltaRot = LookAtRot - GetOwnerController()->PlayerCameraManager->GetCameraRotation();
		DeltaRot.Normalize();

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("DeltaYaw::%f, DeltaPitch::%f"), DeltaRot.Yaw, DeltaRot.Pitch));
		UE_LOG(LogTemp, Warning, TEXT("DeltaYaw::%f, DeltaPitch::%f"), DeltaRot.Yaw, DeltaRot.Pitch);

		return FVector2D(DeltaRot.Yaw, DeltaRot.Pitch);
	}

	return FVector2D();
}

FVector2D UWeakLockComponent::GetHalfFOV_VH()
{
	if (GetOwnerController())
	{
		float halfHFOV = GetOwnerController()->PlayerCameraManager->GetCameraCacheView().FOV * 0.5f;
		float aspect = GetOwnerController()->PlayerCameraManager->GetCameraCacheView().AspectRatio;
		float halfVFOV = FMath::RadiansToDegrees(FMath::Atan(FMath::Tan(FMath::DegreesToRadians(halfHFOV)* aspect)));

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("halfVFOV::%f, halfHFOV::%f"), halfVFOV, halfHFOV));
		UE_LOG(LogTemp, Warning, TEXT("halfVFOV::%f, halfHFOV::%f"), halfVFOV, halfHFOV);

		return FVector2D(halfVFOV, halfHFOV);
	}

	return FVector2D();
}

FVector2D UWeakLockComponent::GetMargin_VH()
{
	int32 ViewX = 0, ViewY = 0;
	if (GetOwnerController())
	{
		FVector2D HalfFOV = GetHalfFOV_VH();
		GetOwnerController()->GetViewportSize(ViewX, ViewY);
		float marginH = (MarginPx / ViewX)*(HalfFOV.Y*2);
		float marginV = (MarginPx / ViewY)*(HalfFOV.X*2);

		return FVector2D(marginV, marginH);
	}
	
	return FVector2D();
}
