#pragma once
#include "Component.h"
#include "LevelParser.h"

struct HoleNode;
class Level;
struct Vector2;
class EntityManager;
class Texture2D;
class HoleManager final : public Component
{
public:
	HoleManager();
	~HoleManager();

	void Initialize() override;
	void Render() const override;
	void Update() override;

	const HoleNode& GetCurrentNode(float x, float y) const;

	void InitLevel(const LevelData& level);
	void SetLevel(Level* pLevel);
	void SetEntityManager(EntityManager* pEntityManager);

private:
	Texture2D* m_pHoleTexture, * m_pHoleCracksTexture, * m_pHoleOpeningTexture;
	Level* m_pLevel;
	EntityManager* m_pEntityManager;
	HoleNode* m_pGrid[LevelData::Columns][LevelData::Rows];

	void DrawHole(const HoleNode& node) const;
	void SetHole(float x, float y, const Vector2& dir);
};