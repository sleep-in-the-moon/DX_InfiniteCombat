// Copyright DX_IC


#include "LockComponent.h"
#include "ICAbilitySystemComponent.h"
#include "ICWorldSubsystem.h"
#include "KismetTraceUtils.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
#include "UMG/WidgetCombatStates.h"

// Sets default values for this component's properties
ULockComponent::ULockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (APawn* ownerP = Cast<APawn>(GetOwner()))
	{
		APlayerController* playerControl = Cast<APlayerController>(ownerP->GetController());
		if (playerControl && LockedWidgetClass)
		{
			LockedWidget = CreateWidget<UUserWidget>(playerControl, LockedWidgetClass);

			FProperty* CombatStatesWidgetPro = playerControl->GetClass()->FindPropertyByName(TEXT("CombatStatesWidget"));
			if (CombatStatesWidgetPro)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("CombatStatesWidgetClass::%s"), *CombatStatesWidgetPro->GetCPPType()));
				UE_LOG(LogTemp, Warning, TEXT("CombatStatesWidgetClass::%s"), *CombatStatesWidgetPro->GetCPPType());

				FClassProperty* CombatStatesWidgetClassPro = static_cast<FClassProperty*>(CombatStatesWidgetPro);
				const void* ValuePtr = CombatStatesWidgetClassPro->ContainerPtrToValuePtr<void>(playerControl);
				TObjectPtr<UObject> CombatStatesWidgetObj = CombatStatesWidgetClassPro->GetPropertyValue(ValuePtr);

				if (Cast<UWidgetCombatStates>(CombatStatesWidgetObj))
					CombatStatesWidget = Cast<UWidgetCombatStates>(CombatStatesWidgetObj);
			}
		}
	}
}


// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetIsLockingOnTarget() && GetOwner())
	{
		FRotator LookRot = FRotationMatrix::MakeFromX(CurLockActor->GetActorLocation() - GetOwner()->GetActorLocation()).Rotator();

		GetOwner()->SetActorRotation(FRotator(GetOwner()->GetActorRotation().Pitch, LookRot.Yaw, GetOwner()->GetActorRotation().Roll));

		if (APawn* ownerP = Cast<APawn>(GetOwner()))
		{
			ownerP->GetController()->SetControlRotation(FRotator(LookRot.Pitch, LookRot.Yaw, ownerP->GetController()->GetControlRotation().Roll));
		}
		
	}
	UpdateLockState();
}

void ULockComponent::SetAutoLockEnable(bool Enable)
{
	if (bAutoTrace == Enable)
		return;

	bAutoTrace = Enable;

	if (bAutoTrace && GetOwner())
	{
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(AutoTraceTimer, [this]() {
			DoOnceTrace();
		}, AutoTraceInterval, true);
	}
	else if(!bAutoTrace && AutoTraceTimer.IsValid() && GetOwner())
	{
		GetOwner()->GetWorld()->GetTimerManager().ClearTimer(AutoTraceTimer); 
		AutoTraceTimer.Invalidate();
	}
}

bool ULockComponent::GetIsLockingOnTarget()
{
	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		return ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget"))) && CurLockActor;
	}

	return false;
}

void ULockComponent::SetIsLockingOnTarget(bool IsLockOn)
{
	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		if(IsLockOn)
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget")));
		else
		{
			//CurLockActor = nullptr;
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("setLock::ASC not valid")));
		UE_LOG(LogTemp, Warning, TEXT("setLock::ASC not valid"));
	}
	
}

void ULockComponent::WhenLockingOnActor(AActor* LockedActor)
{
	if (CurLockActor)
	{
		//处理旧锁定目标上的逻辑
	}
	SetIsLockingOnTarget(true);
	CurLockActor = LockedActor;
	//处理新锁定目标上的逻辑
	if (CurLockActor && CombatStatesWidget.IsValid() && LockedWidget)
		CombatStatesWidget->RegisterPersistentWidget(LockedWidgetPersistentID, FPersistentWidgetHandle(LockedWidget, CurLockActor));
}

void ULockComponent::UpdateLockState()
{
	if (CurLockActor)
	{
		UAbilitySystemComponent* LockActorASC = CurLockActor->FindComponentByClass<UAbilitySystemComponent>();
		if (LockActorASC)
		{
			if (LockActorASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Died")))
			{
				SetIsLockingOnTarget(false);
				CurLockActor = nullptr;

				if (CombatStatesWidget.IsValid())
					CombatStatesWidget->UnRegisterPersistentWidget(LockedWidgetPersistentID);
			}
		}
	}

}

void ULockComponent::AddToCandidateActors(AActor* AddActor, float score)
{
	CandidateActors.Add(AddActor, score);
}

void ULockComponent::AppendCandidateActors(const TMap<AActor*, float>& AddActors)
{
	CandidateActors.Append(AddActors);
}

bool ULockComponent::DoOnceTrace()
{
	TArray<FHitResult> OutHits;
	float TraceHalf=500.f, TraceDist=7000.f;

	if (APawn* pawn = Cast<APawn>(GetOwner()))
	{
		APlayerController* playerController =  Cast<APlayerController>(pawn->GetController());
		if(!playerController)
			return false;
		
		FVector StartLoc = playerController->PlayerCameraManager->GetCameraLocation();
		FRotator CameraYaw = FRotator(0, playerController->PlayerCameraManager->GetCameraRotation().Yaw, 0);
		FVector EndLoc = StartLoc + FRotationMatrix(CameraYaw).GetUnitAxis(EAxis::X) * TraceDist;

		FCollisionShape TraceShape = FCollisionShape::MakeSphere(TraceHalf);

		UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
		if (ICSubSystem && ICSubSystem->GetShowDebug())
			DrawDebugSphereTraceMulti(GetOwner()->GetWorld(), StartLoc, EndLoc, TraceHalf, EDrawDebugTrace::Type::ForDuration, false, OutHits, FLinearColor::Blue, FLinearColor::Green, 10.0f);

		FCollisionQueryParams CollisionQueryParams;
		FCollisionObjectQueryParams CollisionObjectQueryParams;
		UDX_StaticFunlib::MakeCollisionParam(GetOwner(), CollisionQueryParams, CollisionObjectQueryParams, TArray<AActor*>(), TraceObjectTypes);

		bool const bHit = GetOwner()->GetWorld()->SweepMultiByObjectType(OutHits, StartLoc, EndLoc, FQuat::Identity, CollisionObjectQueryParams, TraceShape, CollisionQueryParams);

		if (bHit)
		{
			WhenLockingOnActor(OutHits.Last().GetActor());
		}

		return bHit;
	}

	return false;
}
