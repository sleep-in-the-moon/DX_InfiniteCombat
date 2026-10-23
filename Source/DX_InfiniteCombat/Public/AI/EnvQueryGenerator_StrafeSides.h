// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "EnvQueryGenerator_StrafeSides.generated.h"


class UEnvQueryContext;
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UEnvQueryGenerator_StrafeSides : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()
	
public:
	UEnvQueryGenerator_StrafeSides(const FObjectInitializer& ObjectInitializer);

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Generator")
	TSubclassOf<UEnvQueryContext> StrafeSidesCenterActorContext;

	UPROPERTY(EditDefaultsOnly, Category = "Generator", meta = (ClampMin="0.0"))
	float StrafeDistance = 200.0f;

};
