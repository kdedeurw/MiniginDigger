#include "MiniginPCH.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameState.h"
#include "EntityManager.h"

EntityManager* Projectile::m_pEntityManager{};

Projectile::Projectile()
	: m_Speed{ 75.f }
	, m_Direction{}
{}

Projectile::~Projectile()
{
}

void Projectile::Initialize()
{
}

void Projectile::Render() const
{
}

void Projectile::Update()
{
	Vector2& pos = m_pGameObject->GetTransform().GetPosition();

	pos += m_Direction * GameState::GetInstance().DeltaTime * m_Speed;
}

void Projectile::SetDirection(Vector2 dir)
{
	m_Direction = std::move(dir);
}

void Projectile::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Projectile::OnTriggerHit()
{
	std::cout << "\n!Projectile hit!\n";

	const Vector2& pos = m_pGameObject->GetTransform().GetPosition();
	m_pEntityManager->SpawnExplosion(pos.x, pos.y, 3.f);

	m_pGameObject->SetActive(false);
}

void Projectile::SetEntityManager(EntityManager* pEntityManager)
{
	m_pEntityManager = pEntityManager;
}