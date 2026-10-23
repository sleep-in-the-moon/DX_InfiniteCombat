// Copyright DX_IC


#include "AI/EnvQueryGenerator_StrafeSides.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "NavigationSystem.h"

UEnvQueryGenerator_StrafeSides::UEnvQueryGenerator_StrafeSides(const FObjectInitializer& ObjectInitializer)
{
	ItemType = UEnvQueryItemType_Point::StaticClass();//表示 Generator 生成的候选项究竟是什么类型的数据，主要有 Point, Actor, Direction
}

void UEnvQueryGenerator_StrafeSides::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
    TArray<AActor*> ContextActors;

    if (!StrafeSidesCenterActorContext || !QueryInstance.PrepareContext(StrafeSidesCenterActorContext.Get(), ContextActors) || ContextActors.Num() == 0 || !IsValid(ContextActors[0]))
    {
        return;
    }
    AActor* ContextActor = ContextActors[0];

    

    TArray<FNavLocation> CandidatePoints;
    CandidatePoints.Reserve(2);

    CandidatePoints.Emplace(ContextActor->GetActorLocation() + ContextActor->GetActorRightVector()* StrafeDistance);
    CandidatePoints.Emplace(ContextActor->GetActorLocation() - ContextActor->GetActorRightVector()* StrafeDistance);

    //投影到导航网格并筛选
    ProjectAndFilterNavPoints(CandidatePoints, QueryInstance);
    //交给 Tests
    StoreNavPoints(CandidatePoints, QueryInstance);

}
