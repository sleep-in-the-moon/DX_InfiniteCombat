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
	FVector TraceDirection;
	UPROPERTY(BlueprintReadWrite)
	float TraceDistance;
	UPROPERTY(BlueprintReadWrite)
	float MaxLedgeHeight;
	UPROPERTY(BlueprintReadWrite)
	float MinLedgeHeight;
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
