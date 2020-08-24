#include "MiniginPCH.h"
#include "Explosion.h"
#include "GameState.h"

void Explosion::Initialize()
{
}

void Explosion::Render() const
{
}

void Explosion::Update()
{
	const float deltaTime = GameState::GetInstance().DeltaTime;

	if (m_LifeTime >= 0.f)
		m_LifeTime -= deltaTime;
	else
		m_pGameObject->SetActive(false);

	//slight upward movement, 'particle'
	m_pGameObject->GetTransform().GetPosition().y += 0.1f * deltaTime;
}

void Explosion::SetLifeTime(float lifeTime)
{
	m_LifeTime = lifeTime;
}