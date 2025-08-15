// Copyright DX_IC


#include "DX_StaticFunlib.h"
#include "CollisionQueryParams.h"

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
