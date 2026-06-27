// Copyright DX_IC


#include "ICComponents/ExecuteComponent.h"
#include "LockSystem/WeakLockComponent.h"
#include "GAS/ICAbilitySystemComponent.h"
#include "GAS/CharacterAttributeSet.h"

// Sets default values for this component's properties
UExecuteComponent::UExecuteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UExecuteComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (UICAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UICAbilitySystemComponent>())
	{
		ASC->HelthChangeBPDG.AddUniqueDynamic(this, &UExecuteComponent::CheckExecuteLine);
	}
	
}


// Called every frame
void UExecuteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UExecuteComponent::CheckExecuteLine(float NewHelth)
{
	if (ExecuteLine < 0.0f)
		return;

	if (UICAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UICAbilitySystemComponent>())
	{
		float MaxHP = ASC->GetNumericAttribute(UCharacterAttributeSet::GetMaxHPAttribute());
		if ((NewHelth / MaxHP) <= ExecuteLine && !bCanBeExecuted)
		{
			bCanBeExecuted = true;
			//蓝图更新 UI 变化
			DG_CanBeExecutedStateChange.Broadcast(bCanBeExecuted);
		}
		else if((NewHelth / MaxHP) > ExecuteLine && bCanBeExecuted)
		{
			bCanBeExecuted = false;
			DG_CanBeExecutedStateChange.Broadcast(bCanBeExecuted);
		}
	}
}

