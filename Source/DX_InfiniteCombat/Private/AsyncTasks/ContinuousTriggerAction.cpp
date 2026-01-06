// Copyright DX_IC


#include "AsyncTasks/ContinuousTriggerAction.h"

UContinuousTriggerAction* UContinuousTriggerAction::ContinuousTriggerAction(bool& Stop)
{
	UContinuousTriggerAction* Task = NewObject<UContinuousTriggerAction>();
	Task->End = Stop;
	return Task;
}

void UContinuousTriggerAction::Activate()
{
}
