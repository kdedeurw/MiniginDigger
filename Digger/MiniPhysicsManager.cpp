#include "MiniginPCH.h"
#include "MiniPhysicsManager.h"
#include "MiniPhysicsComponent.h"
#include "CollisionFunctionality.h"
#include "GameObject.h"
#include "Transform.h"

MiniPhysicsManager::MiniPhysicsManager()
	: m_pPhysicsComponents{}
{
}

MiniPhysicsManager::~MiniPhysicsManager()
{
	m_pPhysicsComponents.clear();
}

void MiniPhysicsManager::Initialize()
{
	MiniPhysicsComponent::m_pManager = this;
}

void MiniPhysicsManager::Update()
{
	const int size = (int)m_pPhysicsComponents.size();
	//TODO: update collisioncomponents
	//for (MiniPhysicsComponent* pCm : m_pPhysicsComponents)
	//{
	//	for (MiniPhysicsComponent* pCm2 : m_pPhysicsComponents)
	//	{
	//		if (!pCm->m_IsCollision || !pCm2->m_IsCollision)
	//			break;
	//
	//		//do collision checks
	//	}
	//}

	int skipId{ 1 };
	for (int i{}; i < size - 1; ++i)
	{
		for (int j{ skipId }; j < size; ++j)
		{
			MiniPhysicsComponent* pCm = m_pPhysicsComponents[i];
			MiniPhysicsComponent* pCm2 = m_pPhysicsComponents[j];

			if (!pCm->GetGameObject()->IsActive() || !pCm2->GetGameObject()->IsActive())
				continue;

			if (pCm->m_IsTrigger && pCm2->m_IsTrigger)
			{
				const bool check1{ bool(pCm->m_Mask & pCm2->m_IgnoreMask) };
				const bool check2{ bool(pCm2->m_Mask & pCm->m_IgnoreMask) };
				//if (check1)
				//	continue;
				//if (check2)
				//	continue;

				const Vector2& pos1 = pCm->m_pGameObject->GetTransform().GetPosition();
				Vector4 rect1{ pos1.x - pCm->m_Dimensions.x / 2.f, pos1.y - pCm->m_Dimensions.y / 2.f, pCm->m_Dimensions.x, pCm->m_Dimensions.y };
				const Vector2& pos2 = pCm2->m_pGameObject->GetTransform().GetPosition();
				Vector4 rect2{ pos2.x - pCm2->m_Dimensions.x / 2.f, pos2.y - pCm2->m_Dimensions.y / 2.f, pCm2->m_Dimensions.x, pCm2->m_Dimensions.y };
				if (CollisionFunc::IsOverlapping(rect1, rect2))
				{
					if (!check2)
						pCm->OnTrigger();

					if (!check1)
						pCm2->OnTrigger();
				}
			}
		}
		++skipId; //skip double checking
	}
}

void MiniPhysicsManager::Render() const
{
	//TODO: draw debugs
}

void MiniPhysicsManager::AddPhysicsComponent(MiniPhysicsComponent* pComp)
{
	m_pPhysicsComponents.push_back(pComp);
}