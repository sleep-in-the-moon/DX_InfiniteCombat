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

	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, HP);
	
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHP);
	
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, ATK)

	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Defense)

	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DamageReduce)

	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, IncomingDamage)

protected:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ATK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageReduce;

	//Meta Attribute, consumed in PostGameplayEffectExecute
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterAttribute", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData IncomingDamage;

};
