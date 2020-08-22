#pragma once
#include "MemoryAllocator.h"
#include <limits>

enum class Status : unsigned
{
	free,
	reserved,
};
struct Header
{
	Status status : 1;
	unsigned int count : std::numeric_limits<unsigned int>::digits - 1;
};
struct Block : Header
{
	enum { size = 16 };
	struct Links
	{
		Block* next;
		Block* prev;
	};
	union
	{
		Links link;
		char data[size - sizeof(Header)];
	};
};

class DoubleLinkedListAllocator final : public dae::MemoryAllocator
{
public:
	DoubleLinkedListAllocator();
	~DoubleLinkedListAllocator();

	void Initialize(const size_t size);

	virtual void* Acquire(const size_t nbBytes = 0) override;
	virtual void Release(void* pointerToBuffer) override;

private:
	Block* m_pHead;
	Block* m_pSentinel;

	void InsertAfter(Block& firstBlock, Block& secondBlock);
	void Unlink(Block& block);
};