// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "RootMotionModifier.h"
#include "RootMotionModifier_Zero.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Zero"))
class DX_INFINITECOMBAT_API URootMotionModifier_Zero : public URootMotionModifier
{
	GENERATED_BODY()
	
public:
	virtual FTransform ProcessRootMotion(const FTransform& InRootMotion, float DeltaSeconds) override
	{
		return FTransform::Identity;
	}

};
