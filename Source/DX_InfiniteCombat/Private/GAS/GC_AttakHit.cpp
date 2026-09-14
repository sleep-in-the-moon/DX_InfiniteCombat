// Copyright DX_IC


#include "GAS/GC_AttakHit.h"
#include "GAS/ICGameplayEffectTypes.h"
#include "UMG/WidgetCombatStates.h"
#include "Kismet/GameplayStatics.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "GameFramework/Character.h"

UGC_AttakHit::UGC_AttakHit(const FObjectInitializer& ObjectInit): HitSound(nullptr), WidgetClass(UUserWidget::StaticClass()), Super(ObjectInit)//CDO
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.AttackHit"));
}

bool UGC_AttakHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const FICGameplayEffectContext* EffectContext = static_cast<const FICGameplayEffectContext*>(Parameters.EffectContext.Get());

	if (!EffectContext)
		return false;

	//Parameters.GetSourceObject();
	/*if(!EffectContext->GetInstigator())
		return false;*/

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(MyTarget->GetWorld(), 0);
	/*if (!playerPawn)
		playerPawn = Cast<APawn>(MyTarget);*/

	if (playerPawn)
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
					CombatStatesWidget->SpawnTransientWidgetByActor(MyTarget, WidgetClass, 1.4, FString::FromInt(FMath::Floor(EffectContext->GetAttackInfo().GetFinalDamage())));
			}
		}
	}

	//EffectContext->GetAttackInfo().GetFinalDamage();

	if(HitSound)
		UGameplayStatics::PlaySoundAtLocation(MyTarget->GetWorld(), HitSound, MyTarget->GetActorLocation(), MyTarget->GetActorRotation(), 1.0f);

	UCombatCharacterComponent* CombatComp = MyTarget->FindComponentByClass<UCombatCharacterComponent>();
	if (!CombatComp || !CombatComp->GetHurtMontages.IsValid())
		return false;

	UAnimMontage* GetHitMontage = nullptr;
	float ForwardDot = FVector::DotProduct(MyTarget->GetActorForwardVector(), Parameters.EffectContext.GetHitResult()->Normal);
	float RightDot = FVector::DotProduct(MyTarget->GetActorRightVector(), Parameters.EffectContext.GetHitResult()->Normal);

	if (RightDot >= -0.5 && RightDot <= 0.5)	// [60', 120']
	{
		if (ForwardDot > 0)
		{
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("ForwardHit"));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("ForwardHit"));
#endif
			GetHitMontage = CombatComp->GetHurtMontages.ForwardHitedMontage;
		}
		else
		{
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("BackHit"));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("BackHit"));
#endif
			GetHitMontage = CombatComp->GetHurtMontages.BackHitedMontage;
		}
	}
	else
	{
		if (RightDot > 0)
		{
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("RightHit"));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("RightHit"));
#endif
			GetHitMontage = CombatComp->GetHurtMontages.RightHitedMontage;
		}
		else
		{
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("LeftHit"));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("LeftHit"));
#endif
			GetHitMontage = CombatComp->GetHurtMontages.LeftHitedMontage;
		}
	}

	if (!GetHitMontage)
		return false;

	ACharacter* TargetCharacter = Cast<ACharacter>(MyTarget);
	if (!TargetCharacter)
		return false;

	//TargetCharacter->PlayAnimMontage(GetHitMontage);
	UAnimInstance* AnimInstance = (TargetCharacter->GetMesh()) ? TargetCharacter->GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance)
		return false;
	
	AnimInstance->Montage_Play(GetHitMontage, 1.f,EMontagePlayReturnType::MontageLength, 0.f, false);

	return true;
}
