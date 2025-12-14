// Copyright DX_IC


#include "AN_AttackTrace.h"
#include "CombatCharacterComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KismetTraceUtils.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "CollisionQueryParams.h"
#include "ICAbilitySystemComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Damage.h"
#include "ICWorldSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ICTypes.h"
#include "GAS/ICGameplayEffectTypes.h"
#include "Utils/IC_Utils.h"
#include "GameFramework/Character.h"


void UAN_AttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ApplyedObjs.Empty();
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UCombatCharacterComponent* combatCom = Owner->FindComponentByClass<UCombatCharacterComponent>())
		{
			CurWeapon = combatCom->GetWeaponComponent();
			if(IsValid(CurWeapon))
			{
				//TracePointNum = TracePointNum > 2 ? TracePointNum : 2;
				
				AttackTraceSockets.Empty();
				PreSocketLoc.Empty();

				for (auto& socket : CurWeapon->GetAllSocketNames())
				{
					if (socket.ToString().Contains("AttackTrace"))
					{
						AttackTraceSockets.AddUnique(socket);
						PreSocketLoc.Add(socket, CurWeapon->GetSocketLocation(socket));
					}
				}
			
			}
		}
	}

	static const FName LineTraceMultiName(TEXT("LineTraceMultiForObjects"));
	//ConfigureCollisionParams(LineTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, MeshComp->GetOwner());
	Params = FCollisionQueryParams(LineTraceMultiName, bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	if (bIgnoreSelf)
	{
		const AActor* IgnoreActor = Cast<AActor>(MeshComp->GetOwner());
		if (IgnoreActor)
		{
			Params.AddIgnoredActor(IgnoreActor);
		}
		else
		{
			// find owner
			const UObject* CurrentObject = MeshComp->GetOwner();
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

	OwnerASC = MeshComp->GetOwner()->FindComponentByClass<UICAbilitySystemComponent>();

}

void UAN_AttackTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!IsValid(CurWeapon) || CurWeapon==nullptr)
		return;
	
	static TArray<struct FHitResult> HitRes;
	HitRes.Reset();
	bool  bHit = false;

	if(AttackTraceSockets.Num() > 0)
	for (auto& socket : AttackTraceSockets)
	{
		HitRes.Reset();
		if (!CurWeapon->DoesSocketExist(socket))
			continue;

		UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(MeshComp->GetOwner()->GetWorld());
		switch (AttackTraceShape)
		{
		case EAttackTraceShape::Line:
			bHit = MeshComp->GetOwner()->GetWorld()->LineTraceMultiByObjectType(HitRes, *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), ObjectQueryParams, Params);
			//DrawDebugLine(MeshComp->GetOwner()->GetWorld(), *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), FColor::Red, false, 1.5f, 0, 1.0f);
			if (ICSubSystem->GetShowDebug())
				DrawDebugLineTraceMulti(MeshComp->GetOwner()->GetWorld(), *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), EDrawDebugTrace::Type::ForDuration, bHit, HitRes, FColor::Red, FColor::Green, 1.5f);

			break;

		case EAttackTraceShape::Box:
			bHit = MeshComp->GetOwner()->GetWorld()->SweepMultiByObjectType(HitRes, *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), MeshComp->GetOwner()->GetActorRotation().Quaternion(), ObjectQueryParams, FCollisionShape::MakeBox(BoxHalf), Params);
			
			if (ICSubSystem->GetShowDebug())
				DrawDebugBoxTraceMulti(MeshComp->GetOwner()->GetWorld(), *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), BoxHalf, MeshComp->GetOwner()->GetActorRotation(), EDrawDebugTrace::Type::ForDuration, bHit, HitRes, FColor::Red, FColor::Green, 1.5f);

			break;
		default:
			break;
		}

		//DrawDebugLine(MeshComp->GetOwner()->GetWorld(), *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), FColor::Red, false, 1.5f, 0, 1.0f);
		
		PreSocketLoc.Add(socket, CurWeapon->GetSocketLocation(socket));

		if (bHit)
		{
			//Clash
			TArray<AActor*> ClashActors;
			FGameplayEffectQuery ClashGEQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("GameplayEffect.Clash"), false)));
			TArray<FActiveGameplayEffectHandle> ClashGEHandles = OwnerASC->GetActiveEffects(ClashGEQuery);
			for (FActiveGameplayEffectHandle& ClashGEHandle : ClashGEHandles)
			{
				if (AActor* Instigator = OwnerASC->GetActiveGameplayEffect(ClashGEHandle)->Spec.GetContext().Get()->GetInstigator())
				{
					ClashActors.Emplace(Instigator);
				}
			}

			if (OwnerASC && IsValid(OwnerASC) && HitRes.Num() > 0)
			{
				for (auto& res : HitRes)
				{
					if (ApplyedObjs.Contains(res.GetActor()))
						continue;

					ApplyedObjs.AddUnique(res.GetActor());
					if (UAbilitySystemComponent* TargetASC = res.GetActor()->FindComponentByClass<UAbilitySystemComponent>())
					{
						//应用GE
						if (ICSubSystem && ICSubSystem->DamageApplyGE)
						{
							FGameplayEffectSpecHandle GESpecHandle = AttackUtils::MakeAttackGESpecHandle(MeshComp->GetOwner(), ICSubSystem->DamageApplyGE, res, DamageATKCoefficient);
							OwnerASC->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), TargetASC);
						}

					}

					//伤害感知事件发送
					UAISense_Damage::ReportDamageEvent(MeshComp->GetOwner()->GetWorld(), res.GetActor(), MeshComp->GetOwner(), 0.f, MeshComp->GetOwner()->GetActorLocation(), res.GetActor()->GetActorLocation());

					//击退
					if(!FMath::IsNearlyEqual(KnockbackDist, 0.0f))
					{
						UCharacterMovementComponent* CharaMoveCom = res.GetActor()->FindComponentByClass<UCharacterMovementComponent>();
						ACharacter* HitCharacter = Cast<ACharacter>(res.GetActor());
						if (CharaMoveCom && HitCharacter)
						{
							FVector LaunchDirect = res.GetActor()->GetActorLocation()- MeshComp->GetOwner()->GetActorLocation();
							HitCharacter->LaunchCharacter(FMath::Sqrt(2 * CharaMoveCom->BrakingDecelerationWalking * KnockbackDist)* LaunchDirect.GetSafeNormal(), true, false);
						}
					}

					//物理混合
					/*if (USkeletalMeshComponent* Skele = res.GetActor()->FindComponentByClass<USkeletalMeshComponent>())
					{
						Skele->SetPhysicsBlendWeight(0.5);
						Skele->SetBodySimulatePhysics(TEXT("Pelvis"), false);
					}*/

					//Clash
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

			}
		}

		//通知周期内只执行一次的逻辑
		if (bOnce && bHit)
		{
			bOnce = false;
			//镜头抖动，时间膨胀 等GEGC
			if (ICSubSystem && ICSubSystem->AttackFeedbackGE)
			{
				if (UGameplayEffect* GECDO = Cast<UGameplayEffect>(ICSubSystem->AttackFeedbackGE->GetDefaultObject()))
					OwnerASC->ApplyGameplayEffectToSelf(GECDO, 0, FGameplayEffectContextHandle());
			}

			//缓速 顿感
			UAnimMontage* AnimMon = Cast<UAnimMontage>(Animation);
			if (bSlowdown && AnimMon)
			{
				MeshComp->GetAnimInstance()->Montage_SetPlayRate(AnimMon, SlowdownScale);
				FTimerHandle SlowDownTimer;
				MeshComp->GetWorld()->GetTimerManager().SetTimer(SlowDownTimer, [this, AnimMon, MeshComp]()
					{
						MeshComp->GetAnimInstance()->Montage_SetPlayRate(AnimMon, 1);
					}, SlowdownTime, false);
			}

			//抽帧
			if (bSkipFrames && AnimMon)
			{
				MeshComp->GetAnimInstance()->Montage_Pause(AnimMon);
				float CurPlayTime = MeshComp->GetAnimInstance()->Montage_GetPosition(AnimMon);

				//计算当前帧
				//const FFrameRate FrameRate = AnimMon->GetDataModel()->GetFrameRate();
				////float CurrentFrame = CurPlayTime * FrameRate.AsDecimal();

				//float SkipSeconds = FrameRate.AsDecimal();

				FTimerHandle SkipFrameTimer;
				MeshComp->GetWorld()->GetTimerManager().SetTimer(SkipFrameTimer, [this, AnimMon, MeshComp, CurPlayTime]()
					{
						MeshComp->GetAnimInstance()->Montage_SetPosition(AnimMon, CurPlayTime + SkipSeconds);
						MeshComp->GetAnimInstance()->Montage_Resume(AnimMon);
					}, SkipSeconds, false);
			}

		}
	}

	
}

void UAN_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	bOnce = true;
	AttackTraceSockets.Empty();
	PreSocketLoc.Empty();
	ApplyedObjs.Empty();
}
