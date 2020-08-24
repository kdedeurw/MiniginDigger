#pragma once
#include "MiniginGame.h"

class TextObject;
class GameObject;
struct Vector2;
struct Vector4;
class SpawnManager;
class Level;
class BubbleBobbleGame final : public dae::MiniginGame
{
public:
	BubbleBobbleGame(const char* pTitle = "BubbleBobble", int w = 640, int h = 480, int msPF = 16);
	~BubbleBobbleGame();

private:
	enum class States
	{
		Menu,
		Playing,
		Pause,
	};

	//base functions
	void LoadGame() override;
	void Update() override;
	void Render() override;

	//game functions
	void ParseLevelData();
	void ParseEnemyData();

	void LoadMenuScene();
	void LoadPauseScene();

	States m_State;
	SpawnManager& m_SpawnManager;

	Level* m_pLevel;

	//inactive test content
	void Archive();
	void InputTest(TextObject* pTo);
	void ScaleTest(Vector2& scale);
	GameObject* GetParentChildTest();
	void ParentChildTest(GameObject* pParent);
	void DestRectTest(Vector4& dest);
};