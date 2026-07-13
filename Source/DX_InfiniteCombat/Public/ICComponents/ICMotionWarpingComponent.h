// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "ICMotionWarpingComponent.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICMotionWarpingComponent : public UMotionWarpingComponent
{
	GENERATED_BODY()
	
public:
	static FTransform CalculateRootTransformRelativeToWarpPointAtTime(const UMotionWarpingBaseAdapter& WarpingAdapter, const UAnimSequenceBase* Animation, float Time, const FName& WarpPointBoneName);
};
