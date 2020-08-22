#include "MiniginPCH.h"
#include "Scene.h"
#include "TextObject.h"
#include "GameObject.h"
#include "GlobalMemoryPools.h"
#include "MemoryAllocator.h"

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(std::string name)
	: m_Name{ std::move(name) }
	, m_pObjects{}
{}

Scene::~Scene()
{
	for (SceneObject* pObject : m_pObjects)
	{
		delete pObject;
	}
	m_pObjects.clear();
}

void Scene::AddObject(SceneObject* pObject)
{
	m_pObjects.push_back(pObject);
	//TODO: fix ID for child objects
	//TODO: reject child objects
}

void Scene::Initialize()
{
	for (SceneObject* pObject : m_pObjects)
	{
		pObject->Initialize();
	}
}

void Scene::Update()
{
	for(SceneObject* pObject : m_pObjects)
	{
		pObject->Update();
	}
}

void Scene::Render() const
{
	for (SceneObject* pObject : m_pObjects)
	{
		pObject->Render();
	}
}

bool Scene::operator==(const Scene& other) const
{
	return m_Name == other.m_Name;
}

bool Scene::operator==(Scene* pOther) const
{
	return this->m_Name == pOther->m_Name;
}