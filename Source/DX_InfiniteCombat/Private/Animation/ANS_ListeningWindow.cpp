// Copyright DX_IC


#include "Animation/ANS_ListeningWindow.h"
#include "DX_ICPlayerController.h"

void UANS_ListeningWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

	switch (TriggerEventType)
	{
	case ETriggerEventType::None: default:
		break;
	case ETriggerEventType::Interrupt:
		//StopMontage
		//调用函数模板，传入DG需要绑定的Func，函数模板中SwitchListenType
		BindTriggerEventByLitenType(MeshComp, &UANS_ListeningWindow::StopMontageByInput);
		break;
	}
	

	MeshComp->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &UANS_ListeningWindow::OnMontageInterruptedEvent);
	SafeMeshComp = MeshComp;
}

void UANS_ListeningWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ClearBind();
}

void UANS_ListeningWindow::StopMontageByInput(const FVector2D& Value)
{
	float BlendOutTime = 0.2f;
	SafeMeshComp->GetAnimInstance()->Montage_Stop(BlendOutTime, SafeMeshComp->GetAnimInstance()->GetCurrentActiveMontage());
}

void UANS_ListeningWindow::OnMontageInterruptedEvent(UAnimMontage* montage, bool bInterrupted)
{
	if (!bInterrupted)
		return;

	ClearBind();
}

void UANS_ListeningWindow::ClearBind()
{
	SafeMeshComp->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &UANS_ListeningWindow::OnMontageInterruptedEvent);

	switch (ListenType)
	{
	case EListenType::None: default:
		break;
	case EListenType::MoveInput:
		;
		if (APawn* pawn = Cast<APawn>(SafeMeshComp->GetOwner()))
		{
			if (ADX_ICPlayerController* ICPlayerController = Cast<ADX_ICPlayerController>(pawn->GetController()))
			{
				ICPlayerController->DG_MoveInputTrigger.Unbind();
			}
		}

		break;
	}
}
