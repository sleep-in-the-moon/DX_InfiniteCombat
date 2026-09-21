// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_TurnInplace.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UANS_TurnInplace : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	FQuat StartQuat;
	FQuat TargetQuat;
	float TotalSeconds = 0.0f;
	float CurSeconds = 0.0f;
};
