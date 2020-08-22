#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameState.h"
#include "MemoryAllocator.h"

SceneManager::SceneManager()
	: m_pAllScenes{}
	, m_pActiveScenes{}
{}

SceneManager::~SceneManager()
{
	for (Scene* pScene : m_pAllScenes)
	{
		delete pScene;
	}
	m_pAllScenes.clear();
	m_pActiveScenes.clear();
}

void SceneManager::Initialize()
{
	for (Scene* pScene : m_pAllScenes)
	{
		pScene->Initialize();
	}
}

void SceneManager::Update()
{
	for(Scene* pScene : m_pActiveScenes)
	{
		pScene->Update();
	}
}

void SceneManager::Render() const
{
	for (const Scene* pScene : m_pActiveScenes)
	{
		pScene->Render();
	}
}

void SceneManager::AddScene(Scene* pScene, bool isActive)
{
	for (Scene* pS : m_pAllScenes)
	{
		if (*pS == *pScene)
			return;
	}
	if (isActive)
	{
		m_pActiveScenes.push_back(pScene);
		GameState::GetInstance().pGameScene = pScene;
	}
	m_pAllScenes.push_back(pScene);
}

Scene& SceneManager::CreateScene(const std::string& name, bool isActive)
{
	Scene* pScene = GlobalMemoryPools::GetInstance().CreateScene(name.c_str());
	m_pAllScenes.push_back(pScene);
	if (isActive)
	{
		m_pActiveScenes.push_back(pScene);
		GameState::GetInstance().pGameScene = pScene;
	}
	return *pScene;
}

void SceneManager::ToggleScene(Scene& scene, bool isActive)
{
	for (Scene* pScene : m_pAllScenes)
	{
		if (*pScene == scene)
		{
			const auto it = std::find(m_pActiveScenes.begin(), m_pActiveScenes.end(), pScene);
			if (isActive)
			{
				if (it == m_pActiveScenes.end())
				{
					m_pActiveScenes.push_back(pScene);
					GameState::GetInstance().pGameScene = pScene;
				}
			}
			else
			{
				if (it != m_pActiveScenes.end())
				{
					m_pActiveScenes.erase(it);
					if (!m_pActiveScenes.empty())
						GameState::GetInstance().pGameScene = m_pActiveScenes.back();
					else
						GameState::GetInstance().pGameScene = nullptr;
				}
			}
			return;
		}
	}
}

void SceneManager::ToggleScene(const std::string& name, bool isActive)
{
	for (Scene* pScene : m_pAllScenes)
	{
		if (pScene->m_Name == name)
		{
			const auto it = std::find(m_pActiveScenes.begin(), m_pActiveScenes.end(), pScene);
			if (isActive)
			{
				if (it == m_pActiveScenes.end())
				{
					m_pActiveScenes.push_back(pScene);
					GameState::GetInstance().pGameScene = pScene;
				}
			}
			else
			{
				if (it != m_pActiveScenes.end())
				{
					m_pActiveScenes.erase(it);
					if (!m_pActiveScenes.empty())
						GameState::GetInstance().pGameScene = m_pActiveScenes.back();
					else
						GameState::GetInstance().pGameScene = nullptr;
				}
			}
			return;
		}
	}
}
