// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameBase.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDateTime SaveTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString SaveVersion;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayTimeSeconds = 0.0f;

};
