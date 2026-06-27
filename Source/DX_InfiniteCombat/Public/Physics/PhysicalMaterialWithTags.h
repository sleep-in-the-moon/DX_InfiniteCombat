// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterialWithTags.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Tags;

};
