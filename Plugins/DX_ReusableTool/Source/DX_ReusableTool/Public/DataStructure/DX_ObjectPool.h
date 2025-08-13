// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"

/**
 * 
 */
template<typename T>
class DX_ObjectPool final : public FGCObject
{

	using OriginType = std::remove_cv_t<std::remove_reference_t<T> >;
	static constexpr bool bIsUObject = TIsDerivedFrom<OriginType, UObject>::Value;
	static constexpr bool bIsWidget = TIsDerivedFrom<OriginType, UUserWidget>::Value;
	using ElementType = std::conditional_t<bIsUObject, TObjectPtr<OriginType>, TSharedPtr<OriginType> >;

public:
	DX_ObjectPool() = default;
	~DX_ObjectPool() override = default;

	// 禁用拷贝/移动（避免 FGCObject 注册问题）
	DX_ObjectPool(const DX_ObjectPool&) = delete;
	DX_ObjectPool& operator=(const DX_ObjectPool&) = delete;
	DX_ObjectPool(DX_ObjectPool&&) = delete;
	DX_ObjectPool& operator=(DX_ObjectPool&&) = delete;


	template <typename = std::enable_if_t<bIsUObject && !bIsWidget> >
	//typename std::enable_if<bIsUObject&&!bIsWidget, TObjectPtr<OriginType> >::type
	ElementType AccessObject(UObject* Outer, TSubclassOf<UObject> SubClass)
	{
		if (!UnusedObjectArray.IsEmpty())
			return UnusedObjectArray.Pop();

		return NewObject<OriginType>(Outer, SubClass.Get());
	}

	template <typename = std::enable_if_t<bIsWidget> >
	//typename std::enable_if<bIsWidget, TObjectPtr<OriginType> >::type
	ElementType AccessObject(UUserWidget* OuterWidget, TSubclassOf<UUserWidget> SubClass)
	{
		if (!UnusedObjectArray.IsEmpty())
			return UnusedObjectArray.Pop();

		return CreateWidget<UUserWidget>(OuterWidget, SubClass);
	}

	template <
		typename... Args,
		typename = std::enable_if_t<!bIsUObject>,
		typename = void
	>
	ElementType AccessObject(Args&&... InArgs)
	{
		if (!UnusedObjectArray.IsEmpty())
			return UnusedObjectArray.Pop();

		return MakeShared<OriginType>(Forward<Args>(InArgs)...);
	}

	inline void RecycleObject(ElementType UnusedObject);
	//void RecycleObjects(const TArray<ElementType>& UnusedObject);
	inline void ClearPool();

private:
	inline void AddToUnused(ElementType UnusedObject);
	inline void AppendToUnused(const TArray<ElementType>& UnusedObjects);
	/*void AddToActive(ElementType ActiveObject);
	void AppendToActive(const TArray<ElementType>& ActiveObjects);*/

private:
	TArray<ElementType> UnusedObjectArray;
	//TArray<ElementType> ActiveObjectArray;
	
	// FGCObject 接口：在 GC 时被调用，报告我们持有的 UObject*
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		if constexpr (bIsUObject)
		{
			for (ElementType Obj : UnusedObjectArray)
			{
				if (Obj)
				{
					Collector.AddReferencedObject(Obj);
				}
			}
		}
	}

	virtual FString GetReferencerName() const override
	{
		return TEXT("DX_ObjectPool");
	}

};

template<typename T>
inline void DX_ObjectPool<T>::RecycleObject(ElementType UnusedObject)
{
	//ActiveObjectArray.Remove(UnusedObject);
	AddToUnused(UnusedObject);
}

template<typename T>
inline void DX_ObjectPool<T>::ClearPool()
{
	//ActiveObjectArray.Empty();
	UnusedObjectArray.Empty();
}

template<typename T>
inline void DX_ObjectPool<T>::AddToUnused(ElementType UnusedObject)
{
	UnusedObjectArray.Add(UnusedObject);
}

template<typename T>
inline void DX_ObjectPool<T>::AppendToUnused(const TArray<ElementType>& UnusedObjects)
{
	UnusedObjectArray.Append(UnusedObjects);
}

//template<typename T>
//inline void DX_ObjectPool<T>::AddToActive(ElementType ActiveObject)
//{
//	ActiveObjectArray.Add(ActiveObject);
//}
//
//template<typename T>
//inline void DX_ObjectPool<T>::AppendToActive(const TArray<ElementType>& ActiveObjects)
//{
//	ActiveObjectArray.Append(ActiveObjects);
//}
