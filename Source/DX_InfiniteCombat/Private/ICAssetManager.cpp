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
//    // TODO: �ڴ˴����� return ���
//}
