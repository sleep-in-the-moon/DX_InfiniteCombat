// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicInputPlayerController.generated.h"


class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 *
 */
UCLASS()
class BASICINPUTSYSTEM_API ABasicInputPlayerController : public APlayerController
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultInputMapping;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	UPROPERTY(BlueprintReadOnly)
	FVector2D MoveInputVector;

	//可能会被 SaveGame 覆盖
	UPROPERTY(BlueprintReadWrite)
	float VerticalSensitivity = 1.0f;
	//可能会被 SaveGame 覆盖
	UPROPERTY(BlueprintReadWrite)
	float HorizontalSensitivity = 1.0f;

protected:
	virtual void MoveEvent(const FInputActionValue& InputValue);
	void LookEvent(const FInputActionValue& InputValue);

	void CharacterStartJump();
	void CharacterStopJump();

	virtual void SetupInputComponent() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
