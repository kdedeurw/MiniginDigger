#include "MiniginPCH.h"
#include "SingleLinkedListAllocator.h"
#include <iostream>

SingleLinkedListAllocator::SingleLinkedListAllocator()
	: m_pHead{}
{}

SingleLinkedListAllocator::~SingleLinkedListAllocator()
{
	delete[] m_pHead;
	m_pHead = nullptr;
}

void SingleLinkedListAllocator::Initialize(const size_t size)
{
	const size_t nbBlocks{ (size + sizeof(Header) + sizeof(Block) - 1) / sizeof(Block) };
	m_pHead = new Block[nbBlocks + 1]; // use 1 for the head
	std::memset(m_pHead, 0, (nbBlocks + 1) * sizeof(Block)); // default initialization

	m_pHead->count = nbBlocks; // total amount of blocks, not to be changed or used during run-time
	m_pHead->next = &m_pHead[1]; // head's next is the first free block
	m_pHead->next->count = nbBlocks; // assign full size to entire (free) chunk (of 16 byte blocks) - 1 for the head
}

void* SingleLinkedListAllocator::Acquire(const size_t nbBytes)
{
	const size_t nbBlocks{ (nbBytes + sizeof(Header) + sizeof(Block) - 1) / sizeof(Block) };

	Block* previousBlock{ m_pHead };
	Block* nextBlock{ m_pHead->next };
	while (nextBlock != nullptr && nextBlock->count < nbBlocks)
	{
		// go through single linked list
		previousBlock = nextBlock;
		nextBlock = nextBlock->next;
	}
	if (!nextBlock)
		throw std::exception("out of memory");
	if (nextBlock->count > nbBlocks)
	{
		// if enough memory found, allocate and return memory address, decrease size of remaining free memory
		Block* newBlock = nextBlock + nbBlocks; // block AFTER allocated block
		newBlock->count = nextBlock->count - nbBlocks; // set this block's size to remaining size of allocated memory

		newBlock->next = nextBlock->next; // assign next block of newly block AFTER allocated block to newly formed (smaller) chunk

		nextBlock->count = nbBlocks; // former allocated block now has the size of allocation
		nextBlock->next = newBlock; // this former block's next is the (block with) other remaining free memory

		previousBlock->next = nextBlock->next; //head->next is last block allocated

		return nextBlock->data; // return data (union struct) which is 12bytes for 32bit and 8bytes for 64bit I think
	}
	throw std::exception("out of memory");
}

