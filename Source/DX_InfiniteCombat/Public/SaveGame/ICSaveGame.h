// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/SaveGameBase.h"
#include "ICSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "PlayerData")
	FTransform PlayerTransform = FTransform(FRotator::ZeroRotator, FVector(194.305333, -1423.583481, 1831.790613));
};
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICSaveGame : public USaveGameBase
{
	GENERATED_BODY()
	
public:
	FPlayerData PlayerData;

};
