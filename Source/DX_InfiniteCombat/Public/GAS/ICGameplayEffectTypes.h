// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ICTypes.h"
#include "ICGameplayEffectTypes.generated.h"


USTRUCT()
struct DX_INFINITECOMBAT_API FICGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FICGameplayEffectContext() :FICGameplayEffectContext(FAttackInfo(1.0)) {}
	FICGameplayEffectContext(FAttackInfo AttackInfo) :AttackInfo(AttackInfo) {}

	FAttackInfo GetAttackInfo() const
	{
		return AttackInfo;
	}

	void SetAttackInfo(const FAttackInfo& NewAttackInfo)
	{
		AttackInfo = NewAttackInfo;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FICGameplayEffectContext::StaticStruct();
	}

	virtual FICGameplayEffectContext* Duplicate() const override
	{
		FICGameplayEffectContext* NewContext = new FICGameplayEffectContext();
	
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}

		NewContext->AttackInfo = GetAttackInfo();
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	FAttackInfo AttackInfo;
};

template<>
struct TStructOpsTypeTraits<FICGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FICGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
