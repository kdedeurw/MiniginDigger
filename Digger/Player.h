#pragma once
#include "Component.h"
#include "Vectors.h"

//Unity-style layout
//component is monobehaviour
//using composition

class SpriteComponent;
class Subject;
class EntityManager;
class Player : public Component
{
public:
	Player();
	~Player();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	void SetSprites(SpriteComponent* pComp);
	static void SetEntityManager(EntityManager* pEntityManager);
	Subject* GetSubject() const;
	const Vector2& GetDirection() const;

	enum class States
	{
		Normal,
		Die,
	};

	void Reset();

	void OnTriggerDeath();

protected:
	float m_ShootTimer;
	float m_Speed;
	float m_FormerAngle;
	SpriteComponent* m_pSprites;
	Subject* m_pSubject;
	States m_State;
	Vector2 m_Velocity;
	Vector2 m_FormerVel;
	Vector2 m_SpawnPos;

	static EntityManager* m_pEntityManager;

	void UpdateState();

};