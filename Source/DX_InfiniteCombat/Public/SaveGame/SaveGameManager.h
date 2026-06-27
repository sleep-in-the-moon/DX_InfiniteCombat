// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameManager.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadSettingsCompleted);

class USaveIndex;
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API USaveGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "PlayerSettings")
	void SavePlayerSettings(); 
	UFUNCTION(BlueprintCallable, Category = "PlayerSettings")
	void LoadPlayerSettings();

	UFUNCTION(BlueprintCallable, Category = "AutoSave")
	void AutoSave();
	UFUNCTION(BlueprintCallable, Category = "AutoLoad")
	void AutoLoad();

private:
	//加载已经保存好的元数据文件，如果本地没有则创建一个新的
	void LoadOrCreateSaveIndex();
	// SaveIndex 与本地实际存档文件做校验，保证一致性
	void CalibrateSaveIndex();

private:
	const int32 UserIndex = 0;
	const FString PlayerSettingsSlotName = "PlayerSettings";
	const FString SlotIndexSlotName = "SlotIndex";

	const FString GameDataSlotPrefix = "GameData";
	const uint8 GameDataAutoSlotMaxCount = 3;

	//各存档的辅助元数据文件
	UPROPERTY()
	USaveIndex* SaveIndex = nullptr;
};
