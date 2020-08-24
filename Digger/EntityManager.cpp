#include "MiniginPCH.h"
#include "EntityManager.h"
#include "GlobalMemoryPools.h"
#include "GameState.h"
#include "Level.h"
#include "Player.h"
#include "Gem.h"
#include "MiniPhysicsComponent.h"
#include "Components.h"
#include "DiggerGameObserver.h"
#include "ColliderMasks.h"
#include "NobbinAI.h"
#include "Projectile.h"
#include "Explosion.h"
#include "ResourceManager.h"
#include "Subject.h"
#include "Events.h"

EntityManager::EntityManager()
	: m_pSubject{ GlobalMemoryPools::GetInstance().CreateSubject(1) }
{}

EntityManager::~EntityManager()
{
	m_pLevel = nullptr;
	m_pDiggerGameObserver = nullptr;
	m_pSubject = nullptr;
}

void EntityManager::Initialize()
{
	Player::SetEntityManager(this);
	Projectile::SetEntityManager(this);
	InitLevel();

	ResourceManager& rm = ResourceManager::GetInstance();
	rm.LoadTexture("PlayerSprites.png");
	rm.LoadTexture("NobbinSprites.png");
	rm.LoadTexture("Gem.png");
	rm.LoadTexture("GoldBagSprites.png");
	rm.LoadTexture("Fireball.png");
	rm.LoadTexture("Explosion.png");
}

void EntityManager::Render() const
{
}

void EntityManager::Update()
{
	GameState& gs = GameState::GetInstance();

	if (m_EnemySpawnTimer >= 0.f)
		m_EnemySpawnTimer -= gs.DeltaTime;
	else
	{
		SpawnEnemy();
		m_EnemySpawnTimer = 5.f * m_EnemiesInLevel;
	}
}

void EntityManager::SetLevel(Level* pLevel)
{
	m_pLevel = pLevel;
}

void EntityManager::SetObserver(DiggerGameObserver* pDiggerGameObserver)
{
	m_pDiggerGameObserver = pDiggerGameObserver;
	m_pSubject->AddObserver(pDiggerGameObserver);
}

void EntityManager::SpawnPlayer(float x, float y)
{
	GameState& gs = GameState::GetInstance();
	GlobalMemoryPools& mp = GlobalMemoryPools::GetInstance();

	GameObject* pPlayerObj = mp.CreateGameObject();
	Player* pPlayerComp = mp.CreateComponent<Player>();
	pPlayerObj->AddComponent(pPlayerComp);
	gs.pGameScene->AddObject(pPlayerObj);
	pPlayerComp->GetSubject()->AddObserver(m_pDiggerGameObserver);

	SpriteComponent* pSprite = mp.CreateComponent<SpriteComponent>();
	pSprite->SetTexture("PlayerSprites.png");
	pSprite->SetTickRate(0.15f);
	pPlayerObj->AddComponent(pSprite);
	pPlayerObj->GetTransform().SetScale(2.f, 2.f);
	pPlayerObj->GetTransform().SetPosition(x, y, 1.f);
	pPlayerComp->SetSprites(pSprite); //player uses/modifies sprites, sprites is still a valid component of gameobject

	MiniPhysicsComponent* pPhysicsComp = mp.CreateComponent<MiniPhysicsComponent>();
	pPhysicsComp->SetCollisionChecks(true, true);
	pPhysicsComp->SetCollisionMasks(ColliderMask::PLAYER, ColliderMask::PLAYER | ColliderMask::GEM | ColliderMask::PROJECTILE);
	//both players and items cannot trigger played to die
	pPhysicsComp->SetDimensions({ 30.f, 30.f });
	pPhysicsComp->SetTriggerFunction(std::bind(&Player::OnTriggerDeath, pPlayerComp));
	pPlayerObj->AddComponent(pPhysicsComp);

	pPlayerObj->GetTransform().SetPosition(x, y);

	m_pPlayers.push_back(pPlayerComp);

	pPlayerObj->Initialize();
}

