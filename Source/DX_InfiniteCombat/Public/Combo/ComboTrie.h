// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "Components/ActorComponent.h"
#include "DX_ReusableTool/Public/DataStructure/Trie.h"
#include "ICTypes.h"
#include "ComboTrie.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DX_INFINITECOMBAT_API UComboTrie : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool TryActiveCombo(EComboSource ComboSource);
	UFUNCTION(BlueprintCallable)
	void EndCombo();

	UFUNCTION(BlueprintCallable)
	TArray<FText> GetCurrentComboText() const;

	/*UFUNCTION(BlueprintCallable)
	void PrintCurrentCombo();
	UFUNCTION(BlueprintCallable)
	void PrintComboTrie();*/

private:
	//void ActiveComboReal();
	void InitialComboTrie(const TArray<TArray<EComboSource> >& TotalCombo);

public:

protected:
	UPROPERTY(EditDefaultsOnly)
	float ComboDuration = 0.7f;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ComboStateTag=FGameplayTag::RequestGameplayTag(TEXT("State.InCombo"), false);
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ComboDispatcherGATag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Combo.ComboDispatcher"), false);

private:
	Trie<EComboSource> ComboTrie;

	TArray<EComboSource> CurrentCombo;

	float CurrentDuration = 0.0f;
};
