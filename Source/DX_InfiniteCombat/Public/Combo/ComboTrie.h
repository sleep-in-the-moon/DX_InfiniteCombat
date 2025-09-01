// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DX_ReusableTool/Public/DataStructure/Trie.h"
#include "ICTypes.h"
#include "ComboTrie.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UComboTrie : public UObject
{
	GENERATED_BODY()
	
public:
	TArray<EComboSource> CurrentCombo;
private:
	Trie<EComboSource> ComboTrie;
};
