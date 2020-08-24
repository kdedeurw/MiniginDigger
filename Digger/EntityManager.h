#pragma once
#include <Component.h>
#include <vector>
#include "Vectors.h"

class Player;
class NobbinAI;
class Gem;
class GoldBag;
class Level;
class DiggerGameObserver;
class Subject;
class EntityManager final : public Component
{
public:
	EntityManager();
	~EntityManager();

	void Initialize() override;
	void Render() const override;
	void Update() override;

	void SetLevel(Level* pLevel);
	void SetObserver(DiggerGameObserver* pDiggerGameObserver);

	void SpawnPlayer(float x, float y);
	void SpawnEnemy();
	void SpawnGem(float x, float y);
	void SpawnGoldBag(float x, float y);
	void SpawnGold(float x, float y);
	void SpawnProjectile(float x, float y, Vector2 dir);
	void SpawnExplosion(float x, float y, float lifeTime = 1.f);

	void InitLevel();

	const std::vector<Player*>& GetPlayers() const;
	Subject* GetSubject() const;
	void GetSpawnCoords(float& x, float& y) const;

private:
	unsigned int m_EnemiesInLevel = 0;
	float m_EnemySpawnTimer = 10.f;
	float m_SpawnX, m_SpawnY;
	Level* m_pLevel;
	DiggerGameObserver* m_pDiggerGameObserver;
	Subject* m_pSubject;
	std::vector<Player*> m_pPlayers;
	std::vector<NobbinAI*> m_pEnemies;
	std::vector<Gem*> m_pGems;
	std::vector<GoldBag*> m_pGoldBags;
};