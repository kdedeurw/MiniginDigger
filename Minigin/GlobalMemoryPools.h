#pragma once
#include "SingletonRef.h"
#include "MemoryOverrides.h"
#include "FixedSizeAllocator.h"
#include "StackAllocator.h"
#include "SingleLinkedListAllocator.h"

#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h"
#include "Texture2D.h"
#include "Font.h"
#include "Subject.h"

//Include for access to project's memory pools
//this memory is being cleaned up afterwards
//TODO: removing memory run-time

struct SDL_Texture;
class Scene;
class GlobalMemoryPools final : public SingletonRef<GlobalMemoryPools>
{
public:
	~GlobalMemoryPools();

	GameObject* CreateGameObject();
	TextObject* CreateTextObject(const std::string& text, Font* pFont);
	Texture2D* CreateTexture2D(SDL_Texture* pTexture);
	Font* CreateOwnFont(const std::string& file, unsigned int size);
	Subject* CreateSubject(unsigned int maxSize);

	//create a custom component on a single linked list allocator
	template <typename T>
	T* CreateComponent();
	//create an object on a shared stack
	template <typename T>
	T* CreateOnStack();
	//reset current data on stack
	void ResetStack();

private:
	friend GlobalMemoryPools& SingletonRef<GlobalMemoryPools>::GetInstance();
	GlobalMemoryPools();

	SingleLinkedListAllocator m_ComponentAllocator;
	StackAllocator m_StackAllocator;
	FixedSizeAllocator<Scene> m_SceneAllocator;
	FixedSizeAllocator<GameObject> m_GameObjectAllocator;
	FixedSizeAllocator<TextObject> m_TextObjectAllocator;
	FixedSizeAllocator<Transform> m_TransformAllocator;
	FixedSizeAllocator<Texture2D> m_TextureAllocator;
	FixedSizeAllocator<Font> m_FontAllocator;
	FixedSizeAllocator<Subject> m_SubjectAllocator;

	friend class SceneManager;
	Scene* CreateScene(std::string name);

};

template<typename T>
T* GlobalMemoryPools::CreateComponent()
{
	return new (m_ComponentAllocator) T;
}

template<typename T>
T* GlobalMemoryPools::CreateOnStack()
{
	return new (m_StackAllocator) T;
}