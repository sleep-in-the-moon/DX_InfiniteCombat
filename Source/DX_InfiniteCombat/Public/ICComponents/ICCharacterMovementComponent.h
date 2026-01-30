// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ICCharacterMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FTraversalCheckInput
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector TraceDirection=FVector::Zero();
	UPROPERTY(BlueprintReadWrite)
	float TraceDistance=0.0f;
	UPROPERTY(BlueprintReadWrite)
	float MaxLedgeHeight=0.0f;
	UPROPERTY(BlueprintReadWrite)
	float MinLedgeHeight=0.0f;
};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	UAnimMontage* ClimbMontage=nullptr;

public:
	bool TryTraversalAction(const FTraversalCheckInput& CheckInput);

};
