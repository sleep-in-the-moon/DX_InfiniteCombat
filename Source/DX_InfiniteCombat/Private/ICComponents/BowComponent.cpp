// Copyright DX_IC


#include "ICComponents/BowComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projector/ProjectorActorBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/DXCharacterExtensionComponent.h"
#include "UMG/ForesightWidgeBase.h"


// Sets default values for this component's properties
UBowComponent::UBowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ArrowClass = LoadClass<AProjectorActorBase>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Weapon/BP/Bullets/BP_ArrowProje.BP_ArrowProje_C'"));
	ForesightWidgeClass = LoadClass<AProjectorActorBase>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WBP/WBP_Foresight.WBP_Foresight_C'"));
}


// Called when the game starts
void UBowComponent::BeginPlay()
{
	Super::BeginPlay();

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
	if (UDXCharacterExtensionComponent* ExtensionComp = GetOwner()->FindComponentByClass<UDXCharacterExtensionComponent>())
	{
		ExtensionComp->LerpSpringArmEndOffsetToTarget(AimModeCameraOffset);

		ExtensionComp->LerpActorRotToControlRot();
		LerpToControlRotFinishHandle = ExtensionComp->DG_LerpToControlRotFinish.AddUObject(this, &UBowComponent::LerpToControlRotFinish);

		//TimerHidForesight Clear

		if (!ForesightWidge && ForesightWidgeClass)
		{
			ForesightWidge = CreateWidget<UForesightWidgeBase>(ForesightWidgeClass);
			ForesightWidge->AddToViewport();
		}
		if (ForesightWidge)
		{
			ForesightWidge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UBowComponent::ExitAimMode()
{
	if (UDXCharacterExtensionComponent* ExtensionComp = GetOwner()->FindComponentByClass<UDXCharacterExtensionComponent>())
	{
		ExtensionComp->SetYawByControl(false);
		ExtensionComp->LerpSpringArmEndOffsetToTarget(FVector(0, 0, 0));
	}

	//TimerHidForesight
}

bool UBowComponent::TryHoldingBow()
{
	return false;
}

void UBowComponent::UnHoldingBow()
{
}

void UBowComponent::LerpToControlRotFinish()
{
	if (UDXCharacterExtensionComponent* ExtensionComp = GetOwner()->FindComponentByClass<UDXCharacterExtensionComponent>())
	{
		ExtensionComp->SetYawByControl(true);

		if(LerpToControlRotFinishHandle.IsValid())
			ExtensionComp->DG_LerpToControlRotFinish.Remove(LerpToControlRotFinishHandle);
	}
}
