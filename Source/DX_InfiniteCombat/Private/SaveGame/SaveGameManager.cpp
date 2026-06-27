// Copyright DX_IC


#include "SaveGame/SaveGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerSettings.h"
#include "Character/DX_ICPlayerController.h"
#include "GameFramework/Character.h"
#include "SaveGame/ICSaveGame.h"
#include "SaveGame/SaveIndex.h"
#include "SaveGame/SaveGameBase.h"

void USaveGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadOrCreateSaveIndex();
	ensure(SaveIndex);
	CalibrateSaveIndex();

	//LoadPlayerSettings();
}

void USaveGameManager::SavePlayerSettings()
{
	UPlayerSettings* playerSetting = Cast<UPlayerSettings>(UGameplayStatics::CreateSaveGameObject(UPlayerSettings::StaticClass()));
	ADX_ICPlayerController* playerController = Cast<ADX_ICPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!playerSetting || !playerController)
		return;

	playerSetting->VerticalSensitivity = playerController->VerticalSensitivity;
	playerSetting->HorizontalSensitivity = playerController->HorizontalSensitivity;
	//...

	UGameplayStatics::SaveGameToSlot(playerSetting, PlayerSettingsSlotName, UserIndex);
}

void USaveGameManager::LoadPlayerSettings()
{
	UPlayerSettings* playerSetting = Cast<UPlayerSettings>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSlotName, UserIndex));
	ADX_ICPlayerController* playerController = Cast<ADX_ICPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!playerSetting || !playerController)
		return;

	playerController->VerticalSensitivity = playerSetting->VerticalSensitivity;
	playerController->HorizontalSensitivity = playerSetting->HorizontalSensitivity;
}

void USaveGameManager::AutoSave()
{
	const FString AutoSlot = "";
	UICSaveGame* ICSaveGame = Cast<UICSaveGame>(UGameplayStatics::CreateSaveGameObject(UICSaveGame::StaticClass()));
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!ICSaveGame || !Character)
		return;

	ICSaveGame->PlayerData.PlayerTransform = Character->GetActorTransform();
	//...

	UGameplayStatics::SaveGameToSlot(ICSaveGame, AutoSlot, UserIndex);
}

void USaveGameManager::AutoLoad()
{
	const FString NewestSlot = "";
	UICSaveGame* ICSaveGame = Cast<UICSaveGame>(UGameplayStatics::LoadGameFromSlot(NewestSlot, UserIndex));
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!ICSaveGame || !Character)
		return;

	Character->SetActorTransform(ICSaveGame->PlayerData.PlayerTransform);
}

void USaveGameManager::LoadOrCreateSaveIndex()
{
	SaveIndex = Cast<USaveIndex>(UGameplayStatics::LoadGameFromSlot(SlotIndexSlotName, UserIndex));
	if (!SaveIndex)
	{
		SaveIndex = Cast<USaveIndex>(UGameplayStatics::CreateSaveGameObject(USaveIndex::StaticClass()));
	}
}

void USaveGameManager::CalibrateSaveIndex()
{
	const FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames/");
	IFileManager& FileManager = IFileManager::Get();

	TArray<FString> FoundFiles;
	FileManager.FindFiles(FoundFiles, *(SaveDir / TEXT("*.sav")), true, false);

	TSet<FString> ActualSlots;
	for (const FString& FoundFile : FoundFiles)
	{
		FString FileName = FPaths::GetBaseFilename(FoundFile);
		if (FileName.Equals(SlotIndexSlotName) || FileName.Equals(PlayerSettingsSlotName)) continue;
		ActualSlots.Add(FoundFile);
	}

	//清除多余的
	TArray<FString> RemoveSlots;
	for (auto& Pair : SaveIndex->Slots)
	{
		if (!ActualSlots.Contains(Pair.Key))
		{
			RemoveSlots.Add(Pair.Key);
		}
	}
	for (const FString& RemoveSlot : RemoveSlots)
	{
		SaveIndex->Slots.Remove(RemoveSlot);
	}

	//补上缺少的
	for (const FString& ActualSlot : ActualSlots)
	{
		if (!SaveIndex->Slots.Contains(ActualSlot))
		{
			USaveGameBase* AddSaveGame = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(ActualSlot, UserIndex));
			if (!AddSaveGame)
				continue;

			FSaveSlotInfo SlotInfo;
			SlotInfo.SlotName = ActualSlot;
			SlotInfo.SaveVersion = AddSaveGame->SaveVersion;
			SlotInfo.SaveTime = AddSaveGame->SaveTime;
			SlotInfo.PlayTimeSeconds = AddSaveGame->PlayTimeSeconds;
			SlotInfo.DisplayName = ActualSlot;
			SlotInfo.SlotType = ESaveSlotType::Unknow;

			SaveIndex->Slots.Add(ActualSlot, SlotInfo);
		}
	}
}
