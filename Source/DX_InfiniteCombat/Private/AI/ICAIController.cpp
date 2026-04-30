// Copyright DX_IC


#include "AI/ICAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"

AICAIController::AICAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
}

void AICAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetGenericTeamId(FGenericTeamId(1));

	RunBehaviorTree(BTAsset);

	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AICAIController::TargetPerceptionUpdated);
	}
}

void AICAIController::UpdateDeadKey(bool IsDead)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(TEXT("IsDead"), IsDead);
	}
}

void AICAIController::UpdateTargetKey(AActor* TargetActor)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsObject(TEXT("TargetActor"), TargetActor);
	}
}

void AICAIController::UpdateSightKey(bool HasLineSight)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(TEXT("HasLineSight"), HasLineSight);
	}
}

void AICAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	/*if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{}*/

	if (Stimulus.WasSuccessfullySensed())
	{
		GetWorld()->GetTimerManager().ClearTimer(LoseSightTimer);
		UpdateTargetKey(Actor);
		UpdateSightKey(true);

		return;
	}

	UpdateSightKey(false);
	GetWorld()->GetTimerManager().SetTimer(LoseSightTimer, [this]() 
		{
			UpdateTargetKey(nullptr);
		}
	, LooseSightTime, false);

}
