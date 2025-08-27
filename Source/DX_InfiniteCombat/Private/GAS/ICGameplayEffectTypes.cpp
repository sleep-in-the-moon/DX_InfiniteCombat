// Copyright DX_IC


#include "GAS/ICGameplayEffectTypes.h"

bool FICGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return Super::NetSerialize(Ar, Map, bOutSuccess) && AttackInfo.NetSerialize(Ar, Map, bOutSuccess);
}
