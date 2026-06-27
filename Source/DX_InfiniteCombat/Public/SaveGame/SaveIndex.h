// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveIndex.generated.h"

UENUM(BlueprintType)
enum class ESaveSlotType : uint8
{
	Unknow,
	Auto,
	Manual
};

USTRUCT(BlueprintType)
struct FSaveSlotInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString SlotName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString DisplayName;
	UPROPERTY()
	ESaveSlotType SlotType;
	UPROPERTY()
	FDateTime SaveTime;
	UPROPERTY()
	FString SaveVersion;
	UPROPERTY()
	float PlayTimeSeconds=0.0f;
	//缩略图路径
	UPROPERTY()
	FString ThumbnailPath;
};

/**
 * 各存档的元数据文件
 */
UCLASS()
class DX_INFINITECOMBAT_API USaveIndex : public USaveGame
{
	GENERATED_BODY()
	
public:
	//<SlotName, FSaveSlotInfo>
	UPROPERTY()
	TMap<FString, FSaveSlotInfo> Slots;

};
