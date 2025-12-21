// Copyright DX_IC


#include "Animation/ANS_ListeningWindow.h"
#include "DX_ICPlayerController.h"
#include "Combo/ComboTrie.h"
#include "StructUtils/InstancedStruct.h"


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
	case ETriggerEventType::Combo:
		BindTriggerEventByLitenType(MeshComp, &UANS_ListeningWindow::TryActiveCombo);
		break;
	}

	MeshComp->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &UANS_ListeningWindow::OnMontageInterruptedEvent);
	SaveMeshComp = MeshComp;
}

void UANS_ListeningWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ClearBind();
}

void UANS_ListeningWindow::StopMontageByInput(const FInstancedStruct& Value)
{
	float BlendOutTime = 0.2f;
	SaveMeshComp->GetAnimInstance()->Montage_Stop(BlendOutTime, SaveMeshComp->GetAnimInstance()->GetCurrentActiveMontage());
}

void UANS_ListeningWindow::TryActiveCombo(const FInstancedStruct& ComboInfo)
{
	if (SaveMeshComp->GetOwner())
	{
		UComboTrie* ComboTrie = SaveMeshComp->GetOwner()->FindComponentByClass<UComboTrie>();
		if (!ComboTrie || !ComboInfo.IsValid() || !ComboInfo.GetPtr<FInputComboInfo>())
			return;
		ComboTrie->TryActiveCombo(ComboInfo.Get<FInputComboInfo>().InputComboSource);
	}
}

void UANS_ListeningWindow::OnMontageInterruptedEvent(UAnimMontage* montage, bool bInterrupted)
{
	if (!bInterrupted)
		return;

	ClearBind();
}

void UANS_ListeningWindow::ClearBind()
{
	SaveMeshComp->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &UANS_ListeningWindow::OnMontageInterruptedEvent);

	for (EListenType ListenType : ListenTypes)
	{
		switch (ListenType)
		{
		case EListenType::None: default:
			break;
		case EListenType::MoveInput:
			if (APawn* pawn = Cast<APawn>(SaveMeshComp->GetOwner()))
			{
				if (ADX_ICPlayerController* ICPlayerController = Cast<ADX_ICPlayerController>(pawn->GetController()))
				{
					ICPlayerController->DG_MoveInputTrigger.Unbind();
				}
			}

			break;
		case EListenType::AbilityInput:
			if (UICAbilitySystemComponent* ICASC = SaveMeshComp->GetOwner()->FindComponentByClass<UICAbilitySystemComponent>())
			{
				ICASC->DG_InputComboPress.Unbind();
			}
			break;
		}
	}
}
