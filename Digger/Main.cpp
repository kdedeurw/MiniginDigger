#include "MiniginPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "MiniginGame.h"
#include <SDL.h>
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GlobalMemoryPools.h"
#include "GlobalInput.h"
#include "GameState.h"
#include "GameObject.h"
#include "Components.h"
#include "Transform.h"
#include "WindowInfo.h"

//project includes
#include "Level.h"
#include "EntityManager.h"
#include "MiniPhysicsManager.h"
#include "DiggerGameObserver.h"
#include "HoleManager.h"
#include "NobbinAI.h"
#include "GameManager.h"

int main(int, char* [])
{
	WindowInfo wi{ "Digger" };
	dae::MiniginGame game{ wi.pTitle, wi.Width, wi.Height, wi.MsPerFrame };
	//game.AddFPSScene();
	game.DisableDemoScene();

	SceneManager& sm = SceneManager::GetInstance();
	//GameState& gs = GameState::GetInstance();
	GlobalMemoryPools& mp = GlobalMemoryPools::GetInstance();
	//ResourceManager& rm = ResourceManager::GetInstance();
	
	Scene& scene = sm.CreateScene("Digger");
	
	//level
	GameObject* pLevelObj = mp.CreateGameObject();
	Level* pLevelComp = mp.CreateComponent<Level>();
	pLevelObj->AddComponent(pLevelComp);
	pLevelComp->SetFile("Levels.json");
	pLevelObj->GetTransform().SetScale(2.f, 4.f);
	pLevelObj->GetTransform().SetPosition(25.f, 15.f, 0.75f);
	scene.AddObject(pLevelObj);

	//HoleManager
	GameObject* pHoleManager = mp.CreateGameObject();
	HoleManager* pHoleManagerComp = mp.CreateComponent<HoleManager>();
	pHoleManager->AddComponent(pHoleManagerComp);
	pHoleManager->GetTransform().SetPosition(0.f, 0.f, 1.f);
	//pHoleManager->GetTransform().SetScale(2.f, 2.f);
	scene.AddObject(pHoleManager);
	
	//DiggerGameObserver
	DiggerGameObserver* pDiggerGameObserver = mp.CreateOnStack<DiggerGameObserver>();
	
	//PhysicsManager
	GameObject* pPhysicsManager = mp.CreateGameObject();
	MiniPhysicsManager* pPhysicsComp = mp.CreateComponent<MiniPhysicsManager>();
	pPhysicsManager->AddComponent(pPhysicsComp);
	scene.AddObject(pPhysicsManager);

	//GameManager
	GameObject* pGameManagerObj = mp.CreateGameObject();
	GameManager* pGameManagerComp = mp.CreateComponent<GameManager>();
	pGameManagerObj->AddComponent(pGameManagerComp);
	pGameManagerComp->SetDiggerGameObserver(pDiggerGameObserver);
	scene.AddObject(pGameManagerObj);

	pGameManagerObj->GetTransform().SetPosition(20.f, 440.f, 1.f);
	pGameManagerObj->GetTransform().SetScale(2.f, 2.f);
	
	//EntityManager
	GameObject* pEntityManagerObj = mp.CreateGameObject();
	EntityManager* pEntityManagerComp = mp.CreateComponent<EntityManager>();
	pEntityManagerObj->AddComponent(pEntityManagerComp);
	scene.AddObject(pEntityManagerObj);
	
	pEntityManagerComp->SetLevel(pLevelComp);
	pEntityManagerComp->SetObserver(pDiggerGameObserver);
	
	pLevelComp->SetEntityManager(pEntityManagerComp);
	
	pHoleManagerComp->SetLevel(pLevelComp);
	pHoleManagerComp->SetEntityManager(pEntityManagerComp);

	NobbinAI::SetEntityManager(pEntityManagerComp);
	NobbinAI::SetLevel(pLevelComp);
	NobbinAI::SetHoleManager(pHoleManagerComp);

	game.Start();
	return 0;
}