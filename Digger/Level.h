#pragma once
#include "Component.h"
#include "Subject.h"
#include "Vectors.h"
#include "Types.h"
#include "LevelParser.h"

class Texture2D;
class RigidBody;
class EntityManager;
class HoleManager;
class Level : public Component, public Subject
{
public:
	Level();
	~Level();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	void SetFile(const std::string& file);
	const LevelData& GetCurrentLevel() const;

	void SetEntityManager(EntityManager* pEntityManager);

	void Reset();

private:
	int m_LevelId;
	Texture2D* m_pLevelsTexture;
	EntityManager* m_pEntityManager;
	HoleManager* m_pHoleManager;

	LevelData m_CurrentLevel;

	LevelParser m_Parser;

	void NextLevel();
	void SetHole(const Vector2& pos);
};