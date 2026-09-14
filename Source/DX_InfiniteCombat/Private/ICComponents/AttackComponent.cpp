// Copyright DX_IC


#include "ICComponents/AttackComponent.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "KismetTraceUtils.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "DX_StaticFunlib.h"
#include "CollisionQueryParams.h"
#include "GAS/ICAbilitySystemComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Damage.h"
#include "Subsystem/ICWorldSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/ICTypes.h"
#include "GAS/ICGameplayEffectTypes.h"
#include "Utils/IC_Utils.h"
#include "GameFramework/Character.h"
#include "Data/ICAssetManager.h"
#include "Data/ICDataAsset.h"
#include "Physics/PhysicalMaterialWithTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "MotionWarpingComponent.h"
#include "GAS/AttackEventData.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
		return;

	OwnerASC = GetOwner()->FindComponentByClass<UICAbilitySystemComponent>();
	ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetOwner()->GetWorld());
	MeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	MotionWarpingComp = GetOwner()->FindComponentByClass<UMotionWarpingComponent>();
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAttackComponent::TraceBySocketsForTick(USceneComponent* SocketOwner, const TArray<FName>& TraceSockets, TMap<FName, FVector>& PreSocketLoc, TArray<struct FHitResult>& OutHitArray, const FAttackTraceParam& TraceParam)
{
	if (TraceSockets.IsEmpty() || !SocketOwner)
		return false;

	static  FCollisionQueryParams CollisionQueryParams;
	static FCollisionObjectQueryParams ObjectQueryParams;

	FHitResult OutHitSingle;
	TArray<struct FHitResult> OutHitArrayTemp;

	UDX_StaticFunlib::MakeCollisionParam(GetOwner(), CollisionQueryParams, ObjectQueryParams, TraceParam.IgnoreActors, TraceParam.TraceObjectTypes, TraceParam.bTraceComplex, TraceParam.bIgnoreSelf);
	bool bHit = false;
	OutHitArray.Empty();

	for (const FName& TraceSocket : TraceSockets)
	{
		if (!SocketOwner->DoesSocketExist(TraceSocket))
			continue;
		if (!PreSocketLoc.Contains(TraceSocket))
		{
			PreSocketLoc.Add(TraceSocket, SocketOwner->GetSocketLocation(TraceSocket));
			continue;
		}

		switch (TraceParam.AttackTraceShape)
		{
		case EAttackTraceShape::Line:
			if(TraceParam.bMultiTrace)
				bHit = bHit || GetOwner()->GetWorld()->LineTraceMultiByObjectType(OutHitArrayTemp, *PreSocketLoc.Find(TraceSocket), SocketOwner->GetSocketLocation(TraceSocket), ObjectQueryParams, CollisionQueryParams);
			else
				bHit = bHit || GetWorld()->LineTraceSingleByObjectType(OutHitSingle, *PreSocketLoc.Find(TraceSocket), SocketOwner->GetSocketLocation(TraceSocket), ObjectQueryParams, CollisionQueryParams);
#if WITH_EDITOR	
			if (ICSubSystem && ICSubSystem->GetShowDebug())
				DrawDebugLineTraceMulti(GetOwner()->GetWorld(), *PreSocketLoc.Find(TraceSocket), SocketOwner->GetSocketLocation(TraceSocket), EDrawDebugTrace::Type::ForDuration, bHit, OutHitArrayTemp, FColor::Red, FColor::Green, 1.5f);
#endif
			break;

		case EAttackTraceShape::Box:
			if (TraceParam.bMultiTrace)
				bHit = bHit || GetOwner()->GetWorld()->SweepMultiByObjectType(OutHitArrayTemp, *PreSocketLoc.Find(TraceSocket), SocketOwner->GetSocketLocation(TraceSocket), GetOwner()->GetActorRotation().Quaternion(), ObjectQueryParams, FCollisionShape::MakeBox(TraceParam.ShapeExtent), CollisionQueryParams);
			else
				bHit = bHit || GetOwner()->GetWorld()->SweepSingleByObjectType(OutHitSingle, *PreSocketLoc.Find(TraceSocket), SocketOwner->GetSocketLocation(TraceSocket), GetOwner()->GetActorRotation().Quaternion(), ObjectQueryParams, FCollisionShape::MakeBox(TraceParam.ShapeExtent), CollisionQueryParams);
#if WITH_EDITOR	
			if (ICSubSystem && ICSubSystem->GetShowDebug())
				DrawDebugBoxTraceMulti(GetOwner()->GetWorld(), *PreSocketLoc.Find(TraceSocket), SocketOwner->GetSocketLocation(TraceSocket), TraceParam.ShapeExtent, GetOwner()->GetActorRotation(), EDrawDebugTrace::Type::ForDuration, bHit, OutHitArrayTemp, FColor::Red, FColor::Green, 1.5f);
#endif
			break;
		default:
			break;
		}

		if (TraceParam.bMultiTrace)
			OutHitArray.Append(OutHitArrayTemp);
		else
			OutHitArray.Add(OutHitSingle);
		
		PreSocketLoc.Add(TraceSocket, SocketOwner->GetSocketLocation(TraceSocket));
	}

	return bHit;
}

