//Base includes
#include "MiniginPCH.h" //includes std::string etc.
#include "BubbleBobbleGame.h"
#include <chrono>
#include <thread>

//Project includes
#include "Timer.h"
#include "Components.h"
#include "Globals.h"
#include "General.h"
#include "Player.h"
#include "LevelParser.h"
#include "Level.h"
#include "SpawnManager.h"
#include "EnemyParser.h"

BubbleBobbleGame::BubbleBobbleGame(const char* pTitle, int w, int h, int msPF)
	: MiniginGame{ pTitle, w, h, msPF }
	, m_SpawnManager{ SpawnManager::GetInstance() }
	, m_pLevel{}
	, m_State{ States::Menu }
{}

BubbleBobbleGame::~BubbleBobbleGame()
{
	m_pLevel = nullptr;
}

void BubbleBobbleGame::LoadGame()
{
	GameState& gs = GameState::GetInstance();
	// tell the resource manager where he can find the game data
	m_ResourceManager.Init("../Data/");
	//MiniginGame::LoadGame();

	LoadMenuScene();
	LoadPauseScene();
	
	Scene& scene = m_SceneManager.CreateScene("BubbleBobbleScene", false);
	gs.pGameScene = &scene;

	GameObject* pGo = m_GlobalMemoryPools.CreateGameObject();
	pGo->GetTransform().SetPosition(m_WindowInfo.Width / 4.f, m_WindowInfo.Height / 2.f);

	SpriteComponent* pSprite = m_GlobalMemoryPools.CreateComponent<SpriteComponent>();
	pSprite->SetTexture("PlayerSprites.png");
	pSprite->SetTickRate(0.25f);
	pGo->AddComponent(pSprite);
	pGo->GetTransform().SetScale(Vector2{ 2.f, 2.f });

	Player* pPlayer = m_GlobalMemoryPools.CreateComponent<Player>();
	pPlayer->SetSprites(pSprite); //player uses/modifies sprites, sprites is still a valid component of gameobject
	pGo->AddComponent(pPlayer);
	pGo->GetTransform().SetPosition(Vector2{75.f, 60.f});

	scene.AddObject(pGo);

	LevelParser::GetInstance().Parse("SeperatedLevelData.dat");
	EnemyParser::GetInstance().Parse("SeperatedEnemyData.dat");

	pGo = m_GlobalMemoryPools.CreateGameObject();
	m_pLevel = m_GlobalMemoryPools.CreateComponent<Level>();
	pGo->AddComponent(m_pLevel);
	m_pLevel->SetLevels(&LevelParser::GetInstance().GetLevels());
	m_pLevel->SetEnemyData(&EnemyParser::GetInstance().GetData());
	pGo->GetTransform().SetScale(2.f, 2.f);
	pGo->GetTransform().SetPosition(16.f, 16.f);

	scene.AddObject(pGo);

	AddFPSScene();
}

void BubbleBobbleGame::Update()
{
	switch (m_State)
	{
	case BubbleBobbleGame::States::Menu:
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::Enter) || m_GlobalInput.ControllerListener.IsPressed(Button::Start))
		{
			m_State = States::Playing;
			SceneManager::GetInstance().ToggleScene("MenuScene", false);
			SceneManager::GetInstance().ToggleScene("BubbleBobbleScene", true);
		}
		break;
	case BubbleBobbleGame::States::Playing:
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::P) || m_GlobalInput.ControllerListener.IsPressed(Button::Select))
		{
			m_State = States::Pause;
			SceneManager::GetInstance().ToggleScene("PauseScene", true);
		}
		break;
	case BubbleBobbleGame::States::Pause:
		//TODO: fix time-skip by timer in main loop still counting
		while (true)
		{
			m_GlobalInput.KeyboardMouseListener.ProcessInput();
			m_GlobalInput.ControllerListener.ProcessInput();
			if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::Enter) || m_GlobalInput.ControllerListener.IsPressed(Button::Start))
			{
				m_State = States::Playing;
				SceneManager::GetInstance().ToggleScene("PauseScene", false);
				break;
			}
		}
		break;
	}
}

void BubbleBobbleGame::Render()
{
}

void BubbleBobbleGame::ParseLevelData()
{
}

void BubbleBobbleGame::ParseEnemyData()
{
}

