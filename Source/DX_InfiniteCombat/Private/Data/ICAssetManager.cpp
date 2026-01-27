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

const UICDataAsset& UICAssetManager::GetICDataAsset()
{
    return GetOrLoadTypedGameData<UICDataAsset>(ICGameDataPath);
}

const UWeaponDataAsset& UICAssetManager::GetWeaponDataAsset()
{
    return GetOrLoadTypedGameData<UWeaponDataAsset>(WeaponDataPath);
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

            Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset());
        }
    }
    if (ensureAlways(Asset))
    {
        GameDataMap.Add(DataClass, Asset);
    }

    return Asset;
}

UObject* UICAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
    if (AssetPath.IsValid())
    {
        if (UAssetManager::IsInitialized())
        {
            return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
        }

        // Use LoadObject if asset manager isn't ready yet.
        return AssetPath.TryLoad();
    }

    return nullptr;
}

//void UICAssetManager::StartInitialLoading()
//{
//    Super::StartInitialLoading();
//
//    GeICDataAsset();
//}
