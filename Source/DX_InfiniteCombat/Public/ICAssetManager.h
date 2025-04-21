// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ICAssetManager.generated.h"


class UICDataAsset;
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UICAssetManager& Get();
	//const UICDataAsset& GeICData();

};
