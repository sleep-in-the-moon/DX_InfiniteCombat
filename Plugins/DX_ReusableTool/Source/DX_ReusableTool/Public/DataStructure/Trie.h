// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "Trie.generated.h"

#if __cplusplus >= 201703L && __cplusplus < 202002L
#pragma message("Using C++17")
#elif __cplusplus >= 202002L
#pragma message("Using C++20")
#else
#pragma message("Using pre-C++17")
#endif

/**
 * 
 */
//UCLASS()
template<typename T>
class DX_REUSABLETOOL_API UTrie : public UObject
{
	//GENERATED_BODY()

	struct TrieNode 
	{
		TMap<T, TrieNode> ChildNodesMap;
	};

private:
	TrieNode* Root;

public:
	UTrie()
	{
		Root = new TrieNode();
	}

	~UTrie()
	{
		if (Root)
			delete Root;
	}

	void Insert(const TArray<T>& items)
	{
		TrieNode* node = Root;
		for (const T& item : items)
		{
			if (!node->ChildNodesMap.Find(item))
				node->ChildNodesMap.Add(item, TrieNode());
			node = node->ChildNodesMap[item];
		}

	}

	bool Search(const TArray<T>& prefix)
	{
		return SearchPrefix(prefix) != nullptr;
	}

private:
	TrieNode* SearchPrefix(const TArray<T>& prefix)
	{
		TrieNode* node = Root;
		for (const T& item : prefix)
		{
			if (!node->ChildNodesMap.Find(item))
				return nullptr;

			node = node->ChildNodesMap[item];
		}
		return node;
	}

};