void BubbleBobbleGame::LoadMenuScene()
{
	GameState& gs = GameState::GetInstance();

	Scene& scene = m_SceneManager.CreateScene("MenuScene", true);

	GameObject* pGo = m_GlobalMemoryPools.CreateGameObject();
	pGo->GetTransform().SetPosition(gs.WindowWidth / 2.f, gs.WindowHeight / 2.f);
	pGo->GetTransform().SetScale(2.f, 2.f);

	Texture2DComponent* pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("TitleMenu.png");

	pGo->AddComponent(pTex);

	scene.AddObject(pGo);

	TextObject* pTo = m_GlobalMemoryPools.CreateTextObject("Press Enter to play", ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	pTo->GetTransform().SetScale(2.f, 2.f);
	pTo->GetTransform().SetPosition(gs.WindowWidth / 4.f, gs.WindowHeight / 2.f + 25.f);
	pTo->SetColour(RGBAColour{ 255, 0, 255, 255 });

	scene.AddObject(pTo);

	pTo = m_GlobalMemoryPools.CreateTextObject("Press Start to play", ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	pTo->GetTransform().SetScale(2.f, 2.f);
	pTo->GetTransform().SetPosition(gs.WindowWidth / 4.f, gs.WindowHeight / 2.f - 25.f);
	pTo->SetColour(RGBAColour{ 255, 0, 255, 255 });

	scene.AddObject(pTo);
}

void BubbleBobbleGame::LoadPauseScene()
{
	GameState& gs = GameState::GetInstance();

	Scene& scene = m_SceneManager.CreateScene("PauseScene", false);

	TextObject* pTo = m_GlobalMemoryPools.CreateTextObject("Game Paused", ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	pTo->GetTransform().SetScale(2.f, 2.f);
	pTo->GetTransform().SetPosition(gs.WindowWidth / 4.f, gs.WindowHeight / 2.f + 25.f);
	pTo->SetColour(RGBAColour{ 255, 0, 255, 255 }); //pink

	scene.AddObject(pTo);

	pTo = m_GlobalMemoryPools.CreateTextObject("Press P/Select to resume", ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	pTo->GetTransform().SetScale(2.f, 2.f);
	pTo->GetTransform().SetPosition(gs.WindowWidth / 4.f, gs.WindowHeight / 2.f - 25.f);
	pTo->SetColour(RGBAColour{ 255, 0, 255, 255 }); //pink

	scene.AddObject(pTo);
}

void BubbleBobbleGame::Archive()
{
	GameObject* pParent = GetParentChildTest();

	Scene& scene = m_SceneManager.CreateScene("InputTestScene");
	TextObject* pInputTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pInputTo->GetTransform().SetPosition(320, 280);
	pInputTo->SetText("No input");
	scene.AddObject(pInputTo);
	TextObject* pTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pTo->SetText("MouseButtons to change height");
	pTo->GetTransform().SetPosition(0.f, 50.f);
	scene.AddObject(pTo);
	pTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pTo->SetText("Scrollwheel to change width");
	pTo->GetTransform().SetPosition(0.f, 100.f);
	scene.AddObject(pTo);
	pTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pTo->SetText("WASD to move");
	pTo->GetTransform().SetPosition(0.f, 150.f);
	scene.AddObject(pTo);
	GameObject* pScaleTest = m_GlobalMemoryPools.CreateGameObject();
	Texture2DComponent* pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pScaleTest->GetTransform().SetPosition(320, 200);
	pScaleTest->AddComponent(pTex);
	scene.AddObject(pScaleTest);

	Vector4& dest{ pTex->GetDestRect() };
	//Vector4& src{ pTex->GetSourceRect() };
	Vector2& scale{ pScaleTest->GetTransform().GetScale() };
	float& rot{ pScaleTest->GetTransform().GetRotation() };

	{
		float lag{};
		Timer<high_resolution_clock> timer{}; //start point automatically resets to Time::Now();
		while (!m_IsQuit)
		{
			//forced sleeptime
			//TODO: fix this goddamn time issue
			std::this_thread::sleep_for(16ms);
			//process all input
			m_GlobalInput.KeyboardMouseListener.ProcessInput();
			m_GlobalInput.ControllerListener.ProcessInput();
			m_IsQuit = m_GlobalInput.KeyboardMouseListener.IsQuit();
			//force quit
			m_IsQuit = m_GlobalInput.KeyboardMouseListener.IsPressed(Key::Escape);
			//get Time::Now()
			const auto currentTime = high_resolution_clock::now();
			//set global deltatime
			m_GameState.DeltaTime = timer.GetElapsedTime<float, milliseconds>();

			InputTest(pInputTo);

			pScaleTest->GetTransform().SetScale(scale);
			pScaleTest->GetTransform().SetRotation(rot);
			Texture2DComponent* pTest = pScaleTest->GetComponent<Texture2DComponent>();

			DestRectTest(dest);

			ScaleTest(scale);

			rot += 1.f;

			//pTest->SetSourceRect(src);
			pTest->SetDestRect(dest);

			ParentChildTest(pParent);

			lag += m_GameState.DeltaTime;
			while (lag >= m_WindowInfo.MsPerFrame)
			{
				m_GameState.DeltaTime = m_WindowInfo.MsPerFrame / 1000.f;
				m_SceneManager.Update();
				lag -= m_WindowInfo.MsPerFrame;
			}
			m_Renderer.Render();
			timer.ResetStartTime();

			const auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(m_WindowInfo.MsPerFrame) - high_resolution_clock::now());
			std::this_thread::sleep_for(sleepTime);
		}
	}
}

void BubbleBobbleGame::InputTest(TextObject* pTo)
{
	pTo->SetText("No input");
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
	{
		pTo->SetText("W");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
			pTo->SetText("W + A");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
			pTo->SetText("W + D");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() + Vector3{ 0.f, 1.f });
	}
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
	{
		pTo->SetText("S");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
			pTo->SetText("S + A");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
			pTo->SetText("S + D");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() - Vector3{ 0.f, 1.f });
	}
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
	{
		pTo->SetText("A");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
			pTo->SetText("W + A");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
			pTo->SetText("S + A");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() - Vector3{ 1.f, 0.f });
	}
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
	{
		pTo->SetText("D");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
			pTo->SetText("W + D");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
			pTo->SetText("S + D");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() + Vector3{ 1.f, 0.f });
	}
}

