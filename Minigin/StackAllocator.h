#pragma once
#include "MemoryAllocator.h"

class StackAllocator : public dae::MemoryAllocator
{
public:
	StackAllocator(const StackAllocator& other) = delete;
	StackAllocator(StackAllocator&& other) noexcept = delete;
	StackAllocator& operator=(const StackAllocator& other) = delete;
	StackAllocator& operator=(StackAllocator&& other) noexcept = delete;

	explicit StackAllocator();
	~StackAllocator();

	void Initialize(const size_t size);

	using Marker = char*;

	// Get a marker to the current top of the stack.
	Marker GetMarker() const;

	// Free the memory up to the given marker
	void FreeToMarker(Marker marker);

	// Acquire a pointer to a buffer 
	void* Acquire(size_t nbBytes) override;

	// Release a buffer
	void Release(void*) override;

	// Reset the complete stack
	void Reset();

	// Get the amount of free memory that is left on the stack.
	size_t GetFreeSpaceAmount() const;

private:
	Marker m_pPool;
	Marker m_pStackPointer;
	size_t m_TotalSize;
	size_t m_CurrentSize;
};
