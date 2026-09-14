// Copyright DX_IC


#include "Animation/AN_AttackTrace.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "KismetTraceUtils.h"
//#include "Kismet/KismetSystemLibrary.h"
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
#include "ICComponents/AttackComponent.h"


void UAN_AttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ApplyedObjs.Empty();
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UCombatCharacterComponent* combatCom = Owner->FindComponentByClass<UCombatCharacterComponent>())
		{
			CurWeapon = combatCom->GetWeaponMeshComponent();
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

	OwnerASC = MeshComp->GetOwner()->FindComponentByClass<UICAbilitySystemComponent>();
	AttackComp = MeshComp->GetOwner()->FindComponentByClass<UAttackComponent>(); 
	AttackComp->CreateClashWindow();
}

void UAN_AttackTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!IsValid(CurWeapon) || CurWeapon==nullptr || AttackTraceSockets.Num() == 0 || !AttackComp || !OwnerASC)
		return;
	
	static TArray<struct FHitResult> HitResArray;
	HitResArray.Reset();

	FAttackTraceParam AttackTraceParam(AttackTraceShape, BoxHalf, TraceObjectTypes);
	if(!AttackComp->TraceBySocketsForTick(CurWeapon, AttackTraceSockets, PreSocketLoc, HitResArray, AttackTraceParam))
		return;

	for (const FHitResult& HitRes : HitResArray)//对每一个命中的 actor 执行的逻辑
	{
		if (!HitRes.bBlockingHit || !HitRes.GetActor())
			continue;

		if (ApplyedObjs.Contains(HitRes.GetActor()))//一次通知对同一 Actor 只执行一次
			continue;

		AttackComp->ApplyHitResToTargetActor(HitRes, DamageATKCoefficient, KnockbackDist);

		ApplyedObjs.AddUnique(HitRes.GetActor());
	}
	
	//通知周期内只执行一次的逻辑
	if (bOnce)
	{
		AttackComp->HitFeedback(Cast<UAnimMontage>(Animation), bSlowdown, SlowdownScale, SlowdownTime, bSkipFrames, SkipSeconds);
		bOnce = false;
	}
	
}

void UAN_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	bOnce = true;
	AttackTraceSockets.Empty();
	PreSocketLoc.Empty();
	ApplyedObjs.Empty();
	if (AttackComp)
	{
		AttackComp->CloseClashWindow();
	}
}
