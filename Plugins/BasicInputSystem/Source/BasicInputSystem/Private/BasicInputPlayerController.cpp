// Copyright DX_IC


#include "BasicInputPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void ABasicInputPlayerController::MoveEvent(const FInputActionValue& InputValue)
{
	MoveInputVector = InputValue.Get<FVector2D>();
	
	FRotator ControllRot = GetControlRotation();
	FRotator ControllRotYaw = FRotator(0, ControllRot.Yaw, 0);

	FVector RightVec = FRotationMatrix(ControllRotYaw).GetUnitAxis(EAxis::Y);
	FVector ForwardVec = FRotationMatrix(ControllRotYaw).GetUnitAxis(EAxis::X);

	if (APawn* pawn = GetPawn<APawn>())
	{
		if (MoveInputVector.Y != 0.0f)
			pawn->AddMovementInput(RightVec * MoveInputVector.Y);

		if (MoveInputVector.X != 0.0f)
			pawn->AddMovementInput(ForwardVec * MoveInputVector.X);

	}
}

void ABasicInputPlayerController::LookEvent(const FInputActionValue& InputValue)
{
	FVector2D value = InputValue.Get<FVector2D>();

	if (APawn* pawn = GetPawn<APawn>())
	{
		pawn->AddControllerPitchInput(value.Y);
		pawn->AddControllerYawInput(value.X);
	}
}

void ABasicInputPlayerController::Jump()
{
	//TODO:优化
	if (ACharacter* pawn = GetPawn<ACharacter>())
	{
		pawn->Jump();
	}
}

void ABasicInputPlayerController::StopJump()
{
	//TODO:优化
	if (ACharacter* pawn = GetPawn<ACharacter>())
	{
		pawn->StopJumping();
	}
}

void ABasicInputPlayerController::SetupInputComponent()
{
	
}

void ABasicInputPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//TODO:输入绑定时机
	if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSystem->AddMappingContext(DefaultInputMapping, 0);
		InputSystem->GetUserSettings()->RegisterInputMappingContext(DefaultInputMapping);
	}

	if (APawn* pawn = GetPawn<APawn>())
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(pawn->InputComponent))
		{
			if (JumpAction)
			{
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasicInputPlayerController::Jump);
				EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasicInputPlayerController::StopJump);
			}
			if (MoveAction && LookAction)
			{
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasicInputPlayerController::MoveEvent);
				EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasicInputPlayerController::LookEvent);
			}
			
		}

}
