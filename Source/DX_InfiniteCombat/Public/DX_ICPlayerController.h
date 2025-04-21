// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "BasicInputPlayerController.h"
#include "GameplayTagContainer.h"

#include "DX_ICPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UInputAction* InputAction;

	UPROPERTY(EditAnywhere)
	FGameplayTag InputTag;

};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API ADX_ICPlayerController : public ABasicInputPlayerController
{
	GENERATED_BODY()

public:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<FAbilityInput> AbilityInputArray;

protected:
	virtual void BeginPlay() override;

	virtual void MoveEvent(const FInputActionValue& InputValue) override;
	virtual void Jump() override;
	//virtual void StopJump() override;

};
