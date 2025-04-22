// Copyright DX_IC


#include "ICDataAsset.h"
#include "ICAssetManager.h"

const UICDataAsset& UICDataAsset::Get()
{
	return UICAssetManager::Get().GeICData();
}
