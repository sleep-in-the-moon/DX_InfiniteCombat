// Copyright DX_IC


#include "Character/DXCharacterExtensionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Utils/TimelineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UDXCharacterExtensionComponent::UDXCharacterExtensionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDXCharacterExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	TimelineUtils::InitTimelineByFloatCurve(TL_LerpSpringArm, TimelineUtils::MakeFloatCurve(), TEXT("LerpSpringArmInter"), TEXT(""), this);
	TimelineUtils::InitTimelineByFloatCurve(TL_LerpActorRot, TimelineUtils::MakeFloatCurve(), TEXT("LerpActorRotInter"), TEXT("LerpActorRotFinish"), this);
	
}


// Called every frame
void UDXCharacterExtensionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TL_LerpSpringArm.TickTimeline(DeltaTime);
	TL_LerpActorRot.TickTimeline(DeltaTime);
}

void UDXCharacterExtensionComponent::LerpSpringArmEndOffsetToTarget(const FVector& TargetOffset, float Time)
{
	if (USpringArmComponent* SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>())
	{
		if (Time > 0.0f)
		{
			RawSpringOffset = SpringArm->SocketOffset;
			TargetSpringOffset = TargetOffset;

			TL_LerpSpringArm.Stop();
			TL_LerpSpringArm.SetPlayRate(1.0f / Time);
			TL_LerpSpringArm.PlayFromStart();
		}
		else
		{
			SpringArm->SocketOffset = TargetOffset;
		}
	}
}

void UDXCharacterExtensionComponent::LerpSpringArmInter(float Alpha)
{
	if (USpringArmComponent* SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>())
	{
		SpringArm->SocketOffset = UKismetMathLibrary::VLerp(RawSpringOffset, TargetSpringOffset, Alpha);
	}
}

void UDXCharacterExtensionComponent::LerpActorRotToControlRot(float Time, bool JustYaw)
{
	if (APawn* pawn = Cast<APawn>(GetOwner()))
	{
		if (Time > 0.0f)
		{
			RawActorRot = pawn->GetActorRotation();
			bJustYaw = JustYaw;

			TL_LerpActorRot.Stop();
			TL_LerpActorRot.SetPlayRate(1.0f / Time);
			TL_LerpActorRot.PlayFromStart();
		}
		else
		{
			pawn->SetActorRotation(JustYaw ? 
				FRotator(pawn->GetActorRotation().Pitch, pawn->GetControlRotation().Yaw, pawn->GetActorRotation().Roll) : pawn->GetControlRotation());
		}
	}
}

void UDXCharacterExtensionComponent::SetYawByControl(bool YawByControl)
{
	if (UCharacterMovementComponent* CharacterMove = GetOwner()->FindComponentByClass<UCharacterMovementComponent>())
	{
		CharacterMove->bOrientRotationToMovement = !YawByControl;
	}
	if (APawn* pawn = Cast<APawn>(GetOwner()))
	{
		pawn->bUseControllerRotationYaw = YawByControl;
	}
}

void UDXCharacterExtensionComponent::StopTimeline()
{
	TL_LerpSpringArm.Stop();
	TL_LerpActorRot.Stop();
}

void UDXCharacterExtensionComponent::LerpActorRotInter(float Alpha)
{
	if (APawn* pawn = Cast<APawn>(GetOwner()))
		TargetActorRot = bJustYaw ?
			FRotator(pawn->GetActorRotation().Pitch, pawn->GetControlRotation().Yaw, pawn->GetActorRotation().Roll) : pawn->GetControlRotation();

	GetOwner()->SetActorRotation(UKismetMathLibrary::RLerp(RawActorRot, TargetActorRot, Alpha, true));
}

void UDXCharacterExtensionComponent::LerpActorRotFinish()
{
	DG_LerpToControlRotFinish.Broadcast();
}
