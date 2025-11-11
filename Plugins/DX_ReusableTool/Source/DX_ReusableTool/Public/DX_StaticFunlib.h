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

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static bool GetAllFilesInFolder(const FString& RootDir, TArray<FString>& OutSubDirs, TArray<FString>& OutSubFiles, const FString& ExtentName="");

	static void MakeCollisionParam(const AActor* Avatar, FCollisionQueryParams& CollisionQueryParams, FCollisionObjectQueryParams& ObjectQueryParams, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery> > TraceObjectTypes, bool bTraceComplex=false, bool bIgnoreSelf=true);
	
	//UFUNCTION(BlueprintCallable)
	//static void LineTraceFromPreToCurTickByMeshSockets(const AActor* Owner, const UStaticMeshComponent* StaticMesh, FName SockePrex);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static TArray<FVector2D> SortVec2DsToCounterClockWise(const TArray<FVector2D>& Vec2Ds);
	//根据多边形顶点计算其质心
	UFUNCTION(BlueprintPure, BlueprintCallable)
	static FVector2D GetPolygonCentroid(const TArray<FVector2D>& Vec2Ds);

};
