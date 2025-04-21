// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAtrribute")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HP);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAtrribute")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHP);

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
};
