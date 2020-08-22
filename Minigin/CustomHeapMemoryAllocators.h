#pragma once
#include "FixedSizeAllocator.h"

void* operator new (size_t size, dae::MemoryAllocator& allocator)
{
	return allocator.Acquire(size);
}

void operator delete (void* ptr, dae::MemoryAllocator& allocator)
{
	return allocator.Release(ptr);
}

namespace chma
{
//-----------------------------
//			Vector
//-----------------------------
	template <typename T>
	class Vector final
	{
	public:
		Vector(unsigned int cap)
			: m_Count{}
			, m_Capacity{ cap }
			, m_pArray{}
			, m_Allocator{}
		{
			m_Allocator.Initialize(sizeof(T) * cap);
			m_pArray = m_Allocator.m_pPool;
		}
		~Vector()
		{
			m_pArray = nullptr;
			m_Allocator.CleanUp();
		}
		Vector(const Vector& other) = delete;
		Vector(Vector&& other) = delete;
		Vector& operator=(const Vector& other) = delete;
		Vector& operator=(Vector&& other) = delete;

		void PushBack(T object);
		void PopBack();
		void Remove(const T& object);
		T& At(unsigned int id);
		unsigned int Size() const { return m_Count; };
		unsigned int Capacity() const { return m_Capacity; };

	private:
		unsigned int m_Count, m_Capacity;
		T* m_pArray;
		FixedSizeAllocator<T> m_Allocator;
	};

	template <typename T>
	void Vector<T>::PushBack(T object)
	{
		new (m_Allocator) T{ std::move(object) };
		++m_Count;
	}

	template <typename T>
	void Vector<T>::PopBack()
	{
		//delete m_pArray[m_Count];
		m_Allocator.Release(m_pArray[m_Count]);
		--m_Count;
	}

	template<typename T>
	void Vector<T>::Remove(const T& object)
	{
		int id{ -1 };
		for (unsigned int i{}; i < m_Count; ++i)
		{
			if (m_pArray[i] == &object)
			{
				id = (int)i; //if count > max size of int, crash
			}
		}

		//TODO: finish
	}

	template<typename T>
	inline T& Vector<T>::At(unsigned int id)
	{
		return *m_pArray[id];
	}
}