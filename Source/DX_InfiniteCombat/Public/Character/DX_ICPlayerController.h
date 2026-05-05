// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "BasicInputPlayerController.h"
#include "GameplayTagContainer.h"

#include "DX_ICPlayerController.generated.h"


struct FAbilityInput;
struct FInstancedStruct;

DECLARE_DELEGATE_OneParam(FMoveInputTrigger, const FInstancedStruct& /*InputValue*/);

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API ADX_ICPlayerController : public ABasicInputPlayerController
{
	GENERATED_BODY()

public:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	//世界坐标下移动输入的方向向量
	UFUNCTION(BlueprintCallable)
	FVector GetMoveInput() const;

	UFUNCTION(BlueprintCallable)
	FVector GetControllerForwardVector() const;
	UFUNCTION(BlueprintCallable)
	FVector GetControllerRightVector() const;
	UFUNCTION(BlueprintCallable)
	FVector GetControllerUpVector() const;
	UFUNCTION(BlueprintCallable)
	FVector GetControllerHorizontalForwardVector() const;
	UFUNCTION(BlueprintCallable)
	FVector GetControllerHorizontalRightVector() const;

	UPROPERTY(EditAnywhere, Category = "Input|GAS")
	TArray<FAbilityInput> AbilityInputArray;

	FMoveInputTrigger DG_MoveInputTrigger;

protected:
	virtual void BeginPlay() override;

	virtual void MoveEvent(const FInputActionValue& InputValue) override;

};
