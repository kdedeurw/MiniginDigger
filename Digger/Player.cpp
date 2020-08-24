#include "MiniginPCH.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "GlobalInput.h"
#include "GameObject.h"
#include "GameState.h"
#include "RigidBody.h"
#include "PhysicsComponent.h"
#include "MiniMath.h"
#include "Subject.h"
#include "GlobalMemoryPools.h"
#include "Events.h"
#include "EntityManager.h"

EntityManager* Player::m_pEntityManager{};

Player::Player()
	: m_pSprites{}
    , m_State{}
	, m_Velocity{ 1.f, 0.f }
	, m_Speed{ 100.f }
	, m_FormerAngle{}
	, m_FormerVel{}
	, m_pSubject{ GlobalMemoryPools::GetInstance().CreateSubject(2) }
	, m_ShootTimer{ 0.f }
{}

Player::~Player()
{
	m_pSprites = nullptr;
}

void Player::Initialize()
{
	m_FormerVel = m_Velocity;
	m_SpawnPos = m_pGameObject->GetTransform().GetPosition();
}

void Player::Render() const
{
}

void Player::Update()
{
	const KeyboardMouseListener& listener = GlobalInput::GetInstance().KeyboardMouseListener;

	Vector2& pos = m_pGameObject->GetTransform().GetPosition();

	GameState& gs = GameState::GetInstance();

	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;

	if (listener.IsPressed(Key::ArrowLeft))
		m_Velocity.x = -m_Speed;
	else if (listener.IsPressed(Key::ArrowRight))
		m_Velocity.x = m_Speed;

	if (m_Velocity.x == 0)
	{
		if (listener.IsPressed(Key::ArrowUp))
			m_Velocity.y = m_Speed;
		else if (listener.IsPressed(Key::ArrowDown))
			m_Velocity.y = -m_Speed;
	}

	if (m_Velocity.x != 0 || m_Velocity.y != 0)
	{
		const float angle = atan2f(m_Velocity.x, m_Velocity.y);
		m_FormerAngle = MiniMath::ToDegrees * angle - 90.f;
		m_pGameObject->GetTransform().SetRotation(m_FormerAngle);
		m_FormerVel = m_Velocity;
	}

	if (m_ShootTimer <= 0.f && listener.IsPressed(Key::Space))
	{
		m_pEntityManager->SpawnProjectile(pos.x, pos.y, m_FormerVel);
		m_ShootTimer = 10.f;
	}

	UpdateState();

	m_pSprites->SetStateMultiplier((int)m_State);

	pos += m_Velocity * gs.DeltaTime;

	if (m_ShootTimer >= 0.f)
		m_ShootTimer -= gs.DeltaTime;
}

void Player::SetSprites(SpriteComponent* pComp)
{
	m_pSprites = pComp;
}

void Player::SetEntityManager(EntityManager* pEntityManager)
{
	m_pEntityManager = pEntityManager;
}

Subject* Player::GetSubject() const
{
	return m_pSubject;
}

const Vector2& Player::GetDirection() const
{
	return m_FormerVel;
}

void Player::Reset()
{
	m_pGameObject->GetTransform().SetPosition(m_SpawnPos);
	m_pGameObject->GetTransform().SetRotation(0.f);
}

void Player::OnTriggerDeath()
{
	m_pSubject->Notify(m_pGameObject, (int)Event::event_player_die);
	//TODO: start coroutine to do respawning sequence
	Reset();
}

void Player::UpdateState()
{
	switch (m_State)
	{
	case Player::States::Normal:
		m_pSprites->SetMaxFrames(4);
		m_pSprites->SetSizes(Vector2{ 16.f, 18.f });
		m_pSprites->SetPlayOnce(false);
		break;
	case Player::States::Die:
		//TODO
		m_pSprites->Reset();
		m_pSprites->SetMaxFrames(1);
		m_pSprites->SetSizes(Vector2{ 16.f, 18.f });
		m_pSprites->SetPlayOnce(true);
		break;
	}
}