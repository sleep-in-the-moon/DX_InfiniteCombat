// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "RootMotionModifier_SkewWarp.h"
#include "RootMotionModifier_ICSkewWarp.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API URootMotionModifier_ICSkewWarp : public URootMotionModifier_SkewWarp
{
	GENERATED_BODY()

public:
	/*virtual FTransform ProcessRootMotion(const FTransform& InRootMotion, float DeltaSeconds) override;*/
	virtual void Update(const FMotionWarpingUpdateContext& Context) override;
	
};