void SingleLinkedListAllocator::Release(void* pointerToBuffer)
{
	Block* pBlock{ reinterpret_cast<Block*>(reinterpret_cast<Header*>(pointerToBuffer) - 1) };

	if (pBlock < m_pHead || pBlock >= m_pHead + m_pHead->count) //head.count is only used here!
	{
		throw std::exception("invalid pointer");
		return;
	}

	//Note: inserts the chunk into the list at its respective position
	//		the 'free' list is sorted at all times
	//		merging is done by looping through the list

	Block* pPrevious{ m_pHead };
	Block* pNext{ m_pHead->next }; //first (next) chunk of free memory
	if (pNext < pBlock) //if pBlock is on the right (in memory position) of any existing pNext
	{
		Block* pOldNext{ pNext->next }; //store the original next in line to continue list after insertion
		pNext->next = pBlock; //pBlock comes after this current next
		pBlock->next = pOldNext; //add original next after block
	}
	else //if (pNext > pBlock) //if pBlock is on the left of the existing pNext
	{
		Block* pLast{ pNext }; //save last next
		pPrevious->next = pBlock; // == pNext ref == pLast
		pBlock->next = pLast; //insert last next after pBlock
	}

	MergeReleasedBlocks();

	/*
	Block* pOldHeadNext{ m_pHead->next }; // store old head
	m_pHead->next = pBlock; // head's next becomes released block
	m_pHead->next->count = pBlock->count;

	// perform possible merge of 2 adjacent 'empty' blocks
	if (pOldHeadNext + pBlock->count == pBlock)
	{
		// if released block is on the right of old head, take over block's length as free memory space
		pOldHeadNext->count += pBlock->count;
		pBlock->count = 0;
	}
	else if (pOldHeadNext - pBlock->count == pBlock)
	{
		// if released block is on the left of old head, take over block's pointer position AND length
		pOldHeadNext = pBlock;
		pOldHeadNext->count += pBlock->count;
		pBlock->count = 0;
	}

	m_pHead->next->next = m_pHead->next; // link is closed again as head's next's next becomes original head's next
	m_pHead->next->next->count = m_pHead->count;

	//check if we already have released a block and it's not the same block
	if (m_pReleasedHead && m_pReleasedHead != pBlock)
	{
		// check if the pReleaseHead is closer to the actual pHead in memory
		if (m_pReleasedHead < pBlock)
		{
			//Block* pSmallerReleasedHead{ m_pReleasedHead };
			//while (pSmallerReleasedHead + pBlock->count == pBlock)
			//{
			//	pSmallerReleasedHead->next = pBlock; // link this block to released link
			//	pSmallerReleasedHead->next->count = pBlock->count; // store size of released block
			//	pSmallerReleasedHead = pSmallerReleasedHead->next;
			//}
			// if that block's position + the other block's count equals the position of the block to be released (== adjacent blocks)
			if (m_pReleasedHead + pBlock->count == pBlock)
			{
				// 'merge' blocks by increasing 'free' memory space on current adjacent block
				m_pReleasedHead->count += pBlock->count;
				// releasedhead stays in the same position
			}
		}
		// else the pReleaseHead is further away
		else if (m_pReleasedHead > pBlock)
		{
			// if that block's position + the other block's count equals the position of the block to be released (== adjacent blocks)
			if (m_pReleasedHead - pBlock->count == pBlock)
			{
				// 'merge' blocks by increasing 'free' memory space on current adjacent block
				m_pReleasedHead->count += pBlock->count;
				// move the releasehead up front
				m_pReleasedHead = pBlock;
			}
		}
	}
	else
		m_pReleasedHead = pBlock;

	*/

	//First of all we get back the information about the allocation from the header.
	//Then, we iterate the Linked List to intert the free block in the right position(because is sorted by address).
	//Once we've inserted it, we merge contiguous blocks. 
	//We can merge in _O(1) because our Linked List is sorted. 
	//We only need to look at the previous and next elements in the linked list to see if we can merge this contiguous blocks. 
	//This operation of merging contiguous memory blocks to create bigger blocks is called Coalescence 
	//If we used instead a Sorted Linked List of free and allocated blocks, 
	//the complexity would be O(1) but the allocation compleixity would be *O(N) where N is the number of free and allocated blocks and space complexity would be much higher. 
	//When we free a memory block we also look at the previous and next blocks to see if we can merge them into one bigger block.
}

void SingleLinkedListAllocator::MergeReleasedBlocks()
{
	Block* pPrevious{ m_pHead };
	Block* pNext{ m_pHead->next };
	while (pNext != nullptr)
	{
		bool isMerged{};

		//if previous + size == next block, then merge
		if (pPrevious + pPrevious->count == pNext)
		{
			//if released block is on the right of current block, take over block's length as free memory space
			pPrevious->count += pNext->count;

			if (pNext->next != nullptr)
			{
				pPrevious->next = pNext->next; //if there's more elements to the linked list
				isMerged = true; //we merged the block, so no need to move the previous block pointer
			}
			else
			{
				pPrevious->next = nullptr; //otherwise, end list
				//by overwriting original pNext with next block in linked list (forget about previous pNext->next pointer)
				break;
			}
		}

		pNext = pNext->next;
		if (!isMerged)
			pPrevious = pPrevious->next;
	}
}