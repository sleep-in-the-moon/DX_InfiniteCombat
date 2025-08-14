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
		ElementType NewObjPtr;
		if (!UnusedObjectArray.IsEmpty())
		{
			FScopeLock Lock(&PoolMutex);
			NewObjPtr = UnusedObjectArray.Pop();
		}
		else
		{
			NewObjPtr = NewObject<OriginType>(Outer, SubClass.Get());
		}
		AddToActive(NewObjPtr);

		return NewObjPtr;
	}

	template <typename = std::enable_if_t<bIsWidget> >
	//typename std::enable_if<bIsWidget, TObjectPtr<OriginType> >::type
	ElementType AccessObject(UUserWidget* OuterWidget, TSubclassOf<UUserWidget> SubClass)
	{
		ElementType NewObjPtr;
		if (!UnusedObjectArray.IsEmpty())
		{
			FScopeLock Lock(&PoolMutex);
			NewObjPtr = UnusedObjectArray.Pop();
		}
		else
		{
			NewObjPtr = CreateWidget<UUserWidget>(OuterWidget, SubClass);
		}
		AddToActive(NewObjPtr);

		return NewObjPtr;
	}

	template <
		typename... Args,
		typename = std::enable_if_t<!bIsUObject>,
		typename = void
	>
	ElementType AccessObject(Args&&... InArgs)
	{
		ElementType NewObjPtr;
		if (!UnusedObjectArray.IsEmpty())
		{
			FScopeLock Lock(&PoolMutex);
			NewObjPtr = UnusedObjectArray.Pop();
		}
		else
		{
			NewObjPtr = MakeShared<OriginType>(Forward<Args>(InArgs)...);
		}
		AddToActive(NewObjPtr);

		return NewObjPtr;
	}

	TArray<ElementType> GetAllActives() const
	{
		return ActiveObjectArray;
	}

	inline void RecycleObject(ElementType UnusedObject);
	inline void RecycleObjects(const TArray<ElementType>& UnusedObjects);
	inline void ClearPool();

private:
	inline void AddToUnused(ElementType UnusedObject);
	inline void AppendToUnused(const TArray<ElementType>& UnusedObjects);
	inline void AddToActive(ElementType ActiveObject);
	inline void AppendToActive(const TArray<ElementType>& ActiveObjects);
	
	// FGCObject 接口：在 GC 时被调用，报告我们持有的 UObject*
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		FScopeLock Lock(&PoolMutex);
		if constexpr (bIsUObject)
		{
			for (ElementType Obj : UnusedObjectArray)
			{
				if (Obj)
				{
					Collector.AddReferencedObject(Obj);
				}
			}
			for (ElementType Obj : ActiveObjectArray)
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

private:
	TArray<ElementType> UnusedObjectArray;
	TArray<ElementType> ActiveObjectArray;
	FCriticalSection PoolMutex;
};

template<typename T>
inline void DX_ObjectPool<T>::RecycleObject(ElementType UnusedObject)
{
	FScopeLock Lock(&PoolMutex);
	if(ActiveObjectArray.Find(UnusedObject))
		ActiveObjectArray.Remove(UnusedObject);

	AddToUnused(UnusedObject);
}

template<typename T>
inline void DX_ObjectPool<T>::RecycleObjects(const TArray<ElementType>& UnusedObjects)
{
	for (ElementType UnusedObject : UnusedObjects)
	{
		RecycleObject(UnusedObject);
	}
}

template<typename T>
inline void DX_ObjectPool<T>::ClearPool()
{
	FScopeLock Lock(&PoolMutex);
	ActiveObjectArray.Empty();
	UnusedObjectArray.Empty();
}

template<typename T>
inline void DX_ObjectPool<T>::AddToUnused(ElementType UnusedObject)
{
	FScopeLock Lock(&PoolMutex);
	UnusedObjectArray.Add(UnusedObject);
}

template<typename T>
inline void DX_ObjectPool<T>::AppendToUnused(const TArray<ElementType>& UnusedObjects)
{
	FScopeLock Lock(&PoolMutex);
	UnusedObjectArray.Append(UnusedObjects);
}

template<typename T>
inline void DX_ObjectPool<T>::AddToActive(ElementType ActiveObject)
{
	FScopeLock Lock(&PoolMutex);
	ActiveObjectArray.Add(ActiveObject);
}

template<typename T>
inline void DX_ObjectPool<T>::AppendToActive(const TArray<ElementType>& ActiveObjects)
{
	FScopeLock Lock(&PoolMutex);
	ActiveObjectArray.Append(ActiveObjects);
}
