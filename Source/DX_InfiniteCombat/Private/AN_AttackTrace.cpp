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

		bHit = MeshComp->GetOwner()->GetWorld()->LineTraceMultiByObjectType(HitRes, *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), ObjectQueryParams, Params);
		//DrawDebugLine(MeshComp->GetOwner()->GetWorld(), *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), FColor::Red, false, 1.5f, 0, 1.0f);
		
		UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(MeshComp->GetOwner()->GetWorld());
		if(ICSubSystem->GetShowDebug())
			DrawDebugLineTraceMulti(MeshComp->GetOwner()->GetWorld(), *PreSocketLoc.Find(socket), CurWeapon->GetSocketLocation(socket), EDrawDebugTrace::Type::ForDuration, bHit, HitRes, FColor::Red, FColor::Green, 1.5f);

		PreSocketLoc.Add(socket, CurWeapon->GetSocketLocation(socket));

		if (bHit)
		{
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
						for (auto& GE : GameplayEffectsToApplyTarget)
						{
							if (GE.EffectToTargets)
							{
								if (UGameplayEffect* GECDO = Cast<UGameplayEffect>(GE.EffectToTargets->GetDefaultObject()))
									OwnerASC->ApplyGameplayEffectToTarget(GECDO, TargetASC, GE.Level);
							}
						}

					}

					//伤害感知事件发送
					UAISense_Damage::ReportDamageEvent(MeshComp->GetOwner()->GetWorld(), res.GetActor(), MeshComp->GetOwner(), 0.f, MeshComp->GetOwner()->GetActorLocation(), res.GetActor()->GetActorLocation());

					//施加推力
					if (UCharacterMovementComponent* CharaMoveCom = res.GetActor()->FindComponentByClass<UCharacterMovementComponent>())
					{
						FVector Impulse = res.ImpactNormal * -1; //+(res.GetActor()->GetActorUpVector() * 300);
						//Impulse = FVector(Impulse.X, Impulse.Y, 0);
						Impulse = FVector(Impulse.X * ImpulseScale.X, Impulse.Y * ImpulseScale.Y, Impulse.Z * ImpulseScale.Z);
						CharaMoveCom->AddImpulse(Impulse, true);
						//DrawDebugDirectionalArrow(MeshComp->GetOwner()->GetWorld(), res.GetActor()->GetActorLocation(), Impulse, 1, FColor::Blue, false, 3, SDPG_World, 1);
					}

					//物理混合
					/*if (USkeletalMeshComponent* Skele = res.GetActor()->FindComponentByClass<USkeletalMeshComponent>())
					{
						Skele->SetPhysicsBlendWeight(0.5);
						Skele->SetBodySimulatePhysics(TEXT("Pelvis"), false);
					}*/
					//..其它

				}

			}
		}

		//通知周期内只执行一次的逻辑
		if (bOnce && bHit)
		{
			bOnce = false;
			//镜头抖动，时间膨胀 等GEGC
			for (auto& GE : GameplayEffectsToApplySource)
			{
				if (GE.EffectToTargets)
				{
					if (UGameplayEffect* GECDO = Cast<UGameplayEffect>(GE.EffectToTargets->GetDefaultObject()))
						OwnerASC->ApplyGameplayEffectToSelf(GECDO, GE.Level, FGameplayEffectContextHandle());
				}
			}

			//减速 顿感
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
