// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/DataAsset.h"
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
	const UICDataAsset& GeICData();

protected:
	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const* pResult = GameDataMap.Find(GameDataClass::StaticClass()))
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		// Does a blocking load if needed
		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}

protected:
	UPROPERTY(Config)
	TSoftObjectPtr<UICDataAsset> ICGameDataPath;
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

};
