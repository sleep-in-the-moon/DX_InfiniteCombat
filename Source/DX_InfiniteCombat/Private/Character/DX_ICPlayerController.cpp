// Copyright DX_IC


#include "Character/DX_ICPlayerController.h"
#include "Character/DX_ICCharacter.h"
#include "GAS/ICAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "StructUtils/InstancedStruct.h"


void ADX_ICPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (ADX_ICCharacter* ICCharacter = Cast<ADX_ICCharacter>(GetPawn()))
	{
		if (UICAbilitySystemComponent* ASC = ICCharacter->FindComponentByClass<UICAbilitySystemComponent>())
		{
			ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
		}
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

FVector ADX_ICPlayerController::GetMoveInput() const
{
	return GetControllerHorizontalForwardVector() * MoveInputVector.X +
		   GetControllerHorizontalRightVector() * MoveInputVector.Y;
}

FVector ADX_ICPlayerController::GetControllerForwardVector() const
{
	return FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
}

FVector ADX_ICPlayerController::GetControllerRightVector() const
{
	return FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y);
}

FVector ADX_ICPlayerController::GetControllerUpVector() const
{
	return FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Z);
}

FVector ADX_ICPlayerController::GetControllerHorizontalForwardVector() const
{
	FRotator HorizontalRotation(0, GetControlRotation().Yaw, 0);
	return FRotationMatrix(HorizontalRotation).GetUnitAxis(EAxis::X);
}

FVector ADX_ICPlayerController::GetControllerHorizontalRightVector() const
{
	FRotator HorizontalRotation(0, GetControlRotation().Yaw, 0);
	return FRotationMatrix(HorizontalRotation).GetUnitAxis(EAxis::Y);
}

void ADX_ICPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADX_ICPlayerController::MoveEvent(const FInputActionValue& InputValue)
{ 
	Super::MoveEvent(InputValue);
	//ACharacter* character = GetCharacter();
	//if (!IgnoreMoveInput && character)
	//{
	//	UAbilitySystemComponent* ASC = character->FindComponentByClass<UAbilitySystemComponent>();
	//	if (ASC && (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InCombo"))) ))//|| ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InDodge")))))
	//	{
	//		character->StopAnimMontage();
	//	}
	//}

	DG_MoveInputTrigger.ExecuteIfBound(FInstancedStruct::Make(MoveInputVector));
}

