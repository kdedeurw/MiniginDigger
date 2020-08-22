#include "MiniginPCH.h"
#include "GlobalMemoryPools.h"
#include "ResourceManager.h"
#include <string>

using namespace dae;
void* operator new (size_t nbBytes)
{
	if (nbBytes == 0) nbBytes = 1;
	MemoryAllocator::Tag* const tag = reinterpret_cast<MemoryAllocator::Tag*>
		(malloc(nbBytes + sizeof(MemoryAllocator::Tag)));
	tag->pool = nullptr;
	return tag + 1;
}
void* operator new (size_t nbBytes, MemoryAllocator& allocator)
{
	if (nbBytes == 0) nbBytes = 1;
	MemoryAllocator::Tag* const tag = reinterpret_cast<MemoryAllocator::Tag*>
		(allocator.Acquire(nbBytes + sizeof(MemoryAllocator::Tag)));
		//TODO: fix extra bytes acquired because of Tag->pool* (undefined behaviour found)
	tag->pool = &allocator;
	return tag + 1;
}

void operator delete (void* pointerToBuffer) noexcept
{
	if (pointerToBuffer != nullptr)
	{
		MemoryAllocator::Tag* const tag = reinterpret_cast
			<MemoryAllocator::Tag*>(pointerToBuffer) - 1;
		if (tag->pool != nullptr)
			tag->pool->Release(tag);
		else
			free(tag);
	}
}
void operator delete (void* pointerToBuffer, MemoryAllocator& allocator) noexcept
{
	if (pointerToBuffer != nullptr)
	{
		MemoryAllocator::Tag* const tag = reinterpret_cast
			<MemoryAllocator::Tag*>(pointerToBuffer) - 1;
		allocator.Release(tag);
	}
}

GlobalMemoryPools::GlobalMemoryPools()
	: m_ComponentAllocator{}
	, m_StackAllocator{}
	, m_SceneAllocator{}
	, m_GameObjectAllocator{}
	, m_TextObjectAllocator{}
	, m_TransformAllocator{}
	, m_TextureAllocator{}
	, m_FontAllocator{}
	, m_SubjectAllocator{}
{
	m_ComponentAllocator.Initialize(2048);
	m_StackAllocator.Initialize(1024);
	m_SceneAllocator.Initialize(sizeof(Scene) * 5);
	m_GameObjectAllocator.Initialize(sizeof(GameObject) * 10);
	m_TextObjectAllocator.Initialize(sizeof(TextObject) * 10);
	m_TransformAllocator.Initialize(sizeof(Transform) * 20);
	m_TextureAllocator.Initialize(sizeof(Texture2D) * 15);
	m_FontAllocator.Initialize(sizeof(Font) * 5);
	m_SubjectAllocator.Initialize(sizeof(Subject) * 10);
	//TODO: fix magic number initialization
}

GlobalMemoryPools::~GlobalMemoryPools()
{
	m_SceneAllocator.CleanUp();
	m_GameObjectAllocator.CleanUp();
	m_TextObjectAllocator.CleanUp();;
	m_TransformAllocator.CleanUp();
	m_TextureAllocator.CleanUp();
	m_FontAllocator.CleanUp();
	m_SubjectAllocator.CleanUp();
}

Scene* GlobalMemoryPools::CreateScene(std::string name)
{
	return new (m_SceneAllocator) Scene{ std::move(name) };
}

GameObject* GlobalMemoryPools::CreateGameObject()
{
	return new (m_GameObjectAllocator) GameObject{ *new (m_TransformAllocator) Transform{} };
}

TextObject* GlobalMemoryPools::CreateTextObject(const std::string& text, Font* pFont)
{
	return new (m_TextObjectAllocator) TextObject{ *new (m_TransformAllocator) Transform{}, text, pFont };
}

Texture2D* GlobalMemoryPools::CreateTexture2D(SDL_Texture* pTexture)
{
	return new (m_TextureAllocator) Texture2D{ pTexture };
}

Font* GlobalMemoryPools::CreateOwnFont(const std::string& file, unsigned int size)
{
	return new (m_FontAllocator) Font{ file, size };
}

Subject* GlobalMemoryPools::CreateSubject(unsigned int maxSize)
{
	return new (m_SubjectAllocator) Subject{ maxSize };
}

void GlobalMemoryPools::ResetStack()
{
	m_StackAllocator.Reset();
}