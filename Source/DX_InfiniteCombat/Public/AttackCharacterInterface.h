// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICStructs.h"
#include "AttackCharacterInterface.generated.h"

class UAbilitySystemComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UAttackCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DX_INFINITECOMBAT_API IAttackCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*UFUNCTION(BlueprintCallable)
	virtual FWeaponStruct GetCurWeapon() const = 0;*/
	/*virtual UAbilitySystemComponent* GetICAbilitySystemComponent() const = 0;
	UFUNCTION(BlueprintCallable)
	virtual bool SwitchSecondWeaponToCur() = 0;*/
};
