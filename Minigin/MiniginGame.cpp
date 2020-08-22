//Base includes
#include "MiniginPCH.h"
#include "MiniginGame.h"
#include <chrono>
#include <thread>
#include "Transform.h"
#include "GlobalInput.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"

//Project includes
#include "Timer.h"
#include "Components.h"
#include "GameState.h"

using namespace std;
using namespace std::chrono;

dae::MiniginGame::MiniginGame(const char* pTitle, int w, int h, int msPF)
	: m_QuitKey{ Key::Escape }
	, m_WindowInfo{ pTitle, w, h, msPF }
{
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	GameState::GetInstance().pWindowInfo = &m_WindowInfo;
}

dae::MiniginGame::~MiniginGame()
{}

void dae::MiniginGame::InitializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		m_WindowInfo.pTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_WindowInfo.Width,
		m_WindowInfo.Height,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_pWindow);
}

void dae::MiniginGame::Initialize()
{
	SceneManager::GetInstance().Initialize();
}

/**
 * Code constructing the scene world starts here
 */
void dae::MiniginGame::LoadGame()
{
	SceneManager& sm = SceneManager::GetInstance();
	ResourceManager& rm = ResourceManager::GetInstance();
	GlobalMemoryPools& mp = GlobalMemoryPools::GetInstance();

	Scene& scene = sm.CreateScene("Demo");

	GameObject* pGo = mp.CreateGameObject();
	Texture2DComponent* pTex = mp.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("background.jpg");
	pGo->AddComponent(pTex);
	pGo->GetTransform().SetPosition(320, 240);
	scene.AddObject(pGo);

	pGo = mp.CreateGameObject();
	pTex = mp.CreateComponent<Texture2DComponent>();
	pGo->AddComponent(pTex);
	pTex->SetTexture("logo.png");
	pGo->GetTransform().SetPosition(320, 280);
	scene.AddObject(pGo);

	Font* pFont = rm.LoadFont("Lingua.otf", 36);
	TextObject* pTo = mp.CreateTextObject("Programming 4 Assignment", pFont);
	pTo->GetTransform().SetPosition(80, 440);
	scene.AddObject(pTo);

	AddFPSScene();
}

void dae::MiniginGame::Cleanup()
{
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void dae::MiniginGame::Start()
{
	InitializeSDL();
	//LoadGame();
	Initialize();

	SceneManager& sm = SceneManager::GetInstance();
	Renderer& renderer = Renderer::GetInstance();
	GlobalInput& im = GlobalInput::GetInstance();
	GameState& gs = GameState::GetInstance();

	Timer<high_resolution_clock> timer{}; //start point automatically resets to Time::Now();
	while (!m_IsQuit)
	{
		//set global deltatime
		gs.DeltaTime = timer.GetElapsedTime<float, milliseconds>()/* / 1000.f*/;
		timer.ResetStartTime();
		//process all input
		im.KeyboardMouseListener.ProcessInput();
		im.ControllerListener.ProcessInput();
		m_IsQuit = im.KeyboardMouseListener.IsQuit();
		//force quit
		m_IsQuit = im.KeyboardMouseListener.IsPressed(m_QuitKey);
		//get Time::Now()
		const auto currentTime = high_resolution_clock::now();

		gs.DeltaTime /= 1000.f;
		sm.Update();
		Update();

		PhysicsComponent::UpdatePhysics();

		renderer.Render();
		Render();
	}

	Cleanup();
}

void dae::MiniginGame::ForceQuit()
{
	m_IsQuit = true;
}

void dae::MiniginGame::AddFPSScene(float x, float y) const
{
	SceneManager& sm = SceneManager::GetInstance();
	GlobalMemoryPools& mp = GlobalMemoryPools::GetInstance();

	Scene& scene = sm.CreateScene("FPS", true);
	GameObject* pGo = mp.CreateGameObject();
	FPSComponent* pFPS = mp.CreateComponent<FPSComponent>();
	pGo->AddComponent(pFPS);
	pGo->GetTransform().SetPosition(x, y);
	scene.AddObject(pGo);
}

void dae::MiniginGame::DisableDemoScene()
{
	SceneManager::GetInstance().ToggleScene("Demo", false);
}

void dae::MiniginGame::SetForceQuitKey(Key key)
{
	m_QuitKey = key;
}