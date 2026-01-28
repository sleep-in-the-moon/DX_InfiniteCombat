// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ICDataAsset.h"
#include "WeaponDataAsset.h"

#include "ICAssetManager.generated.h"


/**
 * 
 */
UCLASS(Config = Game)
class DX_INFINITECOMBAT_API UICAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UICAssetManager& Get();

	const UICDataAsset& GetICDataAsset();
	const UWeaponDataAsset& GetWeaponDataAsset();

	// 获取 SoftObjectPtr 所软引用的资源，不在内存中将触发同步加载，可选择加载后保持对其硬引用
	template<typename AssetType>
	static AssetType* GetAssetBySoftPtr(const TSoftObjectPtr<AssetType>& AssetSoftPtr, bool bKeepInMemory = true);
	// 获取 SoftClassPtr 所软引用的类，不在内存中将触发同步加载，可选择加载后保持对其硬引用
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclassBySoftPtr(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

protected:

	void AddLoadedAsset(const UObject* Asset);

	//根据 PrimaryAssetType 从 AssetManager 的 AssetTypeMap 中加载 UPrimaryDataAsset 并存放到 GameDataMap (遍历所有相同 PrimaryAssetType 的 FPrimaryAssetId ，从 FPrimaryAssetId 获取软引用 加载)
	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataPath, FPrimaryAssetType PrimaryAssetType);

	// 以同步的方式加载 FSoftObjectPath 所软引用的资源
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	//virtual void StartInitialLoading() override;
	
	// 将一个 UPrimaryDataAsset 子类的软引用加载/缓存 到对象实例中 
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const* pResult = GameDataMap.Find(GameDataClass::StaticClass()))  // 内存中有则直接取
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}

protected:
	UPROPERTY(Config)
	TSoftObjectPtr<UICDataAsset> ICGameDataPath;  // 主资产对象弱引用
	UPROPERTY(Config)
	TSoftObjectPtr<UICDataAsset> WeaponDataPath;

	// 临时加载存放到内存中的数据缓存
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

private:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection LoadedAssetsCritical;

};

template<typename AssetType>
inline AssetType* UICAssetManager::GetAssetBySoftPtr(const TSoftObjectPtr<AssetType>& AssetSoftPtr, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetSoftPtr.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetSoftPtr.Get(); // 先尝试在内存中 find
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));  // AssetSoftPtr.LoadSynchronous() ?
		}

		if (LoadedAsset && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
inline TSubclassOf<AssetType> UICAssetManager::GetSubclassBySoftPtr(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
