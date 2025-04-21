// Copyright DX_IC


#include "DX_StaticFunlib.h"
#include "CollisionQueryParams.h"

void UDX_StaticFunlib::MakeCollisionParam(const AActor* Owner, FCollisionQueryParams& CollisionQueryParams, FCollisionObjectQueryParams& ObjectQueryParams, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes, bool bTraceComplex, bool bIgnoreSelf)
{
	static const FName LineTraceMultiName(TEXT("LineTraceMultiForObjects"));
	//ConfigureCollisionParams(LineTraceMultiName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, MeshComp->GetOwner());
	CollisionQueryParams = FCollisionQueryParams(LineTraceMultiName, bTraceComplex);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	if (bIgnoreSelf)
	{
		if (Owner)
		{
			CollisionQueryParams.AddIgnoredActor(Owner);
		}
		else
		{
			// find owner
			const UObject* CurrentObject = Owner;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				Owner = Cast<AActor>(CurrentObject);
				if (Owner)
				{
					CollisionQueryParams.AddIgnoredActor(Owner);
					break;
				}
			}
		}
	}

	ObjectQueryParams = FCollisionObjectQueryParams(TraceObjectTypes);
}
