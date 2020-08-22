#pragma once
// overload global new and delete.

namespace dae {
	class MemoryAllocator;
}

void* operator new (size_t nbBytes);

void * operator new[] (size_t nbBytes);

void * operator new (size_t nbBytes, dae::MemoryAllocator& storagePool);

void * operator new[] (size_t nbBytes, dae::MemoryAllocator& storagePool);

void operator delete (void* pointerToBuffer);

void operator delete[] (void* pointerToBuffer);

void operator delete (void* pointerToBuffer, dae::MemoryAllocator &storagePool);

