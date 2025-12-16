// Copyright DX_IC


#include "DX_ICPlayerController.h"
#include "DX_ICCharacter.h"
#include "ICAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "ICCharacterMovementComponent.h"


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
	return ((FRotator(0, 0, GetControlRotation().Yaw).Vector() * InputVector.X +
	FRotationMatrix(FRotator(0, 0, GetControlRotation().Yaw)).GetScaledAxis(EAxis::Y) * InputVector.Y)).GetSafeNormal(0.0001);
}

void ADX_ICPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADX_ICPlayerController::MoveEvent(const FInputActionValue& InputValue)
{ 
	Super::MoveEvent(InputValue);
	ACharacter* character = GetCharacter();
	if (!IgnoreMoveInput && character)
	{
		UAbilitySystemComponent* ASC = character->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC && (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InCombo"))) ))//|| ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InDodge")))))
		{
			character->StopAnimMontage();
		}
	}

	DG_MoveInputTrigger.ExecuteIfBound(FInstancedStruct::Make(InputVector));
}

void ADX_ICPlayerController::Jump()
{
	Super::Jump();
	if (ACharacter* character = GetCharacter())
	{
		UAbilitySystemComponent* ASC = character->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC && !ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir"))))
		{
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir")));
		}

		UICCharacterMovementComponent* MoveComp = character->FindComponentByClass<UICCharacterMovementComponent>();
		if (MoveComp && MoveComp->GetCurrentAcceleration().Length()/ MoveComp->GetMaxAcceleration()>0)
		{
			FTraversalCheckInput TraversalCheckInput(GetMoveInput(), 170.0f, 50.0f, 70.0f);
			MoveComp->TryTraversalAction(TraversalCheckInput);
		}
	}
}

//void ADX_ICPlayerController::StopJump()
//{
//	Super::StopJump();
//	if (ACharacter* character = GetCharacter())
//	{
//		if (UAbilitySystemComponent* ASC = character->FindComponentByClass<UAbilitySystemComponent>())
//		{
//			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir")));
//		}
//	}
//}
