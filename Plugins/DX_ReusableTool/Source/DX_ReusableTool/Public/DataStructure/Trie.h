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
class DX_REUSABLETOOL_API Trie final
{
	//GENERATED_BODY()

	struct TrieNode 
	{
		TMap<T, TrieNode*> ChildNodesMap;
	};

private:
	TrieNode* Root;

public:
	Trie()
	{
		Root = new TrieNode();
	}

	~Trie()
	{
		if (Root)
		{
			TrieNode* node = Root;//it delete

			delete Root;
		}
	}

	void Insert(const TArray<T>& items)
	{
		TrieNode* node = Root;
		for (const T item : items)
		{
			if (!node->ChildNodesMap.Find(item))
			{
				node->ChildNodesMap.Add(item, new TrieNode());
			}

			node = node->ChildNodesMap[item];
		}

	}

	bool Search(const TArray<T>& prefix) const
	{
		return SearchPrefix(prefix) != nullptr;
	}

	void PrintTrie() const
	{
		TQueue<TrieNode*> NodeQueue;
		TrieNode* node = nullptr;
		uint16 QSize = 0;

		NodeQueue.Enqueue(Root);
		++QSize;

		while (!NodeQueue.IsEmpty())
		{
			FString PrintLayer = "";
			uint16 ItQSize = QSize;
			for (uint8 index = 0; index < ItQSize; ++index)
			{
				NodeQueue.Dequeue(node);
				--QSize;

				for (const auto& item : node->ChildNodesMap)
				{
					if constexpr (std::is_integral_v<T>)
					{
						PrintLayer += FString::FromInt(item.Key);
					}
					else if constexpr (std::is_same_v<T, float>)
					{
						PrintLayer += FString::SanitizeFloat(item.Key);
					}
					else
						PrintLayer += item.Key.GetString();

					NodeQueue.Enqueue(item.Value);
					++QSize;
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("%s-"), *(PrintLayer));
		}
	}


private:
	TrieNode* SearchPrefix(const TArray<T>& prefix) const
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
