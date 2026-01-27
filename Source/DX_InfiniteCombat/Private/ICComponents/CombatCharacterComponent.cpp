// Copyright DX_IC


#include "ICComponents/CombatCharacterComponent.h"
#include "KismetTraceUtils.h"
//#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Subsystem/ICWorldSubsystem.h"

// Sets default values for this component's properties
UCombatCharacterComponent::UCombatCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化拥有者的武器
	if (InitWeaponTag.IsValid())
	{
		SwitchWeaponByTag(InitWeaponTag);
	}
	//UnequipWeapon();
	
}


// Called every frame
void UCombatCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCombatCharacterComponent::SwitchWeaponByTag(FGameplayTag Tag)
{
	if (Tag.IsValid() && WeaponList.Contains(Tag))
	{
		//ChangeTag | ChangeStaticMesh | ChangeSocket
		if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
		{
			if(CurWeapon.WeaponTag.IsValid())
				ASC->RemoveLooseGameplayTag(CurWeapon.WeaponTag);
			ASC->AddLooseGameplayTag(Tag);
		}
		CurWeapon = *WeaponList.Find(Tag);
		if (!GetWeaponMeshComponent())
		{
			GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, false)->ComponentTags.Add(TEXT("Weapon"));
			GetWeaponMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
			
		GetWeaponMeshComponent()->SetStaticMesh(CurWeapon.WeaponMesh);
		
		EquipWeapon();//改为动画通知触发

		//LinkAnimLayer | PlayMontage | TriggerGA
		if (USkeletalMeshComponent* BodyMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
		{
			UAnimInstance* OwnerAnimIns = BodyMesh->GetAnimInstance();
			if (OwnerAnimIns && CurWeapon.LinkAnimClass)
			{
				OwnerAnimIns->LinkAnimClassLayers(CurWeapon.LinkAnimClass);
			}
		}
		//if(CurWeapon.AM_SecondToHeld)
		
		return true;
	}
	
	else if (!Tag.IsValid())
	{
		UnequipWeapon();
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Character 没有%s武器"), *Tag.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Character 没有%s武器"), *Tag.ToString()));
	return false;
}

bool UCombatCharacterComponent::EquipWeapon()
{
	return AttachWeaponToSocket(CurWeapon.EquipSocket);
}

bool UCombatCharacterComponent::UnequipWeapon()
{
	return AttachWeaponToSocket(CurWeapon.UnequipSocket);
}

bool UCombatCharacterComponent::AttachWeaponToSocket(FName socketName)
{
	USkeletalMeshComponent* BodyMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	if (!socketName.IsNone() && BodyMesh && BodyMesh->DoesSocketExist(socketName) && GetWeaponMeshComponent())
	{
		GetWeaponMeshComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		return GetWeaponMeshComponent()->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform, socketName);
	}

	return false;
}

void UCombatCharacterComponent::CharacterDied()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && DiedMontage)
	{
		Character->StopAnimMontage();
		Character->PlayAnimMontage(DiedMontage);
	}

	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Died"), false));
	}

	DG_CharacterDied.Broadcast();
}
