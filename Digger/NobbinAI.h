#pragma once
#include <Component.h>
#include "Vectors.h"

class Level;
class EntityManager;
class HoleManager;
class Subject;
struct HoleNode;
class NobbinAI : public Component
{
public:
	NobbinAI();
	virtual ~NobbinAI();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	//gather Curr LevelData from Level
	static void SetLevel(Level* pLevel);
	static void SetEntityManager(EntityManager* pEntityManager);
	static void SetHoleManager(HoleManager* pHoleManager);

	void SetSpeed(float speed);
	void OnDeathTrigger();

	Subject* GetSubject() const;

private:
	float m_Speed;
	Subject* m_pSubject;
	Vector2 m_Velocity;

	//static LevelData member
	static Level* m_pLevel;
	static EntityManager* m_pEntityManager;
	static HoleManager* m_pHoleManager;
};