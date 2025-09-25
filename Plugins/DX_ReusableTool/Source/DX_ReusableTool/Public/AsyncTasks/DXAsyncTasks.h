// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "DXAsyncTasks.generated.h"

/**
 * 
 */
UCLASS()
class DX_REUSABLETOOL_API UDXAsyncTasks : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSuccessPin, UTexture2D*, Texture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFailPin);

UCLASS()
class DX_REUSABLETOOL_API ULoadImageAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSuccessPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FFailPin OnFail;

	// 蓝图调用的入口
	// 异步加载本地路径PNG图片为Texture
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Async")
	static ULoadImageAsync* LoadLocalImageAsync(const FString& LocalPath);

	// 异步逻辑入口
	virtual void Activate() override;

private:
	FString Path;
};
