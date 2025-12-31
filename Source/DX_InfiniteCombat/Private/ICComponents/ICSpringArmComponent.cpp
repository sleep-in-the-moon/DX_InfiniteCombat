// Copyright DX_IC


#include "ICComponents/ICSpringArmComponent.h"

FVector UICSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{
	return bHitSomething ? TraceHitLocation : DesiredArmLocation;
}
