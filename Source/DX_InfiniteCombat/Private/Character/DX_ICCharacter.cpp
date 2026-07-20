// Copyright DX_IC


#include "Character/DX_ICCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/ICAbilitySystemComponent.h"
#include "Character/DX_ICPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Abilities/GameplayAbility.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "Components/CapsuleComponent.h"
#include "Subsystem/ICWorldSubsystem.h"
#include "Data/ICTypes.h"
#include "ICComponents/ICMotionWarpingComponent.h"
#include "ICComponents/ICCharacterMovementComponent.h"


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

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainWeapon"), true);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponMeshComponent->ComponentTags.Add(TEXT("Weapon.Katana"));
	WeaponMeshComponent->SetupAttachment(GetMesh(), TEXT("Belt_Socket"));

	ICMotionWarpingComponent = CreateDefaultSubobject<UICMotionWarpingComponent>(TEXT("ICMotionWarpingComponent"));
}

void ADX_ICCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (ADX_ICPlayerController* ICPlayerController = Cast<ADX_ICPlayerController>(Controller))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			for (const FAbilityInput& GAI : ICPlayerController->AbilityInputArray)
			{
				if (GAI.InputAction && GAI.InputTag.IsValid())
				{
					//EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Started, this, &ADX_ICCharacter::Input_AbilityInputTagClick, GAI.InputTag);
					//EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Triggered, this, &ADX_ICCharacter::Input_AbilityInputTagPressed, GAI.InputTag);
					EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Started, this, &ADX_ICCharacter::Input_AbilityInputTagPressed, GAI.InputTag, GAI.InputComboSource);
					EnhancedInputComponent->BindAction(GAI.InputAction, ETriggerEvent::Completed, this, &ADX_ICCharacter::Input_AbilityInputTagReleased, GAI.InputTag);
				}

			}
			
		}
	}
}

void ADX_ICCharacter::Jump()
{
	Super::Jump();
	if (ASC && !ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir"))))
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir")));
	}

	UICCharacterMovementComponent* MoveComp = FindComponentByClass<UICCharacterMovementComponent>();
	if (MoveComp && ASC/* && MoveComp->GetCurrentAcceleration().Length() / MoveComp->GetMaxAcceleration() > 0*/)
	{
		FTraversalCheckInput TraversalCheckInput(GetControlMoveInput(), 60.0f, 173.0f, 40.0f);
		//MoveComp->TryTraversalAction(TraversalCheckInput);
		MoveComp->TraversalCheckInput = TraversalCheckInput;

		ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Action.Traversal"), false)));
	}
}

FVector ADX_ICCharacter::GetControlMoveInput() const
{
	if (ADX_ICPlayerController* ICController = Cast<ADX_ICPlayerController>(GetController()))
	{
		return ICController->GetMoveInput();
	}

	return FVector();
}

//void ADX_ICCharacter::StopJumping()
//{
//	Super::StopJumping();
//	if (UAbilitySystemComponent* ASC = FindComponentByClass<UAbilitySystemComponent>())
//	{
//		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir")));
//	}
//}

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
	ICSubSystem->MDG_ShowDebugChange.AddUniqueDynamic(this, &ADX_ICCharacter::ShowDebugChange);

}

void ADX_ICCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UE_LOG(LogTemp, Warning, TEXT("Character land"));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Character land"));

	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.InAir")));
}

void ADX_ICCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag, EComboSource InputComboSource)
{
	ASC->AbilityInputTagPressed(InputTag, InputComboSource);
}

void ADX_ICCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	ASC->AbilityInputTagReleased(InputTag);
}

void ADX_ICCharacter::Input_AbilityInputTagClick(FGameplayTag InputTag)
{
	ASC->AbilityInputTagClick(InputTag);
}
