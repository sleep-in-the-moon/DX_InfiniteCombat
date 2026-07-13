// Copyright DX_IC


#include "Animation/RootMotionModifier_ICSkewWarp.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "MotionWarpingAdapter.h"
#include "ICComponents/ICMotionWarpingComponent.h"
#if WITH_EDITOR
#include "Animation/AnimInstance.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "AnimPreviewInstance.h"
#include "SceneManagement.h"
#include "BonePose.h"
#include "Engine/Font.h"
#include "CanvasTypes.h"
#include "Animation/AnimSequenceHelpers.h"
#endif

//FTransform URootMotionModifier_ICSkewWarp::ProcessRootMotion(const FTransform& InRootMotion, float DeltaSeconds)
//{
//	const UMotionWarpingBaseAdapter* OwnerAdapter = GetOwnerAdapter();
//	const AActor* OwnerAsActor = nullptr;
//
//	if (OwnerAdapter)
//	{
//		OwnerAsActor = OwnerAdapter->GetActor();
//	}
//
//	if (!OwnerAdapter || !OwnerAsActor)
//	{
//		return InRootMotion;
//	}
//
//	FTransform FinalRootMotion = InRootMotion;
//
//	const FTransform RootMotionTotal = UMotionWarpingUtilities::ExtractRootMotionFromAnimation(Animation.Get(), PreviousPosition, EndTime);
//	const FTransform RootMotionDelta = UMotionWarpingUtilities::ExtractRootMotionFromAnimation(Animation.Get(), PreviousPosition, FMath::Min(CurrentPosition, EndTime));
//
//	FTransform ExtraRootMotion = FTransform::Identity;
//	if (CurrentPosition > EndTime)
//	{
//		ExtraRootMotion = UMotionWarpingUtilities::ExtractRootMotionFromAnimation(Animation.Get(), EndTime, CurrentPosition);
//	}
//
//	if (bWarpTranslation)
//	{
//		const FVector CurrentLocation = OwnerAdapter->GetVisualRootLocation();
//		const FQuat CurrentRotation = OwnerAdapter->GetActor()->GetActorQuat();
//		const FVector MeshTranslationOffset = OwnerAdapter->GetBaseVisualTranslationOffset();
//		const FQuat MeshRotationOffset = OwnerAdapter->GetBaseVisualRotationOffset();
//
//		const FVector DeltaTranslation = RootMotionDelta.GetLocation();
//		const FVector TotalTranslation = RootMotionTotal.GetLocation();
//
//		FVector TargetLocation = GetTargetLocation();
//		if (bIgnoreZAxis)
//		{
//			TargetLocation.Z = CurrentLocation.Z;
//		}
//
//		// if there is translation in the animation, warp it
//		if (!TotalRootMotionWithinWindow.GetTranslation().IsNearlyZero())
//		{
//			if (!DeltaTranslation.IsNearlyZero())
//			{
//				const FTransform MeshTransform = FTransform(MeshRotationOffset, MeshTranslationOffset) * OwnerAsActor->GetActorTransform();
//				TargetLocation = MeshTransform.InverseTransformPositionNoScale(TargetLocation);
//
//				const FVector WarpedTranslation = WarpTranslation(FTransform::Identity, DeltaTranslation, TotalTranslation, TargetLocation) + ExtraRootMotion.GetLocation();
//				FinalRootMotion.SetTranslation(WarpedTranslation);
//			}
//		}
//		// if there is no translation in the animation, add it
//		else
//		{
//			const FVector DeltaToTarget = TargetLocation - CurrentLocation;
//			if (DeltaToTarget.IsNearlyZero())
//			{
//				FinalRootMotion.SetTranslation(FVector::ZeroVector);
//			}
//			else
//			{
//				float Alpha = FMath::Clamp((CurrentPosition - ActualStartTime) / (EndTime - ActualStartTime), 0.f, 1.f);
//				Alpha = FAlphaBlend::AlphaToBlendOption(Alpha, AddTranslationEasingFunc, AddTranslationEasingCurve);
//
//				const FVector NextLocation = FMath::Lerp<FVector, float>(StartTransform.GetLocation(), TargetLocation, Alpha);
//				FVector FinalDeltaTranslation = (NextLocation - CurrentLocation);
//				FinalDeltaTranslation = (CurrentRotation.Inverse() * DeltaToTarget.ToOrientationQuat()).GetForwardVector() * FinalDeltaTranslation.Size();
//				FinalDeltaTranslation = MeshRotationOffset.UnrotateVector(FinalDeltaTranslation);
//
//				FinalRootMotion.SetTranslation(FinalDeltaTranslation + ExtraRootMotion.GetLocation());
//			}
//		}
//	}
//
//	if (bWarpRotation)
//	{
//		const FQuat WarpedRotation = ExtraRootMotion.GetRotation() * WarpRotation(RootMotionDelta, RootMotionTotal, DeltaSeconds);
//		FinalRootMotion.SetRotation(WarpedRotation);
//	}
//
//	// Debug
//#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
//	const int32 DebugLevel = FMotionWarpingCVars::CVarMotionWarpingDebug.GetValueOnGameThread();
//	if (DebugLevel == 1 || DebugLevel == 3)
//	{
//		PrintLog(TEXT("SkewWarp"), InRootMotion, FinalRootMotion);
//	}
//
//	if (DebugLevel == 2 || DebugLevel == 3)
//	{
//		const float DrawDebugDuration = FMotionWarpingCVars::CVarMotionWarpingDrawDebugDuration.GetValueOnGameThread();
//		DrawDebugCoordinateSystem(OwnerAsActor->GetWorld(), GetTargetLocation(), GetTargetRotator(), 50.f, false, DrawDebugDuration, 0, 1.f);
//	}
//#endif
//
//	return FinalRootMotion;
//}

