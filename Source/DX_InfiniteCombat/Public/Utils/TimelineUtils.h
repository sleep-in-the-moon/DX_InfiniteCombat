#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Components/TimelineComponent.h"

//struct FTimeline;

namespace TimelineUtils
{

	static UCurveFloat* MakeFloatCurve(const TArray<FVector2D>& CurveKeys = TArray<FVector2D>({ FVector2D(0, 0), FVector2D(1, 1) }))
	{
		UCurveFloat* FloatCurve = NewObject<UCurveFloat>();

		for (const FVector2D& Vec2 : CurveKeys)
			FloatCurve->FloatCurve.UpdateOrAddKey(Vec2.X, Vec2.Y);

		return FloatCurve;
	}

	// 记得Tick中添加Timeline.TickTimeline(DeltaTime);
	static void InitTimelineByFloatCurve(FTimeline& InTimeline, UCurveFloat* FloatCurve, FName UpdateFuncName, FName FinishFuncName, UObject* FuncOwner)
	{
		if (!FloatCurve || !FuncOwner || UpdateFuncName.IsNone())
			return;

		InTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		FOnTimelineFloat OnTimelineUpdate;
		OnTimelineUpdate.BindUFunction(FuncOwner, UpdateFuncName);
		InTimeline.AddInterpFloat(FloatCurve, OnTimelineUpdate);
		//InTimeline.SetTimelineLength(1.0f);

		if (!FinishFuncName.IsNone())
		{
			FOnTimelineEvent OnTimelineFinish;
			OnTimelineFinish.BindUFunction(FuncOwner, FinishFuncName);
			InTimeline.SetTimelineFinishedFunc(OnTimelineFinish);
		}
	}

}
