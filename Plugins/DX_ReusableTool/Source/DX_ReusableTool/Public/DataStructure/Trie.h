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
template<typename T>
class Trie final
{

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
		Clear();

		if (Root)
		{
			delete Root;
			Root = nullptr;
		}
	}

	void Insert(const TArray<T>& items)
	{
		/*if(!Root)
			Root = new TrieNode();*/

		TrieNode* node = Root;
		for (const T item : items)
		{
			if (!node->ChildNodesMap.Find(item))
			{
				node->ChildNodesMap.Emplace(item, new TrieNode());
			}

			node = node->ChildNodesMap[item];
		}

	}

	bool Search(const TArray<T>& prefix) const
	{
		return SearchPrefix(prefix) != nullptr;
	}

	void LayerIt() const
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

	void Clear()
	{
		if (Root)
		{
			ClearRecursively(Root);
		}
	}

	void PrintTrie() const
	{
		PrintTrieRecursively(Root);
	}

	TArray<TArray<T> > GetNodeAllChildBranches(TrieNode* node, T nodeElemT) const
	{
		TArray<TArray<T> > Res;
		TArray<T> CurElem;

		if (node != Root)
			CurElem.Add(nodeElemT);

		GetChildBranchsRecursively(node, Res, CurElem);
		
		return Res;
	}

	void SplitEndNode(const TArray<T>& prefix)
	{
		if (prefix.Num()<2)
			return;

		TrieNode* node = Root;
		TrieNode* ParentNode = nullptr;
		for (uint16 i = 0; i < prefix.Num(); ++i)
		{
			if (!node->ChildNodesMap.Find(prefix[i]))
				return;
			node = node->ChildNodesMap[prefix[i]];

			if (i == prefix.Num() - 2)
			{
				ParentNode = node;
			}
			if (i == prefix.Num() - 1)
			{
				if(ParentNode)
					ParentNode->ChildNodesMap.Remove(prefix[i]);

				for(const TArray<T>& Elems : GetNodeAllChildBranches(node, prefix[i]))
					Insert(Elems);  // 将node剩下的全部重新插入到Root

				break;
			}

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

	void PrintTrieRecursively(TrieNode* node, const FString& prefix = "", const FString& ch = "*", bool isLast = true) const
	{
		if (ch != "*") // 根节点用 * 
		{	
			FString str = prefix + (isLast ? TEXT("└── ") : TEXT("├── ")) + ch + "\n";
			UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("(Root)\n"));
		}

		// 遍历子节点
		uint16 i = 0;
		for (const auto& pair : node->ChildNodesMap)
		{
			++i;
			bool lastChild = (i == node->ChildNodesMap.Num());

			FString Nextch="xx";
			if constexpr (std::is_same_v<T, FString>)
			{
				Nextch = pair.Key;
			}
			else if constexpr (std::is_integral_v<T>)
			{
				Nextch = FString::FromInt(pair.Key);
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				Nextch = FString::SanitizeFloat(pair.Key);
			}
			else if constexpr (std::is_enum_v<T>)
			{
				UEnum* TUEnum = StaticEnum<T>();
				FText EnumText;

				if (TUEnum->FindDisplayNameTextByValue(EnumText, static_cast<int64>(pair.Key)))
					Nextch = EnumText.ToString();
				else
					Nextch = TUEnum->GetNameStringByValue(static_cast<int64>(pair.Key));
			}
			else
				Nextch = pair.Key.GetString();
			
			// 进入下一层时，prefix 要追加合适的前缀
			PrintTrieRecursively(pair.Value,
				prefix + (ch == "*" ? TEXT("") : (isLast ? TEXT("    ") : TEXT("│   "))),
				Nextch,
				lastChild);
		}
	}

	void ClearRecursively(TrieNode* node)  // 后序遍历，递归delete非Root点
	{
		if (node == nullptr)
		{
			return;
		}

		for (auto& pair : node->ChildNodesMap)
		{
			ClearRecursively(pair.Value);
		}

		node->ChildNodesMap.Empty();
		if (node != Root)
		{
			delete node;
			node = nullptr;
		}
		
	}

	void GetChildBranchsRecursively(TrieNode* node, TArray<TArray<T> >& Res, TArray<T>& CurElem) const  // 回溯算法
	{
		if(node->ChildNodesMap.Num()==0)
			Res.Emplace(CurElem);

		for (auto& childPair : node->ChildNodesMap)
		{
			if (!childPair.Value)
				break;

			CurElem.Add(childPair.Key);
			GetChildBranchsRecursively(childPair.Value, Res, CurElem);
			CurElem.Pop();
		}
	}
};
