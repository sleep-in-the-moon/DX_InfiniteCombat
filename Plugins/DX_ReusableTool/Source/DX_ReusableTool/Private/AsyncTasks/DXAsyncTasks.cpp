// Copyright DX_IC


#include "AsyncTasks/DXAsyncTasks.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2D.h"
#include "Async/Async.h"

ULoadImageAsync* ULoadImageAsync::LoadLocalImageAsync(const FString& LocalPath)
{
	ULoadImageAsync* Task = NewObject<ULoadImageAsync>();
	Task->Path = LocalPath;
	return Task;
}

void ULoadImageAsync::Activate()
{
	Async(EAsyncExecution::Thread, [this]()
		{
			// 1. 在后台线程：读取文件数据并解码
			TArray<uint8> CompressedData;
			if (!FFileHelper::LoadFileToArray(CompressedData, *Path))
			{
				// 错误处理：文件读取失败，回到主线程回调
				Async(EAsyncExecution::TaskGraphMainThread, [this]() { OnFail.Broadcast(); });
				return;
			}

			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG); // PNG；格式可扩展

			if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
			{
				// 错误处理：解码失败，回到主线程回调
				Async(EAsyncExecution::TaskGraphMainThread, [this]() { OnFail.Broadcast(); });
				return;
			}

			TArray<uint8> UncompressedData;
			if (!ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedData))
			{
				// 错误处理：获取原始数据失败
				Async(EAsyncExecution::TaskGraphMainThread, [this]() { OnFail.Broadcast(); });
				return;
			}

			// 2. 获取图片尺寸信息
			int32 Width = ImageWrapper->GetWidth();
			int32 Height = ImageWrapper->GetHeight();

			// 3. 回到游戏主线程创建纹理（因为纹理操作是线程不安全的）
			Async(EAsyncExecution::TaskGraphMainThread, [Width, Height, UncompressedData = MoveTemp(UncompressedData), this]()
				{
					// 创建临时纹理
					UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
					if (Texture)
					{
						// 锁定纹理数据并拷贝
						FByteBulkData& BulkData = Texture->GetPlatformData()->Mips[0].BulkData;
						void* TextureData = BulkData.Lock(LOCK_READ_WRITE);
						FMemory::Memcpy(TextureData, UncompressedData.GetData(), UncompressedData.Num());
						BulkData.Unlock();
						// 更新纹理资源
						Texture->UpdateResource();
					}
					// 回调传递创建好的纹理
					OnSuccess.Broadcast(Texture);
				});
		});
}
