// Copyright DX_IC


#include "ICComponents/ICMotionWarpingComponent.h"
#include "MotionWarping/Public/MotionWarpingCharacterAdapter.h"

FTransform UICMotionWarpingComponent::CalculateRootTransformRelativeToWarpPointAtTime(const UMotionWarpingBaseAdapter& WarpingAdapter, const UAnimSequenceBase* Animation, float Time, const FName& WarpPointBoneName)
{
	if (const USkeletalMeshComponent* Mesh = WarpingAdapter.GetMesh())
	{
		if (const UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			const FBoneContainer& FullBoneContainer = AnimInstance->GetRequiredBones();
			const int32 BoneIndex = FullBoneContainer.GetPoseBoneIndexForBoneName(WarpPointBoneName);
			if (BoneIndex != INDEX_NONE)
			{
				TArray<FBoneIndexType> RequiredBoneIndexArray = { 0, (FBoneIndexType)BoneIndex };
				FullBoneContainer.GetReferenceSkeleton().EnsureParentsExistAndSort(RequiredBoneIndexArray);

				FBoneContainer LimitedBoneContainer(RequiredBoneIndexArray, UE::Anim::FCurveFilterSettings(UE::Anim::ECurveFilterMode::DisallowAll), *FullBoneContainer.GetAsset());

				FCSPose<FCompactPose> Pose;
				UMotionWarpingUtilities::ExtractComponentSpacePose(Animation, LimitedBoneContainer, Time, false, Pose);

				// Inverse of mesh's relative rotation. Used to convert root and warp point in the animation from Y forward to X forward
				const FTransform MeshCompRelativeRotInverse = FTransform(WarpingAdapter.GetBaseVisualRotationOffset().Inverse());

				const FTransform RootTransform = MeshCompRelativeRotInverse * Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(0));
				const FTransform WarpPointTransform = MeshCompRelativeRotInverse * Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(1));
				UE_LOG(LogTemp, Warning, TEXT("RootTransform::%s"), *RootTransform.GetRotation().Rotator().ToString());
				UE_LOG(LogTemp, Warning, TEXT("WarpPointTranslation::%s"), *WarpPointTransform.GetRotation().Rotator().ToString());
				UE_LOG(LogTemp, Warning, TEXT("RelativeTransform::%s"), *RootTransform.GetRelativeTransform(WarpPointTransform).GetRotation().Rotator().ToString());
				return RootTransform.GetRelativeTransform(WarpPointTransform);//root 在 WarpPoint 局部空间中的位置
				//return WarpPointTransform.GetRelativeTransform(RootTransform);//WarpPoint 在 root 局部空间中的位置
			}
		}
	}

	return FTransform::Identity;
}
