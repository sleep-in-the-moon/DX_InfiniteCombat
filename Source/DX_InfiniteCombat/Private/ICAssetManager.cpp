// Copyright DX_IC


#include "ICAssetManager.h"

UICAssetManager& UICAssetManager::Get()
{
    check(GEngine);

    if (UICAssetManager* Singleton = Cast<UICAssetManager>(GEngine->AssetManager))
    {
        return *Singleton;
    }

    return *NewObject<UICAssetManager>();
}

//const UICDataAsset& UICAssetManager::GeICData()
//{
//    // TODO: 在此处插入 return 语句
//}
