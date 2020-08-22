#pragma once
namespace dae
{
	class MemoryAllocator
	{
	public:
		virtual ~MemoryAllocator() = default;

		virtual void* Acquire(size_t = 0) = 0;
		virtual void Release(void*) = 0;

		// remove default copy & move constructors/assignment operators
		MemoryAllocator(const MemoryAllocator &) = delete;
		MemoryAllocator(MemoryAllocator &&) = delete;
		MemoryAllocator & operator= (const MemoryAllocator &) = delete;
		MemoryAllocator & operator= (const MemoryAllocator &&) = delete;

		struct Tag
		{
			MemoryAllocator* pool;
		};

	protected:
		MemoryAllocator() = default;
	};
}

