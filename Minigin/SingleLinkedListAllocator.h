#pragma once
#include "MemoryAllocator.h"

struct Header
{
	size_t count = 0;
};

struct Block : Header
{
	const static int size = 16;
	union
	{
		Block* next = nullptr;
		char data[size - sizeof(Header)];
	};
};

class SingleLinkedListAllocator final : public dae::MemoryAllocator
{
public:
	SingleLinkedListAllocator();
	~SingleLinkedListAllocator();

	void Initialize(const size_t size);

	void* Acquire(const size_t nbBytes = 0) override;
	void Release(void* pointerToBuffer) override;

private:
	Block* m_pHead;

	void MergeReleasedBlocks();
};