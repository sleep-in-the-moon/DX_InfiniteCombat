// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ICWorldSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMDG_DebugChange, bool, showDebug);

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	bool GetShowDebug() const
	{
		return bShowDebug;
	}
	UFUNCTION(BlueprintCallable)
	void SetShowDebug(bool ShowDebug);

	UPROPERTY(BlueprintAssignable)
	FMDG_DebugChange MDG_ShowDebugChange;

private:
	bool bShowDebug=false;

};