void EntityManager::SpawnEnemy()
{
	GlobalMemoryPools& mp = GlobalMemoryPools::GetInstance();

	GameObject* pEnemyObj = mp.CreateGameObject();
	NobbinAI* pNobbinComp = mp.CreateComponent<NobbinAI>();
	pEnemyObj->AddComponent(pNobbinComp);
	pNobbinComp->GetSubject()->AddObserver(m_pDiggerGameObserver);
	pNobbinComp->SetSpeed(105.f);

	SpriteComponent* pSpriteComp = mp.CreateComponent<SpriteComponent>();
	pEnemyObj->AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("NobbinSprites.png");
	pSpriteComp->SetTickRate(0.15f);
	pSpriteComp->SetMaxFrames(4);
	pSpriteComp->SetSizes({ 16.f, 18.f });

	MiniPhysicsComponent* pPhysicsComp = mp.CreateComponent<MiniPhysicsComponent>();
	pPhysicsComp->SetCollisionChecks(false, true);
	pPhysicsComp->SetCollisionMasks(ColliderMask::ENEMY, ColliderMask::ENEMY | ColliderMask::GEM | ColliderMask::PLAYER);
	pPhysicsComp->SetTriggerFunction(std::bind(&NobbinAI::OnDeathTrigger, pNobbinComp));
	pPhysicsComp->SetDimensions({ 30.f, 30.f });
	pEnemyObj->AddComponent(pPhysicsComp);

	pEnemyObj->GetTransform().SetScale(2.f, 2.f);
	pEnemyObj->GetTransform().SetPosition(m_SpawnX, m_SpawnY, 0.5f);
	GameState::GetInstance().pGameScene->AddObject(pEnemyObj);

	m_pEnemies.push_back(pNobbinComp);

	pEnemyObj->Initialize();

	++m_EnemiesInLevel;
	//TODO: notify
	m_pSubject->Notify(m_pGameObject, (int)Event::event_enemy_spawned_Nobbin);
}

void EntityManager::SpawnGem(float x, float y)
{
	GlobalMemoryPools& mp = GlobalMemoryPools::GetInstance();

	GameObject* pGemObj = mp.CreateGameObject();
	Gem* pGemComp = mp.CreateComponent<Gem>();
	pGemObj->AddComponent(pGemComp);
	pGemComp->GetSubject()->AddObserver(m_pDiggerGameObserver);

	Texture2DComponent* pTexComp = mp.CreateComponent<Texture2DComponent>();
	pGemObj->AddComponent(pTexComp);
	pTexComp->SetTexture("Gem.png");

	MiniPhysicsComponent* pPhysicsComp = mp.CreateComponent<MiniPhysicsComponent>();
	pPhysicsComp->SetCollisionChecks(true, true);
	pPhysicsComp->SetCollisionMasks(ColliderMask::GEM, ColliderMask::GEM | ColliderMask::PROJECTILE | ColliderMask::ENEMY);
	pPhysicsComp->SetTriggerFunction(std::bind(&Gem::OnPickedUp, pGemComp));
	pPhysicsComp->SetDimensions({20.f, 20.f});
	pGemObj->AddComponent(pPhysicsComp);

	pGemObj->GetTransform().SetScale(2.5f, 2.5f);
	pGemObj->GetTransform().SetPosition(x, y, 0.5f);
	GameState::GetInstance().pGameScene->AddObject(pGemObj);

	m_pGems.push_back(pGemComp);

	pGemObj->Initialize();
}

void EntityManager::SpawnGoldBag(float x, float y)
{
	GameObject* pGoldBagObj = GlobalMemoryPools::GetInstance().CreateGameObject();
	SpriteComponent* pSpriteComp = GlobalMemoryPools::GetInstance().CreateComponent<SpriteComponent>();
	pGoldBagObj->AddComponent(pSpriteComp);

	pSpriteComp->SetTexture("GoldBagSprites.png");
	pSpriteComp->SetMaxFrames(1);
	pSpriteComp->SetTickRate(0.15f);
	pSpriteComp->SetSizes(Vector2{15.f, 19.f});

	pGoldBagObj->GetTransform().SetScale(2.f, 2.f);
	pGoldBagObj->GetTransform().SetPosition(x, y, 0.75f);

	GameState::GetInstance().pGameScene->AddObject(pGoldBagObj);
	//m_pGoldBags.push_back();

	pGoldBagObj->Initialize();
}

void EntityManager::SpawnGold(float x, float y)
{
	x;
	y;
}