void URootMotionModifier_ICSkewWarp::Update(const FMotionWarpingUpdateContext& Context)
{
	// Update playback times and state
	//Super::Update(Context);
	URootMotionModifier::Update(Context);

	// Cache sync point transform and trigger OnTargetTransformChanged if needed
	const UMotionWarpingComponent* OwnerComp = GetOwnerComponent();
	if (OwnerComp && GetState() == ERootMotionModifierState::Active)
	{
		const FMotionWarpingTarget* WarpTargetPtr = OwnerComp->FindWarpTarget(WarpTargetName);

		// Disable if there is no target for us
		if (WarpTargetPtr == nullptr)
		{

			SetState(ERootMotionModifierState::Disabled);
			return;
		}

		// Get the warp point sent by the game
		FTransform WarpPointTransformGame = WarpTargetPtr->GetTargetTrasform();

		// Initialize our target transform (where the root should end at the end of the window) with the warp point sent by the game
		FTransform TargetTransform = WarpPointTransformGame;

		// Check if a warp point is defined in the animation. If so, we need to extract it and offset the target transform 
		// the same amount the root bone is offset from the warp point in the animation
		if (WarpPointAnimProvider != EWarpPointAnimProvider::None)
		{
			if (!CachedOffsetFromWarpPoint.IsSet())
			{
				if (const UMotionWarpingBaseAdapter* OwnerAdapter = GetOwnerAdapter())
				{
					if (WarpPointAnimProvider == EWarpPointAnimProvider::Static)
					{
						CachedOffsetFromWarpPoint = UMotionWarpingUtilities::CalculateRootTransformRelativeToWarpPointAtTime(*OwnerAdapter, GetAnimation(), EndTime, WarpPointAnimTransform);
					}
					else if (WarpPointAnimProvider == EWarpPointAnimProvider::Bone)
					{
						//CachedOffsetFromWarpPoint = UMotionWarpingUtilities::CalculateRootTransformRelativeToWarpPointAtTime(*OwnerAdapter, GetAnimation(), EndTime, WarpPointAnimBoneName);
						CachedOffsetFromWarpPoint = UICMotionWarpingComponent::CalculateRootTransformRelativeToWarpPointAtTime(*OwnerAdapter, GetAnimation(), EndTime, WarpPointAnimBoneName);
					}
				}
			}

			// Update Target Transform based on the offset between the root and the warp point in the animation
			TargetTransform = CachedOffsetFromWarpPoint.GetValue() * WarpPointTransformGame;
		}

		if (!CachedTargetTransform.Equals(TargetTransform))
		{
			CachedTargetTransform = TargetTransform;

			OnTargetTransformChanged();
		}
	}
}
