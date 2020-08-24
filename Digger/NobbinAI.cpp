#include "MiniginPCH.h"
#include "NobbinAI.h"
#include "Level.h"
#include "EntityManager.h"
#include "HoleManager.h"
#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "HoleNode.h"
#include "Renderer.h"
#include "Subject.h"
#include "Events.h"
#include "GameState.h"
#include <vector>

Level* NobbinAI::m_pLevel{};
EntityManager* NobbinAI::m_pEntityManager{};
HoleManager* NobbinAI::m_pHoleManager{};

NobbinAI::NobbinAI()
	: m_pSubject{ GlobalMemoryPools::GetInstance().CreateSubject(2) }
	, m_Speed{ 100.f }
{
}

NobbinAI::~NobbinAI()
{
	m_pSubject = nullptr;
}

void NobbinAI::Initialize()
{
	m_Velocity.x = -1.f;
}

void NobbinAI::Render() const
{
	Renderer::GetInstance().DrawLine(m_pGameObject->GetTransform().GetPosition(), (Vector2)m_pGameObject->GetTransform().GetPosition() + m_Velocity, RGBAColour{0, 255, 0, 255});
}

void NobbinAI::Update()
{
	//const LevelData& level = m_pLevel->GetCurrentLevel();
	Vector2& pos = m_pGameObject->GetTransform().GetPosition();

	//find closest item or player
	Vector2 closestPlayer{};
	for (Player* pPlayer : m_pEntityManager->GetPlayers())
	{
		closestPlayer = pPlayer->GetGameObject()->GetTransform().GetPosition();
	}
	//vector magnitude
	float shortestLength = (pos - closestPlayer).Magnitude();

	//find current node
	const HoleNode& node = m_pHoleManager->GetCurrentNode(pos.x, pos.y);

	//MOVING LEFT
	if (m_Velocity.x < 0)
	{
		//CONTINUE
		if (node.IsLeftConnected)
		{
			const Vector2 leftNodePos{ (float)(node.x - node.Size), (float)node.y };
			float newLength = (closestPlayer - leftNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
				shortestLength = newLength;

			m_Velocity.x = -1.f;
			m_Velocity.y = 0.f;
		}

		//POSSIBLE INTERSECTIONS
		if (node.IsBottomConnected)
		{
			const Vector2 bottomNodePos{ (float)node.x, (float)(node.y - node.Size) };
			float newLength = (closestPlayer - bottomNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = 0.f;
				m_Velocity.y = -1.f;
				shortestLength = newLength;
			}
		}
		if (node.IsTopConnected)
		{
			const Vector2 topNodePos{ (float)node.x, (float)(node.y + node.Size) };
			float newLength = (closestPlayer - topNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = 0.f;
				m_Velocity.y = 1.f;
				shortestLength = newLength;
			}
		}

		//DEAD-END
		//if (!node.IsLeftConnected)
		//	m_Velocity.x = 1.f;
	}

	//MOVING RIGHT
	else if (m_Velocity.x > 0)
	{
		//CONTINUE
		if (node.IsRightConnected)
		{
			const Vector2 rightNodePos{ (float)(node.x + node.Size), (float)node.y };
			float newLength = (closestPlayer - rightNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
				shortestLength = newLength;

			m_Velocity.x = 1.f;
			m_Velocity.y = 0.f;
		}

		//POSSIBLE INTERSECTIONS
		if (node.IsBottomConnected)
		{
			const Vector2 bottomNodePos{ (float)node.x, (float)(node.y - node.Size) };
			float newLength = (closestPlayer - bottomNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = 0.f;
				m_Velocity.y = -1.f;
				shortestLength = newLength;
			}
		}
		if (node.IsTopConnected)
		{
			const Vector2 topNodePos{ (float)node.x, (float)(node.y + node.Size) };
			float newLength = (closestPlayer - topNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = 0.f;
				m_Velocity.y = 1.f;
				shortestLength = newLength;
			}
		}

		//DEAD-END
		//if (!node.IsRightConnected)
		//	m_Velocity.x = -1.f;
	}

	//MOVING DOWNWARDS
	if (m_Velocity.y < 0)
	{
		//CONTINUE
		if (node.IsBottomConnected)
		{
			const Vector2 bottomNodePos{ (float)node.x, (float)(node.y - node.Size) };
			float newLength = (closestPlayer - bottomNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
				shortestLength = newLength;

			//continue going downwards
			m_Velocity.x = 0.f;
			m_Velocity.y = -1.f;
		}

		//POSSIBLE INTERSECTIONS
		if (node.IsRightConnected)
		{
			const Vector2 rightNodePos{ (float)(node.x + node.Size), (float)node.y };
			float newLength = (closestPlayer - rightNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = 1.f;
				m_Velocity.y = 0.f;
				shortestLength = newLength;
			}
		}
		if (node.IsLeftConnected)
		{
			const Vector2 leftNodePos{ (float)(node.x - node.Size), (float)node.y };
			float newLength = (closestPlayer - leftNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = -1.f;
				m_Velocity.y = 0.f;
				shortestLength = newLength;
			}
		}

		//DEAD-END
		//if (!node.IsBottomConnected)
		//	m_Velocity.y = 1.f;
	}

	//MOVING UPWARDS
	else if (m_Velocity.y > 0)
	{
		//CONTINUE
		if (node.IsTopConnected)
		{
			const Vector2 topNodePos{ (float)node.x, (float)(node.y + node.Size) };
			float newLength = (closestPlayer - topNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
				shortestLength = newLength;

			m_Velocity.x = 0.f;
			m_Velocity.y = 1.f;
		}

		//POSSIBLE INTERSECTIONS
		if (node.IsRightConnected)
		{
			const Vector2 rightNodePos{ (float)(node.x + node.Size), (float)node.y };
			float newLength = (closestPlayer - rightNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = 1.f;
				m_Velocity.y = 0.f;
				shortestLength = newLength;
			}
		}
		if (node.IsLeftConnected)
		{
			const Vector2 leftNodePos{ (float)(node.x - node.Size), (float)node.y };
			float newLength = (closestPlayer - leftNodePos).Magnitude();

			//check if closer
			if (newLength <= shortestLength)
			{
				//don't swap velocity
				m_Velocity.x = -1.f;
				m_Velocity.y = 0.f;
				shortestLength = newLength;
			}
		}

		//DEAD-END
		//if (!node.IsTopConnected)
		//	m_Velocity.y = -1.f;
	}

	pos += m_Velocity * GameState::GetInstance().DeltaTime * m_Speed;

	//get current velocity, if x == 0 and y == 0, neglect next step
	//	continue current velocity in either x or y if it can't 'turn' aka change velocity in either x or y
	//	if it can turn, check if it can move further along same velocity (if it's closer to closest entity)
	//		update velocity and return
	//	if it can turn, and not move further, check where you can turn and pick the one that's closer
	//		Use estimated pos through math and take that vector magnitude to decide (for the remaining or 2 remaining turns)
	//update velocity

	//only flip current velocity if there is a dead-end (which should almost be impossible)
	//if path is longer than current, but velocity is flipped, continue path and don't flip velocity
}

void NobbinAI::SetLevel(Level* pLevel)
{
	m_pLevel = pLevel;
}

void NobbinAI::SetEntityManager(EntityManager* pEntityManager)
{
	m_pEntityManager = pEntityManager;
}

void NobbinAI::SetHoleManager(HoleManager* pHoleManager)
{
	m_pHoleManager = pHoleManager;
}

void NobbinAI::SetSpeed(float speed)
{
	m_Speed = speed;
}

void NobbinAI::OnDeathTrigger()
{
	std::cout << "\n!A nobbin has died!\n";

	m_pSubject->Notify(m_pGameObject, (int)Event::event_enemy_killed_Nobbin);

	m_pGameObject->SetActive(false);
}

Subject* NobbinAI::GetSubject() const
{
	return m_pSubject;
}