// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ICSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
protected:
	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime) override;

};
