// Copyright DX_IC


#include "DX_ICPlayerController.h"
#include "DX_ICCharacter.h"
#include "ICAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"


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
		if (ASC && ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InCombo"))))
		{
			character->StopAnimMontage();
		}
	}
}

void ADX_ICPlayerController::Jump()
{
	Super::Jump();
	if (ACharacter* character = GetCharacter())
	{
		if (UAbilitySystemComponent* ASC =  character->FindComponentByClass<UAbilitySystemComponent>())
		{
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir")));
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
