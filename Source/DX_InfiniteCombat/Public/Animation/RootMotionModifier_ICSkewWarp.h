// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "RootMotionModifier_SkewWarp.h"
#include "RootMotionModifier_ICSkewWarp.generated.h"

/**
 * 修复了原 SkewWarp 骨骼 Provider 位置异常的 bug
 */
UCLASS(meta = (DisplayName = "IC Skew Warp"))
class DX_INFINITECOMBAT_API URootMotionModifier_ICSkewWarp : public URootMotionModifier_SkewWarp
{
	GENERATED_BODY()

public:
	/*virtual FTransform ProcessRootMotion(const FTransform& InRootMotion, float DeltaSeconds) override;*/
	virtual void Update(const FMotionWarpingUpdateContext& Context) override;
	
};
