// Copyright DX_IC


#include "ICComponents/BowComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projector/ProjectorActorBase.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UBowComponent::UBowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ArrowClass = LoadClass<AProjectorActorBase>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Weapon/BP/Bullets/BP_ArrowProje.BP_ArrowProje_C'"));
}


// Called when the game starts
void UBowComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBowComponent::TrySetupArrow()
{
	if (ArrowCount > 0)
	{
		//TODO::PlayMontage
		USkeletalMeshComponent* SkeleMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if (SkeleMesh && SkeleMesh->DoesSocketExist(HandArrowSocketName) && ArrowClass)
		{
			CurHoldingArrow = GetWorld()->SpawnActor<AProjectorActorBase>(ArrowClass, SkeleMesh->GetSocketTransform(HandArrowSocketName));
			CurHoldingArrow->SetOwner(GetOwner());
			CurHoldingArrow->AttachToComponent(SkeleMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandArrowSocketName);

			UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
			if (ASC && ArrowTag.IsValid())
			{
				ASC->AddLooseGameplayTag(ArrowTag);
			}

			DG_ArrowCountChange.Broadcast(--ArrowCount);

			return true;
		}
	}

	return false;
}

bool UBowComponent::TryLaunchArrow()
{
	return false;
}

void UBowComponent::EnterAimMode()
{
	if (USpringArmComponent* SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>())
	{
		SpringArm->SocketOffset;
	}
}

void UBowComponent::ExitAimMode()
{
}
