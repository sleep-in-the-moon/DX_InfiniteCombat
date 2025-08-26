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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HP);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHP);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	FGameplayAttributeData ATK;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ATK)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Defense)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute")
	FGameplayAttributeData DamageReduce;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DamageReduce)

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
};
