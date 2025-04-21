// Copyright DX_IC


#include "CombatCharacterComponent.h"
#include "KismetTraceUtils.h"
//#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "ICWorldSubsystem.h"

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

	if (IsLockOnTarget)
		DoLockToTarget();
	// ...
}

bool UCombatCharacterComponent::StartLockOntoTarget(ETraceShapeType TraceType, float CheckHalf, float Dist)
{
	RawOutHits.Empty();
	FCollisionShape shape;
	bool bDebugHit = false;

	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + GetOwner()->GetActorForwardVector() * Dist;
	if (UCameraComponent* camera = GetOwner()->FindComponentByClass<UCameraComponent>())
	{
		Start = camera->GetComponentLocation();
		End = Start + camera->GetForwardVector() * Dist;
	}
	UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());

	switch (TraceType)
	{
	case ETraceShapeType::Sphere:
		shape = FCollisionShape::MakeSphere(CheckHalf);
		
		if (ICSubSystem->GetShowDebug())
			DrawDebugSphereTraceMulti(GetOwner()->GetWorld(), Start, End, CheckHalf, EDrawDebugTrace::Type::ForDuration, bDebugHit, RawOutHits, FLinearColor::Blue, FLinearColor::Green, 10.0f);
		break;
	case ETraceShapeType::Capsule:
		shape = FCollisionShape::MakeCapsule(CheckHalf /2, CheckHalf /2);
		break;
	case ETraceShapeType::Box:
		shape = FCollisionShape::MakeBox(FVector(CheckHalf, CheckHalf /2, CheckHalf /2));
		break;
	case ETraceShapeType::Line:
		break;
	case ETraceShapeType::Cone:
		break;
	default:
		break;
	}

	static const FName LineTraceMultiName(TEXT("LineTraceMultiForObjects"));
	//ConfigureCollisionParams(LineTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, MeshComp->GetOwner());
	Params = FCollisionQueryParams(LineTraceMultiName, bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	if (bIgnoreSelf)
	{
		const AActor* IgnoreActor = Cast<AActor>(GetOwner());
		if (IgnoreActor)
		{
			Params.AddIgnoredActor(IgnoreActor);
		}
		else
		{
			// find owner
			const UObject* CurrentObject = GetOwner();
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				IgnoreActor = Cast<AActor>(CurrentObject);
				if (IgnoreActor)
				{
					Params.AddIgnoredActor(IgnoreActor);
					break;
				}
			}
		}
	}
	ObjectQueryParams = FCollisionObjectQueryParams(TraceObjectTypes);
	
	bool const bHit = GetOwner()->GetWorld()->SweepMultiByObjectType(RawOutHits, Start, End, FQuat::Identity, ObjectQueryParams, shape, Params);
	ChangeOutHits = RawOutHits;

	if (bHit)
	{
		LockObj = RawOutHits.Last().GetActor();
		//GetOwner()->GetWorldTimerManager().SetTimer(TimerLockTarget, this, &UCombatCharacterComponent::DoLockToTarget, LockFrequency, true, 0.f);
		IsLockOnTarget = true;

		if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget")));
	}

	return bHit;
}

void UCombatCharacterComponent::EndLockOntoTarget()
{
	IsLockOnTarget = false;
	RawOutHits.Empty();
	ChangeOutHits.Empty();

	if (UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget")));

	if (TimerLockTarget.IsValid())
		GetOwner()->GetWorldTimerManager().ClearTimer(TimerLockTarget);
}

bool UCombatCharacterComponent::ChangeTarget()
{
	if (RawOutHits.Num() < 2)
		return false;

	if (ChangeOutHits.Num() == 0)
		ChangeOutHits = RawOutHits;

	LockObj = ChangeOutHits.Pop().GetActor();

	return true;
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
		if (!GetWeaponComponent())
		{
			GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, false)->ComponentTags.Add(TEXT("Weapon"));
			GetWeaponComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
			
		GetWeaponComponent()->SetStaticMesh(CurWeapon.WeaponMesh);
		
		WeaponToHeld();

		//LinkAnimLayer | PlayMontage | TriggerGA
		if (USkeletalMeshComponent* BodyMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>())
		{
			UAnimInstance* OwnerAnimIns = BodyMesh->GetAnimInstance();
			if (OwnerAnimIns && CurWeapon.LinkAnimClass)
			{
				OwnerAnimIns->LinkAnimClassLayers(CurWeapon.LinkAnimClass);
			}
		}
		
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
	if (BodyMesh && GetWeaponComponent())
	{
		GetWeaponComponent()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		return GetWeaponComponent()->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform, socketName);
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
		asc->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Died")));
	}

	DG_CharacterDied.Broadcast();
}

void UCombatCharacterComponent::DoLockToTarget()
{
	if (LockObj)
	{
		FRotator TargetRot = FRotationMatrix::MakeFromX(LockObj->GetActorLocation() - GetOwner()->GetActorLocation()).Rotator();

		GetOwner()->SetActorRotation(FRotator(GetOwner()->GetActorRotation().Pitch, TargetRot.Yaw, GetOwner()->GetActorRotation().Roll));
	}
}

