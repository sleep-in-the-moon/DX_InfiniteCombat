// Copyright DX_IC


#include "ICComponents/BowComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projector/ProjectorActorBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/DXCharacterExtensionComponent.h"
#include "UMG/ForesightWidgeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UBowComponent::UBowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ArrowClass = LoadClass<AProjectorActorBase>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Weapon/BP/Bullets/BP_ArrowProje.BP_ArrowProje_C'"));
	ForesightWidgeClass = LoadClass<UForesightWidgeBase>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WBP/WBP_Foresight.WBP_Foresight_C'"));
	ArrowLaunchSound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/Bow/ArrowClatter.ArrowClatter'"));
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

bool UBowComponent::TryLaunchArrow(float ChargeTime)
{
	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (ChargeTime > 0.7f && CurHoldingArrow && Camera)
	{
		CurHoldingArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		FVector LaunchVelocity = Camera->GetForwardVector();

		FHitResult HitRes;
		FVector End = Camera->GetComponentLocation() + Camera->GetForwardVector()* TraceDist;
		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Camera->GetComponentLocation(), End, TraceObjTypes, false, {},
			EDrawDebugTrace::None, HitRes, true))
		{
			LaunchVelocity = (HitRes.ImpactPoint - CurHoldingArrow->GetActorLocation()).GetSafeNormal(0.0001);
		}

		CurHoldingArrow->Launch(ChargeTime, LaunchVelocity);
		CurHoldingArrow->EnableTraceBySocketName(TraceObjTypes, true, ArrowTraceSocketPrefix);

		if(ArrowLaunchSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ArrowLaunchSound, CurHoldingArrow->GetActorLocation(), 2.5f);

		UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC && ArrowTag.IsValid())
		{
			ASC->RemoveLooseGameplayTag(ArrowTag);
		}

		return true;
	}

	return false;
}

void UBowComponent::EnterAimMode()
{
	if (UDXCharacterExtensionComponent* ExtensionComp = GetOwner()->FindComponentByClass<UDXCharacterExtensionComponent>())
	{
		ExtensionComp->LerpSpringArmEndOffsetToTarget(AimModeCameraOffset);

		ExtensionComp->LerpActorRotToControlRot();
		LerpToControlRotFinishHandle = ExtensionComp->DG_LerpToControlRotFinish.AddUObject(this, &UBowComponent::LerpToControlRotFinish);

		if (HidUITimer.IsValid())
			GetWorld()->GetTimerManager().ClearTimer(HidUITimer);

		APawn* OwnerP = Cast<APawn>(GetOwner());
		if (!ForesightWidge && ForesightWidgeClass && OwnerP)
		{
			if (APlayerController* PlayerControl = Cast<APlayerController>(OwnerP->GetController()))
			{
				ForesightWidge = CreateWidget<UForesightWidgeBase>(PlayerControl, ForesightWidgeClass);
				ForesightWidge->AddToViewport();
			}	
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

	GetWorld()->GetTimerManager().SetTimer(HidUITimer, [this]()
	{
		if (ForesightWidge)
		{
			ForesightWidge->SetVisibility(ESlateVisibility::Collapsed);
		}
	}, 0.9, false);
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
