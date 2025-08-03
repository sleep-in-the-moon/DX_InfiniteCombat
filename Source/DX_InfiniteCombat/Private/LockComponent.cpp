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

void ULockComponent::SetLockEnable(bool Enable)
{
	bEnableLock = Enable;
}

bool ULockComponent::GetIsLockOnTarget()
{
	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		return ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.OnLockTarget")));
	}

	return false;
}

void ULockComponent::SetIsLockOnTarget(bool IsLockOn)
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
