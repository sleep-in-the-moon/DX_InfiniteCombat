// Copyright DX_IC


#include "LockComponent.h"
#include "ICAbilitySystemComponent.h"

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
	
}


// Called every frame
void ULockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULockComponent::SetAutoLockEnable(bool Enable)
{
	bAutoTrace = Enable;
}

bool ULockComponent::GetIsLockingOnTarget()
{
	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		return ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget")));
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
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("setLock::ASC not valid")));
		UE_LOG(LogTemp, Warning, TEXT("setLock::ASC not valid"));
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

	}

	return false;
}
