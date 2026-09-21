// Copyright DX_IC


#include "Animation/ANS_TurnInplace.h"
#include "BasicInputSystem/Public/BasicInputPlayerController.h"
#include "GameFramework/Character.h"

void UANS_TurnInplace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if(!MeshComp->GetOwner())
		return;

	StartQuat = MeshComp->GetOwner()->GetActorQuat();
	TargetQuat = MeshComp->GetOwner()->GetActorQuat();
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (ABasicInputPlayerController* BasicController = Cast<ABasicInputPlayerController>(OwnerCharacter->GetController()))
		{
			TargetQuat = BasicController->MoveInputLastVelocity.Rotation().Quaternion();
		}
	}

	TotalSeconds = TotalDuration;
	CurSeconds = 0.0f;
}

void UANS_TurnInplace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!MeshComp->GetOwner())
		return;

	if (TotalSeconds <= KINDA_SMALL_NUMBER)
		return;

	CurSeconds += FrameDeltaTime;

	float Alpha = FMath::Clamp(CurSeconds/ TotalSeconds, 0.0, 1.0);

	MeshComp->GetOwner()->SetActorRotation(FQuat::Slerp(StartQuat, TargetQuat, Alpha));
}

void UANS_TurnInplace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp->GetOwner())
		return;
}
