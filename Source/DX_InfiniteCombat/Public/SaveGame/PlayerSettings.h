// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/SaveGameBase.h"
#include "PlayerSettings.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UPlayerSettings : public USaveGameBase
{
	GENERATED_BODY()
	
public:
	//转向灵敏度 垂直
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "PlayerControllSetting")
	float VerticalSensitivity = 1.0f;
	//转向灵敏度 水平
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "PlayerControllSetting")
	float HorizontalSensitivity = 1.0f;

};
