// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DX_StaticFunlib.generated.h"

/**
 * 
 */
UCLASS()
class DX_REUSABLETOOL_API UDX_StaticFunlib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*UFUNCTION(BlueprintCallable)
	static void PrintStr();*/

	UFUNCTION(BlueprintPure)
	static bool GetAllFilesInFolder(const FString& RootDir, TArray<FString>& OutSubDirs, TArray<FString>& OutSubFiles, const FString& ExtentName="");

	static void MakeCollisionParam(const AActor* Avatar, FCollisionQueryParams& CollisionQueryParams, FCollisionObjectQueryParams& ObjectQueryParams, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes, bool bTraceComplex=false, bool bIgnoreSelf=true);
	
	//UFUNCTION(BlueprintCallable)
	//static void LineTraceFromPreToCurTickByMeshSockets(const AActor* Owner, const UStaticMeshComponent* StaticMesh, FName SockePrex);

};
