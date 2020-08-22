#include "MiniginPCH.h"
#include "StackAllocator.h"
#include <exception>
#include <memory>

StackAllocator::StackAllocator()
	: m_pPool{}
	, m_pStackPointer{}
	, m_TotalSize{}
	, m_CurrentSize{}
{}

StackAllocator::~StackAllocator()
{
	delete[] m_pPool;
	m_pPool = nullptr;

	m_pStackPointer = nullptr;
}

void StackAllocator::Initialize(const size_t size)
{
	m_pPool = new char[size];
	//std::memset(m_pPool, 0, size); //unnecessary operation?
	m_pStackPointer = m_pPool;
	m_TotalSize = size;
}

StackAllocator::Marker StackAllocator::GetMarker() const
{
	return m_pStackPointer;
}

void StackAllocator::FreeToMarker(const Marker marker)
{
	m_CurrentSize -= m_pStackPointer - marker;
	m_pStackPointer = marker;
}

void* StackAllocator::Acquire(size_t nbBytes)
{
	if (m_TotalSize - m_CurrentSize >= nbBytes)
	{
		m_CurrentSize += nbBytes;
		m_pStackPointer += nbBytes;
		return m_pStackPointer - nbBytes;
	}
	else
		throw std::exception("out of memory");
}

void StackAllocator::Release(void* ptr)
{
	m_CurrentSize -= sizeof(ptr);
	m_pStackPointer = &m_pPool[m_CurrentSize];
}

void StackAllocator::Reset()
{
	m_pStackPointer = m_pPool;
	m_CurrentSize = 0;
}

size_t StackAllocator::GetFreeSpaceAmount() const
{
	return m_TotalSize - m_CurrentSize;
}
