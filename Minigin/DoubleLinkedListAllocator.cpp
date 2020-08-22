#include "MiniginPCH.h"
#include "DoubleLinkedListAllocator.h"

DoubleLinkedListAllocator::DoubleLinkedListAllocator()
	: m_pHead{}
	, m_pSentinel{}
{}

DoubleLinkedListAllocator::~DoubleLinkedListAllocator()
{
	delete[] m_pHead;
	m_pHead = nullptr;

	m_pSentinel = nullptr;
}

void DoubleLinkedListAllocator::Initialize(const size_t size)
{
	const size_t nbBlocks{ (size + sizeof(Header) + sizeof(Block) - 1) / sizeof(Block) };
	m_pHead = new Block[nbBlocks + 1];
	std::memset(m_pHead, 0, (nbBlocks + 1) * sizeof(Block));

	m_pSentinel = &m_pHead[nbBlocks];
	m_pSentinel->status = Status::reserved;
	m_pSentinel->link.next = m_pSentinel;
	m_pSentinel->link.prev = m_pSentinel;

	m_pHead->status = Status::free;
	m_pHead->count = nbBlocks;
	InsertAfter(*m_pSentinel, *m_pHead);
}

void* DoubleLinkedListAllocator::Acquire(const size_t nbBytes)
{
	const size_t nbBlocks{ (nbBytes + sizeof(Header) + sizeof(Block) - 1) / sizeof(Block) };

	Block* pBlock{ m_pSentinel->link.next };
	while (pBlock != m_pSentinel)
	{
		while (true)
		{
			Block* pSuccessor{ pBlock + pBlock->count };
			if (pSuccessor->status == Status::reserved)
				break;
			Unlink(*pSuccessor);
			pBlock->count += pSuccessor->count;
		}
		if (pBlock->count >= nbBlocks)
			break;

		pBlock = pBlock->link.next;
	}
	if (pBlock == m_pSentinel)
		throw std::exception("out of memory");
	if (pBlock->count > nbBlocks)
	{
		Block* newBlock{ pBlock + nbBlocks };
		newBlock->status = Status::free;
		newBlock->count = pBlock->count - nbBlocks;
		pBlock->count = nbBlocks;
		InsertAfter(*m_pSentinel, *newBlock);
	}
	Unlink(*pBlock);
	pBlock->status = Status::reserved;
	return pBlock->data;
}

void DoubleLinkedListAllocator::Release(void* pointerToBuffer)
{
	Block* pBlock{ reinterpret_cast<Block*>(reinterpret_cast<Header*>(pointerToBuffer) - 1) };

	if (pBlock < m_pHead || pBlock >= m_pHead + m_pHead->count)
	{
		throw std::exception("invalid pointer");
		return;
	}

	pBlock->status = Status::free;
	InsertAfter(*m_pSentinel, *pBlock);
}

void DoubleLinkedListAllocator::InsertAfter(Block& firstBlock, Block& secondBlock)
{
	secondBlock.link.next = firstBlock.link.next;
	secondBlock.link.prev = &firstBlock;
	firstBlock.link.next = &secondBlock;
	secondBlock.link.next->link.prev = &secondBlock;
}

void DoubleLinkedListAllocator::Unlink(Block& block)
{
	block.link.next->link.prev = block.link.prev;
	block.link.prev->link.next = block.link.next;
}