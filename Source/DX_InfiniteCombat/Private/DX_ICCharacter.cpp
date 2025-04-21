// Copyright DX_IC


#include "DX_ICCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ICAbilitySystemComponent.h"
#include "DX_ICPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Abilities/GameplayAbility.h"
#include "CombatCharacterComponent.h"
#include "Components/CapsuleComponent.h"
#include "ICWorldSubsystem.h"


UAbilitySystemComponent* ADX_ICCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void ADX_ICCharacter::ShowDebugChange(bool showDebug)
{
	GetCapsuleComponent()->SetVisibility(showDebug);
}

ADX_ICCharacter::ADX_ICCharacter()
{
	ASC = CreateDefaultSubobject<UICAbilitySystemComponent>(TEXT("ICAbilitySystemComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatCharacterComponent>(TEXT("CombatCharacterComponent"));

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainWeapon"));
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponMeshComponent->ComponentTags.Add(TEXT("Weapon"));

}

void ADX_ICCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (ADX_ICPlayerController* ICPlayerController = Cast<ADX_ICPlayerController>(Controller))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			for (auto& GAI : ICPlayerController->AbilityInputArray)
			{
				if (GAI.InputAction && GAI.InputTag.IsValid())
				{
					//EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Started, this, &ADX_ICCharacter::Input_AbilityInputTagClick, GAI.InputTag);
					//EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Triggered, this, &ADX_ICCharacter::Input_AbilityInputTagPressed, GAI.InputTag);
					EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Started, this, &ADX_ICCharacter::Input_AbilityInputTagPressed, GAI.InputTag);
					EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Completed, this, &ADX_ICCharacter::Input_AbilityInputTagReleased, GAI.InputTag);
				}

			}
			
		}
	}
}

void ADX_ICCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator();

	GetCharacterMovement()->AirControl = 1;
	//GetCharacterMovement()->GetMaxSpeed = 1;
	
	/*if (ASC)
	{
		for (const FGAGive& FGA : GAArray)
		{
			if(FGA.GA)
				ASC->GiveAbility(FGameplayAbilitySpec(FGA.GA.GetDefaultObject(), FGA.Level));
		}
		ASC->InitAbilityActorInfo(this, this);
	}*/

	UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
	ICSubSystem->MDG_ShowDebugChange.AddDynamic(this, &ADX_ICCharacter::ShowDebugChange);

}

void ADX_ICCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir")));
}

void ADX_ICCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	ASC->AbilityInputTagPressed(InputTag);
}

void ADX_ICCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	ASC->AbilityInputTagReleased(InputTag);
}

void ADX_ICCharacter::Input_AbilityInputTagClick(FGameplayTag InputTag)
{
	ASC->AbilityInputTagClick(InputTag);
}