void UAttackComponent::HitFeedback(UAnimMontage* CurAttackAnimMontage, bool bSlowdown, float SlowdownScale, float SlowdownTime, bool bSkipFrames, float SkipSeconds)
{
	//镜头抖动，时间膨胀 等GEGC
	if (ICSubSystem->AttackFeedbackGE)
	{
		if (UGameplayEffect* GECDO = Cast<UGameplayEffect>(ICSubSystem->AttackFeedbackGE->GetDefaultObject()))
			OwnerASC->ApplyGameplayEffectToSelf(GECDO, 0, FGameplayEffectContextHandle());
	}

	if (!MeshComp || !CurAttackAnimMontage)
		return;

	//缓速 顿感
	if (bSlowdown && SlowdownScale > 0.0f && SlowdownTime > 0.0f)
	{
		MeshComp->GetAnimInstance()->Montage_SetPlayRate(CurAttackAnimMontage, SlowdownScale);
		FTimerHandle SlowDownTimer;
		MeshComp->GetWorld()->GetTimerManager().SetTimer(SlowDownTimer, [this, CurAttackAnimMontage]()
			{
				MeshComp->GetAnimInstance()->Montage_SetPlayRate(CurAttackAnimMontage, 1);
			}, SlowdownTime, false);
	}

	//抽帧
	if (bSkipFrames && SkipSeconds > 0.0f)
	{
		MeshComp->GetAnimInstance()->Montage_Pause(CurAttackAnimMontage);
		float CurPlayTime = MeshComp->GetAnimInstance()->Montage_GetPosition(CurAttackAnimMontage);

		//计算当前帧
		//const FFrameRate FrameRate = CurAttackAnimMontage->GetDataModel()->GetFrameRate();
		////float CurrentFrame = CurPlayTime * FrameRate.AsDecimal();

		//float SkipSeconds = FrameRate.AsDecimal();

		FTimerHandle SkipFrameTimer;
		MeshComp->GetWorld()->GetTimerManager().SetTimer(SkipFrameTimer, [this, CurAttackAnimMontage, CurPlayTime, SkipSeconds]()
			{
				MeshComp->GetAnimInstance()->Montage_SetPosition(CurAttackAnimMontage, CurPlayTime + SkipSeconds);
				MeshComp->GetAnimInstance()->Montage_Resume(CurAttackAnimMontage);
			}, SkipSeconds, false);
	}

}

