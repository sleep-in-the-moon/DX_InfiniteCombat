// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/**
 * 
 */
template<typename T>
class DX_REUSABLETOOL_API DX_ObjectPool final
{

	using OriginType = std::remove_cv_t<std::remove_reference_t<T>>;
	static constexpr bool bIsUObject = TIsDerivedFrom<OriginType, UObject>::Value;
	using ElementType = std::conditional_t<bIsUObject, OriginType*, TSharedPtr<OriginType>>;

public:
	DX_ObjectPool<T>();

	ElementType AccessObject() const 
	{
		if(!UnusedObjectArray.IsEmpty())
			return UnusedObjectArray.Pop();

		if (bIsUObject)
			return NewObject<T>();

		return MakeShared<T>();
	}
	/*TArray<ElementType> AccessObjects(uint16 Num) const 
	{
		return TArray<ElementType>();
	}*/

	void RecycleObject(ElementType UnusedObject);
	void RecycleObjects(const TArray<ElementType>& UnusedObject);
	void ClearPool();

private:
	void AddToUnused(ElementType UnusedObject);
	void AppendToUnused(const TArray<ElementType>& UnusedObjects);
	void AddToActive(ElementType ActiveObject);
	void AppendToActive(const TArray<ElementType>& ActiveObjects);

private:
	TArray<ElementType> UnusedObjectArray;
	TArray<ElementType> ActiveObjectArray;
	
};

template<typename T>
inline DX_ObjectPool<T>::DX_ObjectPool()
{
}

template<typename T>
inline void DX_ObjectPool<T>::RecycleObject(ElementType UnusedObject)
{
	AddToUnused(UnusedObject);
}

template<typename T>
inline void DX_ObjectPool<T>::RecycleObjects(const TArray<ElementType>& UnusedObject)
{
	AppendToUnused(UnusedObject);
}

template<typename T>
inline void DX_ObjectPool<T>::ClearPool()
{
	ActiveObjectArray.Empty();
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

template<typename T>
inline void DX_ObjectPool<T>::AddToActive(ElementType ActiveObject)
{
	ActiveObjectArray.Add(ActiveObject);
}

template<typename T>
inline void DX_ObjectPool<T>::AppendToActive(const TArray<ElementType>& ActiveObjects)
{
	ActiveObjectArray.Append(ActiveObjects);
}
