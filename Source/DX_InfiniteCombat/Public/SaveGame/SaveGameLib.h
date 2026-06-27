// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SaveGameLib.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API USaveGameLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable)
	//Object override Serialize(Ar) to save the specified field
	static TArray<uint8> SaveObject(UObject* Object, const FString& Src=FPaths::ProjectSavedDir()+TEXT("SaveGames/"));
	UFUNCTION(BlueprintCallable)
	static void LoadObject(UObject* Object, const FString& Src);
};

inline TArray<uint8> USaveGameLib::SaveObject(UObject* Object, const FString& Src)
{
	TArray<uint8> Data;
	FMemoryWriter Writer(Data, true);
	FObjectAndNameAsStringProxyArchive Ar(Writer, true);

	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;	//非增量更新
	Object->Serialize(Ar);

	if (FFileHelper::SaveArrayToFile(Data, *Src))
	{
		UE_LOG(LogTemp, Warning, TEXT("Object::%s::SaveSuccess::Path::%s"), *Object->GetFName().ToString(), *Src);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object::%s::SaveFailed::Path::%s"), *Object->GetFName().ToString(), *Src);
	}

	return Data;
}

inline void USaveGameLib::LoadObject(UObject* Object, const FString& Src)
{
	TArray<uint8> Data;
	if (FFileHelper::LoadFileToArray(Data, *Src))
	{
		FMemoryReader Reader(Data, true);
		FObjectAndNameAsStringProxyArchive Ar(Reader, true);
		Ar.ArIsSaveGame = true;
		Ar.ArNoDelta = true;
		Object->Serialize(Ar);
	}
}
