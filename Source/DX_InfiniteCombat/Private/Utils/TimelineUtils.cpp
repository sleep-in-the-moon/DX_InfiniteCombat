#include "Utils/TimelineUtils.h"
//#include "Components/TimelineComponent.h"

//UCurveFloat* TimelineUtils::MakeFloatCurve(const TArray<FVector2D>& CurveKeys)
//{
//	UCurveFloat* FloatCurve = NewObject<UCurveFloat>();
//
//	for(const FVector2D& Vec2 : CurveKeys)
//		FloatCurve->FloatCurve.UpdateOrAddKey(Vec2.X, Vec2.Y);
//
//	return FloatCurve;
//}

//void TimelineUtils::InitTimelineByFloatCurve(FTimeline& InTimeline, UCurveFloat* FloatCurve, FName UpdateFuncName, FName FinishFuncName, UObject* FuncOwner)
//{
//	if (!FloatCurve || !FuncOwner || UpdateFuncName.IsNone())
//		return;
//
//	InTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
//
//	FOnTimelineFloat OnTimelineUpdate;
//	OnTimelineUpdate.BindUFunction(FuncOwner, UpdateFuncName);
//	InTimeline.AddInterpFloat(FloatCurve, OnTimelineUpdate);
//	//InTimeline.SetTimelineLength(1.0f);
//
//	if (!FinishFuncName.IsNone())
//	{
//		FOnTimelineEvent OnTimelineFinish;
//		OnTimelineFinish.BindUFunction(FuncOwner, FinishFuncName);
//		InTimeline.SetTimelineFinishedFunc(OnTimelineFinish);
//	}
//	
//}