void UAttackComponent::ApplyHitResToTargetActor(const FHitResult& HitRes, float DamageATKCoefficient, float KnockbackDist, bool bCanBeBlock)
{
	if (!HitRes.bBlockingHit || !HitRes.GetActor())
		return;

	//根据物理材质分部位叠加计算
	float BodyPartDamageATKCoefficient = 1.0f;
	UPhysicalMaterialWithTags* PhysicalMaterialWithTags = Cast<UPhysicalMaterialWithTags>(HitRes.PhysMaterial);
	UCombatCharacterComponent* combatCom = MeshComp->GetOwner()->FindComponentByClass<UCombatCharacterComponent>();
	if (PhysicalMaterialWithTags && combatCom)
	{
		for (FGameplayTag MatTag : PhysicalMaterialWithTags->Tags.GetGameplayTagArray())
		{
			if (combatCom->BodyPartDamageATKCoefficient.Contains(MatTag))
			{
				BodyPartDamageATKCoefficient *= combatCom->BodyPartDamageATKCoefficient.FindRef(MatTag);
			}
		}
	}

	if (UAbilitySystemComponent* TargetASC = HitRes.GetActor()->FindComponentByClass<UAbilitySystemComponent>())
	{
		//格挡 招架
		if (bCanBeBlock && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Blocking"), false)))//TODO::受击与格挡面朝方向角度限制
		{
			FGameplayEventData EventData;
			EventData.Instigator = GetOwner();
			UAttackEventData* EventDataObj = NewObject<UAttackEventData>();
			EventDataObj->HitLocation = HitRes.ImpactPoint;
			EventDataObj->ImpactNormal = HitRes.ImpactNormal;
			EventData.OptionalObject = EventDataObj;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitRes.GetActor(), FGameplayTag::RequestGameplayTag(TEXT("GameplayEvent.BlockTrigger"), false), EventData);
		}
		//应用伤害 GE
		else if (const TSubclassOf<UGameplayEffect> DamageGE = UICAssetManager::GetSubclassBySoftPtr(UICDataAsset::Get().DamageGEClass))
		{
			FGameplayEffectSpecHandle GESpecHandle = AttackUtils::MakeAttackGESpecHandle(MeshComp->GetOwner(), DamageGE, HitRes, DamageATKCoefficient * BodyPartDamageATKCoefficient);
			OwnerASC->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), TargetASC);
		}

	}

	//伤害感知事件发送
	UAISense_Damage::ReportDamageEvent(MeshComp->GetOwner()->GetWorld(), HitRes.GetActor(), MeshComp->GetOwner(), 0.f, MeshComp->GetOwner()->GetActorLocation(), HitRes.GetActor()->GetActorLocation());

	//击退
	if (!FMath::IsNearlyEqual(KnockbackDist, 0.0f))
	{
		UCharacterMovementComponent* CharaMoveCom = HitRes.GetActor()->FindComponentByClass<UCharacterMovementComponent>();
		ACharacter* HitCharacter = Cast<ACharacter>(HitRes.GetActor());
		if (CharaMoveCom && HitCharacter)
		{
			FVector LaunchDirect = HitRes.GetActor()->GetActorLocation() - MeshComp->GetOwner()->GetActorLocation();
			HitCharacter->LaunchCharacter(FMath::Sqrt(2 * CharaMoveCom->BrakingDecelerationWalking * KnockbackDist) * LaunchDirect.GetSafeNormal(), true, false);
		}
	}

	//物理混合
	/*if (USkeletalMeshComponent* Skele = res.GetActor()->FindComponentByClass<USkeletalMeshComponent>())
	{
		Skele->SetPhysicsBlendWeight(0.5);
		Skele->SetBodySimulatePhysics(TEXT("Pelvis"), false);
	}*/

	//Clash
	/*TArray<AActor*> ClashActors;
	FGameplayEffectQuery ClashGEQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("GameplayEffect.Clash"), false)));
	TArray<FActiveGameplayEffectHandle> ClashGEHandles = OwnerASC->GetActiveEffects(ClashGEQuery);
	for (FActiveGameplayEffectHandle& ClashGEHandle : ClashGEHandles)
	{
		if (AActor* Instigator = OwnerASC->GetActiveGameplayEffect(ClashGEHandle)->Spec.GetContext().Get()->GetInstigator())
		{
			ClashActors.Emplace(Instigator);
		}
	}*/
	//if (ClashActors.Contains(res.GetActor()))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Clash success"));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Clash success")));
	//}
	//else if (UAbilitySystemComponent* TargetASC = res.GetActor()->FindComponentByClass<UAbilitySystemComponent>())
	//{
	//	FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();
	//	//ContextHandle.cont SetDuration
	//	FGameplayEffectSpecHandle GESpecHandle = OwnerASC->MakeOutgoingSpec(ICSubSystem->ClashGE, 0, ContextHandle);
	//	OwnerASC->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), TargetASC);
	//}

	//..其它
}

void UAttackComponent::AddOrUpdateAttackFollowWarp(const FVector& NewFollowLocation)
{
	if (!MotionWarpingComp)
		return;

#if WITH_EDITOR
	DrawDebugPoint(GetWorld(), NewFollowLocation, 10, FColor::Blue, false, 3);
#endif

	MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(TEXT("AttackFollow"), NewFollowLocation);
}

void UAttackComponent::RemoveAttackFollowWarp()
{
	if (!MotionWarpingComp)
		return;

	MotionWarpingComp->RemoveWarpTarget(TEXT("AttackFollow"));
}

void UAttackComponent::CreateClashWindow()
{
	//ClashWindow = true;
}

bool UAttackComponent::HasClashWindow() const
{
	return ClashWindow;
}

void UAttackComponent::ConsumeClashWindow()
{
	//ClashWindow = false;
}

void UAttackComponent::CloseClashWindow()
{
	//ClashWindow = false;
}
