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
#include "UMG/WidgetCombatStates.h"


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
		//SwitchWeaponByTag(InitWeaponTag);
	}
	
}


// Called every frame
void UCombatCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
	{
		if (CurWeaponTag.IsValid() && ASC->HasMatchingGameplayTag(CurWeaponTag))
			ASC->RemoveLooseGameplayTag(CurWeaponTag);

		if(NewTag.IsValid())
			ASC->AddLooseGameplayTag(NewTag);
	}
	if (WeaponList.Find(NewTag) || !NewTag.IsValid())
		CurWeaponTag = NewTag;

	if (APawn* playerPawn = Cast<APawn>(GetOwner()))
	{
		APlayerController* playerControl = Cast<APlayerController>(playerPawn->GetController());
		if (playerControl)
		{
			FProperty* CombatStatesWidgetPro = playerControl->GetClass()->FindPropertyByName(TEXT("CombatStatesWidget"));
			if (CombatStatesWidgetPro)
			{

				FClassProperty* CombatStatesWidgetClassPro = static_cast<FClassProperty*>(CombatStatesWidgetPro);
				const void* ValuePtr = CombatStatesWidgetClassPro->ContainerPtrToValuePtr<void>(playerControl);
				TObjectPtr<UObject> CombatStatesWidgetObj = CombatStatesWidgetClassPro->GetPropertyValue(ValuePtr);

				if (UWidgetCombatStates* CombatStatesWidget = Cast<UWidgetCombatStates>(CombatStatesWidgetObj))
					CombatStatesWidget->UpdateWeaponColumn(CurWeaponTag);
			}
		}
	}
}

void UCombatCharacterComponent::CurWeaponToUnequipSocket()
{
	if (GetCurrentWeapon())
	{
		if (GetCurrentWeapon()->UnequipSocket.IsValid())
			AttachWeaponToSocket(GetCurrentWeapon()->UnequipSocket);
		else
			GetWeaponMeshComponent()->SetStaticMesh(nullptr);//DestroyComponent(); 
	}
}

void UCombatCharacterComponent::CurWeaponToEquipSocket()
{
	if (GetCurrentWeapon())
		AttachWeaponToSocket(GetCurrentWeapon()->EquipSocket);

	AddOrUpdateStaticMesh();
}

void UCombatCharacterComponent::AddOrUpdateStaticMesh()
{
	if (!GetCurrentWeapon())
		return;

	if (!GetWeaponMeshComponent())
	{
		GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, false)->ComponentTags.Add(CurWeaponTag.GetTagName());
		GetWeaponMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (!GetWeaponMeshComponent()->GetStaticMesh() && GetCurrentWeapon()->WeaponMesh)
		GetWeaponMeshComponent()->SetStaticMesh(UICAssetManager::GetAssetBySoftPtr(GetCurrentWeapon()->WeaponMesh));
}

void UCombatCharacterComponent::UpdateAnimLayer()
{
	if (USkeletalMeshComponent* BodyMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
	{
		UAnimInstance* OwnerAnimIns = BodyMesh->GetAnimInstance();
		if (!OwnerAnimIns)
			return;

		if (GetCurrentWeapon() && UICAssetManager::GetSubclassBySoftPtr(GetCurrentWeapon()->LinkAnimClass))
		{
			OwnerAnimIns->LinkAnimClassLayers(UICAssetManager::GetSubclassBySoftPtr(GetCurrentWeapon()->LinkAnimClass));
		}
		else if(UnarmLinkAnimLayer)
		{
			OwnerAnimIns->LinkAnimClassLayers(UICAssetManager::GetSubclassBySoftPtr(UnarmLinkAnimLayer));
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