void EntityManager::SpawnProjectile(float x, float y, Vector2 dir)
{
	GameObject* pProjectileObj = GlobalMemoryPools::GetInstance().CreateGameObject();
	SpriteComponent* pSpriteComp = GlobalMemoryPools::GetInstance().CreateComponent<SpriteComponent>();
	pProjectileObj->AddComponent(pSpriteComp);

	Projectile* pProjectileComp = GlobalMemoryPools::GetInstance().CreateComponent<Projectile>();
	pProjectileComp->SetSpeed(75.f);
	pProjectileComp->SetDirection(std::move(dir));
	pProjectileObj->AddComponent(pProjectileComp);

	pSpriteComp->SetTexture("Fireball.png");
	pSpriteComp->SetMaxFrames(3);
	pSpriteComp->SetTickRate(0.15f);
	pSpriteComp->SetSizes(Vector2{ 8.f, 8.f });

	MiniPhysicsComponent* pPhysicsComp = GlobalMemoryPools::GetInstance().CreateComponent<MiniPhysicsComponent>();
	pPhysicsComp->SetCollisionChecks(true, true);
	pPhysicsComp->SetCollisionMasks(ColliderMask::PROJECTILE, ColliderMask::PROJECTILE | ColliderMask::GEM);
	//pPhysicsComp->SetTriggerFunction(std::bind(&Gem::OnPickedUp, pGemComp));
	pPhysicsComp->SetDimensions({ 10.f, 10.f });
	pProjectileObj->AddComponent(pPhysicsComp);

	pProjectileObj->GetTransform().SetScale(2.f, 2.f);
	pProjectileObj->GetTransform().SetPosition(x, y, 0.5f);

	GameState::GetInstance().pGameScene->AddObject(pProjectileObj);

	pProjectileObj->Initialize();
}

void EntityManager::SpawnExplosion(float x, float y, float lifeTime)
{
	GameObject* pProjectileObj = GlobalMemoryPools::GetInstance().CreateGameObject();
	SpriteComponent* pSpriteComp = GlobalMemoryPools::GetInstance().CreateComponent<SpriteComponent>();
	pProjectileObj->AddComponent(pSpriteComp);

	Explosion* pExplosionComp = GlobalMemoryPools::GetInstance().CreateComponent<Explosion>();
	pExplosionComp->SetLifeTime(lifeTime);
	pProjectileObj->AddComponent(pExplosionComp);

	pSpriteComp->SetTexture("Explosion.png");
	pSpriteComp->SetMaxFrames(3);
	pSpriteComp->SetTickRate(0.15f);
	pSpriteComp->SetPlayOnce(true);
	pSpriteComp->SetSizes(Vector2{ 8.f, 8.f });

	pProjectileObj->GetTransform().SetScale(2.f, 2.f);
	pProjectileObj->GetTransform().SetPosition(x, y, 0.5f);

	GameState::GetInstance().pGameScene->AddObject(pProjectileObj);

	pProjectileObj->Initialize();
}

void EntityManager::InitLevel()
{
	//Init level with only current LevelData as param?
	const LevelData& level = m_pLevel->GetCurrentLevel();

	for (int i{}; i < LevelData::Columns; ++i)
	{
		for (int j{}; j < LevelData::Rows; ++j)
		{
			const float x = i * 40.f + 25.f;
			const float y = j * 40.f + 25.f;
			switch (level.data[i][j])
			{
			case LevelTypes::tile:
				break;
			case LevelTypes::hole:
				//m_pLevel->SpawnHole(i, j);
				break;
			case LevelTypes::gem:
				SpawnGem(x, y);
				break;
			case LevelTypes::goldbag:
				SpawnGoldBag(x, y);
				break;
			case LevelTypes::spawner:
				m_SpawnX = x;
				m_SpawnY = y;
				break;
			case LevelTypes::player:
				SpawnPlayer(x, y);
				break;
			default:
				break;
			}
		}
	}
}

const std::vector<Player*>& EntityManager::GetPlayers() const
{
	return m_pPlayers;
}

Subject* EntityManager::GetSubject() const
{
	return m_pSubject;
}

void EntityManager::GetSpawnCoords(float& x, float& y) const
{
	x = m_SpawnX;
	y = m_SpawnY;
}