// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DX_ICPlayerController.h"
#include "ANS_ListeningWindow.generated.h"


UENUM(BlueprintType)
enum class EListenType : uint8
{
	None,
	AbilityInput,
	MoveInput
};
UENUM(BlueprintType)
enum class ETriggerEventType : uint8
{
	None,
	Interrupt,
	Combo,
	Ability
};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UANS_ListeningWindow : public UAnimNotifyState
{
	GENERATED_BODY()
	

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	EListenType ListenType = EListenType::None;
	UPROPERTY(EditAnywhere)
	ETriggerEventType TriggerEventType = ETriggerEventType::None;

private:
	void StopMontageByInput(const FVector2D& Value);

	UFUNCTION()
	void OnMontageInterruptedEvent(UAnimMontage* montage, bool bInterrupted);

	void ClearBind();

	template<typename TriggerEventFuncType>
	void BindTriggerEventByLitenType(USkeletalMeshComponent* MeshComp, TriggerEventFuncType TriggerEventFunc);

	USkeletalMeshComponent* SafeMeshComp;

};

template<typename TriggerEventFuncType>
inline void UANS_ListeningWindow::BindTriggerEventByLitenType(USkeletalMeshComponent* MeshComp, TriggerEventFuncType TriggerEventFunc)
{
	switch (ListenType)
	{
	case EListenType::None: default:
		break;
	case EListenType::MoveInput:
		if (APawn* pawn = Cast<APawn>(MeshComp->GetOwner()))
		{
			if (ADX_ICPlayerController* ICPlayerController = Cast<ADX_ICPlayerController>(pawn->GetController()))
			{
				ICPlayerController->DG_MoveInputTrigger.BindUObject(this, TriggerEventFunc);
			}
		}
		break;
	}
}