void BubbleBobbleGame::ScaleTest(Vector2& scale)
{
	if (m_GlobalInput.KeyboardMouseListener.IsScrollWheelMove(ScrollWheel::Up))
		scale.x += 0.1f;
	else if (m_GlobalInput.KeyboardMouseListener.IsScrollWheelMove(ScrollWheel::Down))
		scale.x -= 0.1f;

	if (m_GlobalInput.KeyboardMouseListener.IsMouseButtonPressed(Mouse::LMB))
		scale.y += 0.1f;
	else if (m_GlobalInput.KeyboardMouseListener.IsMouseButtonPressed(Mouse::RMB))
		scale.y -= 0.1f;
}

GameObject* BubbleBobbleGame::GetParentChildTest()
{
	Scene& scene = m_SceneManager.CreateScene("ParentChild");
	GameObject* pParent = m_GlobalMemoryPools.CreateGameObject();
	Texture2DComponent* pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pParent->AddComponent(pTex);
	pParent->GetTransform().SetPosition(100.f, 100.f);
	GameObject* pChild = m_GlobalMemoryPools.CreateGameObject();
	pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pChild->AddComponent(pTex);
	pChild->GetTransform().SetPosition(100.f, 100.f);
	GameObject* pPivot = m_GlobalMemoryPools.CreateGameObject();
	pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pPivot->AddComponent(pTex);
	pPivot->GetTransform().SetPosition(100.f, 100.f);
	pPivot->AddChildObject(pChild);
	pParent->AddChildObject(pPivot);
	//pParent->AddChildObject(pChild);
	pPivot->GetTransform().SetScale(0.1f, 0.1f);
	pParent->GetTransform().SetRotation(90.f);
	pParent->GetTransform().SetScale(1.5f, 0.5f);
	pChild->GetTransform().SetScale(0.5f, 1.5f);
	pChild->GetTransform().SetRotation(180.f);
	scene.AddObject(pParent);
	return pParent;
}

void BubbleBobbleGame::ParentChildTest(GameObject* pParent)
{
	pParent->GetTransform().SetPosition(m_GlobalInput.KeyboardMouseListener.GetMousePos() + Vector2{ 100.f, 0.f });
	pParent->GetTransform().Rotate(1.f); //rotate parent
	pParent->GetChildren().front()->GetTransform().Rotate(-2.f); //rotate pivot (child of parent)
	pParent->GetChildren().front()->GetChildren().front()->GetTransform().Rotate(3.f); //rotate child (child of child)
}

void BubbleBobbleGame::DestRectTest(Vector4& dest)
{
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
		dest.x -= 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
		dest.x += 1.f;
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
		dest.y += 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
		dest.y -= 1.f;

	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowLeft))
		dest.z -= 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowRight))
		dest.z += 1.f;
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowUp))
		dest.w += 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowDown))
		dest.w -= 1.f;
}