// Copyright DX_IC


#include "Data/ICDataAsset.h"
#include "Data/ICAssetManager.h"

const UICDataAsset& UICDataAsset::Get()
{
	return UICAssetManager::Get().GeICDataAsset();
}
