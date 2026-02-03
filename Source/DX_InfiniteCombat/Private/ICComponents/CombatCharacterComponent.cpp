// Copyright DX_IC


#include "ICComponents/CombatCharacterComponent.h"
#include "KismetTraceUtils.h"
//#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Subsystem/ICWorldSubsystem.h"
#include "Data/WeaponDataAsset.h"
#include "Data/ICAssetManager.h"


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
	
}


// Called every frame
void UCombatCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCombatCharacterComponent::SwitchWeaponByTag(FGameplayTag Tag)
{
	if (!WeaponList.Contains(Tag))
	{
		UE_LOG(LogTemp, Warning, TEXT("Character 没有%s武器"), *Tag.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Character 没有%s武器"), *Tag.ToString()));
		return false;
	}
		
	if (Tag.IsValid())
	{
		if (CurWeaponTag.IsValid())
		{
			UnequipWeapon();
		}
		// ChangeTag
		SetToNewWeaponTag(Tag);

		// ChangeSocket、PlayMontage
		EquipWeapon();

		// ChangeStaticMesh
		AddOrUpdateStaticMesh();
		
		// LinkAnimLayer
		UpdateAnimLayer();
	}
	else
	{
		if (CurWeaponTag.IsValid())
		{
			UnequipWeapon();
			SetToNewWeaponTag(FGameplayTag::EmptyTag);
		}

		if (GetWeaponMeshComponent())
		{
			GetWeaponMeshComponent()->SetStaticMesh(nullptr);
		}
		
		// LinkUnarmAnimLayer
		
	}

	return true;
}

void UCombatCharacterComponent::AddNewWeapon(const FGameplayTag& NewWeaponTag)
{
	WeaponList.Add(NewWeaponTag);
}

const UWeaponDataAsset* UCombatCharacterComponent::GetCurrentWeapon() const
{
	if (CurWeaponTag.IsValid())
	{
		return UWeaponDataAsset::GetInsByTag(CurWeaponTag);
	}

	return nullptr;
}

void UCombatCharacterComponent::EquipWeapon()
{
	if (const UWeaponDataAsset* Asset =  GetCurrentWeapon())
	{
		PlayMontageBySoftPtr(Asset->AM_EquipWeapon);
	}
	if(GetCurrentWeapon())
		AttachWeaponToSocket(GetCurrentWeapon()->EquipSocket);// 改为动画通知触发
}

void UCombatCharacterComponent::UnequipWeapon()
{
	if (GetCurrentWeapon())
	{
		PlayMontageBySoftPtr(GetCurrentWeapon()->AM_UnequipWeapon);
	}
	if(GetCurrentWeapon())
		AttachWeaponToSocket(GetCurrentWeapon()->UnequipSocket);
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

void UCombatCharacterComponent::PlayMontageBySoftPtr(TSoftObjectPtr<UAnimMontage> Montage)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && UICAssetManager::GetAssetBySoftPtr(Montage, false))
	{
		Character->StopAnimMontage();
		Character->PlayAnimMontage(UICAssetManager::GetAssetBySoftPtr(Montage, false));
	}
}

void UCombatCharacterComponent::SetToNewWeaponTag(const FGameplayTag& NewTag)
{
	if (!WeaponList.Find(NewTag))
		return;

	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		if (CurWeaponTag.IsValid() && ASC->HasMatchingGameplayTag(CurWeaponTag))
			ASC->RemoveLooseGameplayTag(CurWeaponTag);

		if(NewTag.IsValid())
			ASC->AddLooseGameplayTag(NewTag);
	}
	CurWeaponTag = *WeaponList.Find(NewTag);
}

void UCombatCharacterComponent::CurWeaponToUnequipSocket()
{
	if (GetCurrentWeapon())
		AttachWeaponToSocket(GetCurrentWeapon()->UnequipSocket);
}

void UCombatCharacterComponent::CurWeaponToEquipSocket()
{
	if (GetCurrentWeapon())
		AttachWeaponToSocket(GetCurrentWeapon()->EquipSocket);
}

void UCombatCharacterComponent::AddOrUpdateStaticMesh()
{
	if (!GetWeaponMeshComponent())
	{
		GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, false)->ComponentTags.Add(TEXT("Weapon"));
		GetWeaponMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (GetCurrentWeapon())
		GetWeaponMeshComponent()->SetStaticMesh(UICAssetManager::GetAssetBySoftPtr(GetCurrentWeapon()->WeaponMesh));
}

void UCombatCharacterComponent::UpdateAnimLayer()
{
	if (USkeletalMeshComponent* BodyMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		UAnimInstance* OwnerAnimIns = BodyMesh->GetAnimInstance();
		if (OwnerAnimIns && GetCurrentWeapon() && UICAssetManager::GetSubclassBySoftPtr(GetCurrentWeapon()->LinkAnimClass))
		{
			OwnerAnimIns->LinkAnimClassLayers(UICAssetManager::GetSubclassBySoftPtr(GetCurrentWeapon()->LinkAnimClass));
		}
	}
}

void UCombatCharacterComponent::CharacterDied()
{
	PlayMontageBySoftPtr(Montage_Died);

	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Died"), false));
	}

	DG_CharacterDied.Broadcast();
}
