// Copyright DX_IC


#include "Data/ICAssetManager.h"

UICAssetManager& UICAssetManager::Get()
{
    check(GEngine);

    if (UICAssetManager* Singleton = Cast<UICAssetManager>(GEngine->AssetManager))
    {
        return *Singleton;
    }

    return *NewObject<UICAssetManager>();
}

const UICDataAsset& UICAssetManager::GeICData()
{
    return GetOrLoadTypedGameData<UICDataAsset>(ICGameDataPath);
}

void UICAssetManager::AddLoadedAsset(const UObject* Asset)
{
    if (ensureAlways(Asset))
    {
        FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
        LoadedAssets.Add(Asset);
    }
}

UPrimaryDataAsset* UICAssetManager::LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType)
{
    UPrimaryDataAsset* Asset = nullptr;

    if (!DataClassPath.IsNull())
    {
        UE_LOG(LogTemp, Log, TEXT("Loading GameData: %s ..."), *DataClassPath.ToString());
        TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
        if (Handle.IsValid())
        {
            Handle->WaitUntilComplete(0.0f, false);//等待资源加载完毕，0为无限等待，即同步

            // This should always work
            Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset());
        }
    }
    if (Asset)
    {
        GameDataMap.Add(DataClass, Asset);
    }
    else
    {
        // It is not acceptable to fail to load any GameData asset. It will result in soft failures that are hard to diagnose.
        UE_LOG(LogTemp, Fatal, TEXT("Failed to load GameData asset at %s. Type %s. This is not recoverable and likely means you do not have the correct data to run %s."), *DataClassPath.ToString(), *PrimaryAssetType.ToString(), FApp::GetProjectName());
    }

    return Asset;
}
