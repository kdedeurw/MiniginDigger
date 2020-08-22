#pragma once
#include "MemoryAllocator.h"
#include <memory>
#include <exception>

template <typename T>
class FixedSizeAllocator : public dae::MemoryAllocator
{
	enum class Status : size_t
	{
		free,
		reserved,
	};
	struct Header
	{
		//as long as there is an object of type size_t upfront in the header, the reinterpret_cast with the current math will work
		Status status;
	};
	struct Block : Header
	{
		//x64 => struct padding!
		const static size_t size = sizeof(T);
		union
		{
			Block* pNext;
			char data[size + 1];
		};
		//undefined behaviour found: new and delete overloaded operator returns tag + 1
		//which means everything is shifted, making the status undefined, adding 1 more byte to data will fix this (bc struct padding)
		//TODO: fix
	};
public:
	FixedSizeAllocator();
	~FixedSizeAllocator();

	void Initialize(const size_t nbBytes, const size_t size = 0, bool isBytes = true);
	//manual forced deletion of all adjacent blocks
	void CleanUp();

	virtual void* Acquire(size_t = 0) override;
	virtual void Release(void* ptr) override;

	//sets all blocks to status free, meaning they will be overwritten when allocating a new object T (note: previous objects will become invalid!)
	void Reset();
	//returns the amount of free spaces where an object T can be allocated
	const size_t GetFreeBlocks() const;

private:

	Block* m_pPool;
	Block* m_pLast;
	size_t m_Size;
	size_t m_FreeBlocks;
};

template <typename T>
FixedSizeAllocator<T>::FixedSizeAllocator()
	: m_pPool{}
	, m_pLast{}
	, m_Size{}
	, m_FreeBlocks{}
{}

template <typename T>
FixedSizeAllocator<T>::~FixedSizeAllocator()
{
	m_pPool = nullptr;
	m_pLast = nullptr;
}

template<typename T>
void FixedSizeAllocator<T>::CleanUp()
{
	delete[] m_pPool;
	m_pPool = nullptr;
	m_pLast = nullptr;
}

template<typename T>
void FixedSizeAllocator<T>::Initialize(const size_t nbBytes, const size_t size, bool isBytes)
{
	m_Size = isBytes ? nbBytes / sizeof(T) : size;

	//if (isBytes) defined in how many bytes, with a constant objectsize of 'size'
	//else if (!isbytes) defined in constant objectsize of 'nbBytes', with amount of blocks as 'size'

	m_FreeBlocks = m_Size;
	m_pPool = new Block[m_Size];
	std::memset(m_pPool, 0, sizeof(Block) * m_Size);

	m_pLast = m_pPool;

	for (size_t i{ 1 }; i < m_Size; ++i)
	{
		m_pPool[i - 1].pNext = &m_pPool[i];
	}
	m_pPool[m_Size - 1].pNext = m_pPool;
}

template <typename T>
void* FixedSizeAllocator<T>::Acquire(size_t)
{
	if (m_FreeBlocks == 0)
		throw std::exception("out of memory");
	if (m_pLast->status == Status::free)
	{
		Block* pFormerLast{ m_pLast };
		m_pLast->status = Status::reserved;
		m_pLast = m_pLast->pNext; //change last to next (supposed-to-be) free block
		--m_FreeBlocks;
		return pFormerLast->data; //return reserved block's data to be used
	}
	throw std::exception("out of memory");
}

template <typename T>
void FixedSizeAllocator<T>::Release(void* ptr)
{
	Block* pBlock{ reinterpret_cast<Block*>(reinterpret_cast<Header*>(ptr) - 1) };

	//if (pBlock < m_pPool || pBlock >= m_pPool + m_Size * sizeof(Block))
	//	throw std::exception("invalid pointer");
	//TODO: fix

	pBlock->status = Status::free;
	Block* pFormerLast{ m_pLast };
	m_pLast = pBlock;
	m_pLast->pNext = pFormerLast;
	++m_FreeBlocks;
}

template <typename T>
void FixedSizeAllocator<T>::Reset()
{
	for (int i{ 1 }; i < m_Size; ++i)
	{
		m_pPool[i - 1].status = Status::free;
		m_pPool[i - 1].pNext = &m_pPool[i];
	}
	m_pPool[m_Size - 1].status = Status::free;
	m_pPool[m_Size - 1].pNext = m_pPool;
}

template<typename T>
inline const size_t FixedSizeAllocator<T>::GetFreeBlocks() const
{
	return m_FreeBlocks;
}