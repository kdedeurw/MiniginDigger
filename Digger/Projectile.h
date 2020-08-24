#pragma once
#include <Component.h>
#include "Vectors.h"

class EntityManager;
class Projectile : public Component
{
public:
	Projectile();
	~Projectile();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	void SetDirection(Vector2 dir);
	void SetSpeed(float speed);

	void OnTriggerHit();

	static void SetEntityManager(EntityManager* pEntityManager);

private:
	float m_Speed;
	Vector2 m_Direction;

	static EntityManager* m_pEntityManager;
};