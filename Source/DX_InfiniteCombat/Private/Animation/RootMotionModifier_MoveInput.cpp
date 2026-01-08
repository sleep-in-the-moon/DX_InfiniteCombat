// Copyright DX_IC


#include "Animation/RootMotionModifier_MoveInput.h"
#include "Character/DX_ICPlayerController.h"

FTransform URootMotionModifier_MoveInput::ProcessRootMotion(const FTransform& InRootMotion, float DeltaSeconds)
{
	FTransform Res = InRootMotion;
	if (APawn* pawn = Cast<APawn>(GetActorOwner()))
	{
		ADX_ICPlayerController* Control = Cast<ADX_ICPlayerController>(pawn->GetController());
		if (Control)
		{
			/*Control->GetMoveInput()
			Res.SetLocation();*/
		}
	}

	return Res;
}
