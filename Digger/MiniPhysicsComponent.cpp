#include "MiniginPCH.h"
#include "MiniPhysicsComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "MiniPhysicsManager.h"

MiniPhysicsManager* MiniPhysicsComponent::m_pManager{};

MiniPhysicsComponent::MiniPhysicsComponent()
	: m_Mask{}
	, m_IgnoreMask{}
	, m_IsCollision{}
	, m_IsTrigger{}
	, m_TriggerFunction{}
{
}

void MiniPhysicsComponent::Initialize()
{
	m_pManager->AddPhysicsComponent(this);
}

void MiniPhysicsComponent::Render() const
{
	//TODO: draw debugs
	//const Vector2& pos = m_pGameObject->GetTransform().GetPosition();
	//Vector2 line1{ pos.x - m_Dimensions.x / 2, pos.y - m_Dimensions.y / 2 };
	//Vector2 line2{ pos.x + m_Dimensions.x / 2, pos.y - m_Dimensions.y / 2 };
	//Renderer::GetInstance().DrawLine(line1, line2);
	//line1 = { pos.x + m_Dimensions.x / 2, pos.y - m_Dimensions.y / 2 };
	//line2 = { pos.x + m_Dimensions.x / 2, pos.y + m_Dimensions.y / 2 };
	//Renderer::GetInstance().DrawLine(line1, line2);
	//line1 = { pos.x + m_Dimensions.x / 2, pos.y + m_Dimensions.y / 2 };
	//line2 = { pos.x - m_Dimensions.x / 2, pos.y + m_Dimensions.y / 2 };
	//Renderer::GetInstance().DrawLine(line1, line2);
	//line1 = { pos.x - m_Dimensions.x / 2, pos.y + m_Dimensions.y / 2 };
	//line2 = { pos.x - m_Dimensions.x / 2, pos.y - m_Dimensions.y / 2 };
	//Renderer::GetInstance().DrawLine(line1, line2);
}

void MiniPhysicsComponent::Update()
{
	//TODO? update collisions
}

void MiniPhysicsComponent::SetDimensions(Vector2 dim)
{
	m_Dimensions = std::move(dim);
}

void MiniPhysicsComponent::SetCollisionMasks(unsigned int mask, unsigned int ignoreMask)
{
	m_Mask = mask;
	m_IgnoreMask = ignoreMask;
}

void MiniPhysicsComponent::SetCollisionChecks(bool isCollision, bool isTrigger)
{
	m_IsCollision = isCollision;
	m_IsTrigger = isTrigger;
}

void MiniPhysicsComponent::SetTriggerFunction(std::function<void(void)> triggerFunc)
{
	m_TriggerFunction = triggerFunc;
}

void MiniPhysicsComponent::OnTrigger()
{
	if (m_TriggerFunction)
		m_TriggerFunction();
}