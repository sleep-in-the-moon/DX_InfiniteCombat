// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "RootMotionModifier.h"
#include "RootMotionModifier_MoveInput.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "MoveInputDirect"))
class DX_INFINITECOMBAT_API URootMotionModifier_MoveInput : public URootMotionModifier
{
	GENERATED_BODY()
	
public:
	virtual FTransform ProcessRootMotion(const FTransform& InRootMotion, float DeltaSeconds) override;
};
