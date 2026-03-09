// Copyright DX_IC


#include "GAS/GA/GA_Jump.h"
#include "Character/DX_ICCharacter.h"


void UGA_Jump::JumpStart()
{
	if (ADX_ICCharacter* Character = Cast<ADX_ICCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->Jump();
	}
}

void UGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ADX_ICCharacter* Character = Cast<ADX_ICCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->StopJumping();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
