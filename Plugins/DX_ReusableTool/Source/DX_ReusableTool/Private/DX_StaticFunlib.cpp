// Copyright DX_IC


#include "DX_StaticFunlib.h"
#include "CollisionQueryParams.h"
#include "Polygon2.h"

bool UDX_StaticFunlib::GetAllFilesInFolder(const FString& RootDir, TArray<FString>& OutSubDirs, TArray<FString>& OutSubFiles, const FString& ExtentName)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	/*FString AbsolutePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	AbsolutePath = AbsolutePath.Append(RootDir);*/
	FString AbsolutePath = RootDir;

	// 检查目录是否存在
	if (!PlatformFile.DirectoryExists(*AbsolutePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Directory does not exist: %s"), *AbsolutePath);
		return false;
	}

	// 使用IterateDirectory进行遍历
	PlatformFile.IterateDirectory(*AbsolutePath, [&](const TCHAR* FilenameOrDirectory, bool bIsDirectory) -> bool
		{
			if (bIsDirectory)
			{
				// 过滤掉当前目录(.)和上级目录(..)
				FString DirName = FPaths::GetCleanFilename(FilenameOrDirectory);
				if (DirName != TEXT(".") && DirName != TEXT(".."))
				{
					OutSubDirs.Add(FString(FilenameOrDirectory));
				}
			}
			else
			{
				FString FilePath = FString(FilenameOrDirectory);

				if (!ExtentName.IsEmpty())
				{
					// 检查文件扩展名
					FString Extension = FPaths::GetExtension(FilePath);
					if (Extension.Equals(ExtentName, ESearchCase::IgnoreCase))
					{
						OutSubFiles.Add(FilePath);
					}
				}
				else
					OutSubFiles.Add(FilePath);
			}
			return true; // 返回true以继续遍历
		});

	return true;
}

void UDX_StaticFunlib::MakeCollisionParam(const AActor* Avatar, FCollisionQueryParams& CollisionQueryParams, FCollisionObjectQueryParams& ObjectQueryParams, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, bool bTraceComplex, bool bIgnoreSelf)
{
	static const FName LineTraceTag(TEXT("LineTraceMultiForObjects"));
	//ConfigureCollisionParams(LineTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, MeshComp->GetOwner());
	CollisionQueryParams = FCollisionQueryParams(LineTraceTag, bTraceComplex);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	if (bIgnoreSelf)
	{
		if (Avatar)
		{
			CollisionQueryParams.AddIgnoredActor(Avatar);
		}
		else
		{
			// find owner
			const UObject* CurrentObject = Avatar;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				Avatar = Cast<AActor>(CurrentObject);
				if (Avatar)
				{
					CollisionQueryParams.AddIgnoredActor(Avatar);
					break;
				}
			}
		}
	}

	ObjectQueryParams = FCollisionObjectQueryParams(TraceObjectTypes);
}

TArray<FVector2D> UDX_StaticFunlib::SortVec2DsToCounterClockWise(const TArray<FVector2D>& Vec2Ds)
{
	UE::Geometry::FPolygon2d OuterPoly(Vec2Ds);

	if (OuterPoly.IsClockwise())//顺时针
	{
		OuterPoly.Reverse();//反转
		return OuterPoly.GetVertices();
	}

	FVector2D Centroid(0, 0);
	for (auto& V : Vec2Ds) Centroid += V;
	Centroid /= Vec2Ds.Num();

	TArray<FVector2D> Res = Vec2Ds;
	Res.Sort([&](auto& A, auto& B) {//逆时针排序
		float a = FMath::Atan2(A.Y - Centroid.Y, A.X - Centroid.X);
		float b = FMath::Atan2(B.Y - Centroid.Y, B.X - Centroid.X);
		return a < b;
		});

	return Res;
}

FVector2D UDX_StaticFunlib::GetPolygonCentroid(const TArray<FVector2D>& Vec2Ds)
{
	int32 NumVertices = Vec2Ds.Num();
	if (NumVertices < 3)
	{
		return FVector2D::ZeroVector;
	}

	double Area = 0.0;
	double Cx = 0.0;
	double Cy = 0.0;

	for (int32 i = 0; i < NumVertices; ++i)
	{
		const FVector2D& Current = Vec2Ds[i];
		const FVector2D& Next = Vec2Ds[(i + 1) % NumVertices];

		double Cross = Current.X * Next.Y - Next.X * Current.Y;
		Area += Cross;
		Cx += (Current.X + Next.X) * Cross;
		Cy += (Current.Y + Next.Y) * Cross;
	}

	Area *= 0.5;
	if (FMath::IsNearlyZero(Area))
	{
		return FVector2D::ZeroVector;
	}

	Cx /= (6.0 * Area);
	Cy /= (6.0 * Area);

	return FVector2D(Cx, Cy);
}
