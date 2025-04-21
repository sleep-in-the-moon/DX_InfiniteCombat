// Copyright DX_IC


#include "ICWorldSubsystem.h"

void UICWorldSubsystem::SetShowDebug(bool ShowDebug)
{
	bool PreState = bShowDebug;
	bShowDebug = ShowDebug;

	if (PreState != ShowDebug)
		MDG_ShowDebugChange.Broadcast(ShowDebug);

}
