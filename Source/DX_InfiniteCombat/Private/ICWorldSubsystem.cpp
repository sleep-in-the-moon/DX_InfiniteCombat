// Copyright DX_IC


#include "ICWorldSubsystem.h"
#include "GameplayEffect.h"

UICWorldSubsystem::UICWorldSubsystem()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> DamageGEClass(TEXT("/Game/Character/GAS/GE/GE_DamageApply.GE_DamageApply_C"));
	if (DamageGEClass.Succeeded())
	{
		DamageApplyGE = DamageGEClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameplayEffect> FeedbackGEClass(TEXT("/Game/Character/GAS/GE/GE_HitFeedback.GE_HitFeedback_C"));
	if (FeedbackGEClass.Succeeded())
	{
		AttackFeedbackGE = FeedbackGEClass.Class;
	}
}

void UICWorldSubsystem::SetShowDebug(bool ShowDebug)
{
	bool PreState = bShowDebug;
	bShowDebug = ShowDebug;

	if (PreState != ShowDebug)
		MDG_ShowDebugChange.Broadcast(ShowDebug);

}
