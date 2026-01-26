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
	if (InitWeaponTag.IsValid() && !InitWeaponTag.ToString().Equals("Weapon.Unarmed") && WeaponList.Contains(InitWeaponTag))
	{
		SwitchWeaponByTag(InitWeaponTag);
	}
	else if (WeaponList.Num() > 0 && !InitWeaponTag.ToString().Equals("Weapon.Unarmed"))
	{
		TArray<FGameplayTag> TempKey;
		WeaponList.GetKeys(TempKey);
		SwitchWeaponByTag(TempKey[0]);
	}
	WeaponToSecond();
	
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
		
		WeaponToHeld();//改为动画通知触发

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
	
	return false;
}

bool UCombatCharacterComponent::WeaponToHeld()
{
	return AttachWeaponToSocket(CurWeapon.HeldSocket);
}

bool UCombatCharacterComponent::WeaponToSecond()
{
	return AttachWeaponToSocket(CurWeapon.SecondSocket);
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
	ACharacter* character = Cast<ACharacter>(GetOwner());
	if (character && DiedMontage)
	{
		character->StopAnimMontage();
		character->PlayAnimMontage(DiedMontage);
	}

	if (UAbilitySystemComponent* asc = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		asc->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Died"), false));
	}

	DG_CharacterDied.Broadcast();
}
