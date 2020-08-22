#include "MiniginPCH.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include <Box2D.h>

int PhysicsComponent::m_VelIterations{ 6 };
int PhysicsComponent::m_PosIterations{ 2 };
float PhysicsComponent::m_PhysicsTickRate{ 1.f / 60.f };
b2World PhysicsComponent::m_Box2DWorld{ b2Vec2{0.f, -9.81f} };

void PhysicsComponent::SetGravity(float x, float y)
{
	m_Box2DWorld.SetGravity(b2Vec2{ x, y });
}

void PhysicsComponent::SetGravity(const Vector2& gravity)
{
	SetGravity(gravity.x, gravity.y);
}

void PhysicsComponent::UpdatePhysics()
{
	//TODO: find better solution
	m_Box2DWorld.Step(m_PhysicsTickRate, m_VelIterations, m_PosIterations);
}

PhysicsComponent::PhysicsComponent()
	: m_pBody{}
{}

PhysicsComponent::~PhysicsComponent()
{
	m_pBody = nullptr;
}

void PhysicsComponent::Initialize()
{}

void PhysicsComponent::Render() const
{
	DrawDebug();
}

void PhysicsComponent::SetActive(bool isActive)
{
	m_pBody->SetEnabled(isActive);
	m_pGameObject->SetActive(isActive);
}

void PhysicsComponent::Update()
{
	const b2Vec2& pos = m_pBody->GetPosition();
	m_pGameObject->GetTransform().SetPosition(Vector2{ pos.x, pos.y });

	m_pGameObject->GetTransform().SetRotation(m_pBody->GetAngle());
}

void PhysicsComponent::Create(const Vector2& pos, Type type)
{
	b2BodyDef bodyDef{};
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.type = (b2BodyType)type;
	bodyDef.userData = this;
	m_pBody = m_Box2DWorld.CreateBody(&bodyDef);
}

void PhysicsComponent::CreateBox(const Vector2& pos, float m, const Vector2& dim, bool isCollider, short cBits, short mBits, Type type)
{
	//TODO: create in Initialize and use GameObject::Scale.xy

	Create(pos, type);
	b2PolygonShape shape{};
	shape.SetAsBox(dim.x, dim.y);
	b2FixtureDef fixDef{};
	fixDef.shape = &shape;
	fixDef.density = 1.f;
	fixDef.isSensor = isCollider;
	fixDef.filter.categoryBits = cBits;
	fixDef.filter.maskBits = mBits;
	m_pBody->CreateFixture(&fixDef);
	m_pBody->SetAwake(true);
	//pBody->SetUserData(this);
	SetMass(m);
	m_Dim = dim;
}

void PhysicsComponent::CreatePolygon(const Vector2&, float, const std::vector<Vector2>&, bool, short, short, Type)
{
	//TODO: add
	//Create(pos, type);
	//b2PolygonShape shape{};
}

void PhysicsComponent::CreateCircle(const Vector2&, float, float, bool, short, short, Type)
{
	//TODO: add
	//Create(pos, type);
}

void PhysicsComponent::SetType(Type type)
{
	m_pBody->SetType((b2BodyType)type);
}

void PhysicsComponent::ApplyForce(const Vector2& force)
{
	m_pBody->ApplyForce(b2Vec2{ force.x, force.y }, m_pBody->GetWorldCenter(), true);
}

void PhysicsComponent::ApplyTorque(float t)
{
	m_pBody->ApplyTorque(t, true);
}

void PhysicsComponent::SetMass(float m)
{
	b2MassData massData;
	m_pBody->GetMassData(&massData);
	massData.mass = m;
	m_pBody->SetMassData(&massData);
}

void PhysicsComponent::SetLinearVelocity(const Vector2& vel)
{
	m_pBody->SetLinearVelocity(b2Vec2{ vel.x, vel.y });
}

void PhysicsComponent::SetAngularVelocity(float w)
{
	m_pBody->SetAngularVelocity(w);
}

void PhysicsComponent::DrawDebug() const
{
	Renderer::GetInstance().DrawPoint(Vector2{ m_pBody->GetPosition().x, m_pBody->GetPosition().y });

	Vector2 p1{ m_pBody->GetPosition().x - m_Dim.x, m_pBody->GetPosition().y - m_Dim.y };
	Vector2 p2{ m_pBody->GetPosition().x + m_Dim.x, m_pBody->GetPosition().y - m_Dim.y };
	Vector2 p3{ m_pBody->GetPosition().x + m_Dim.x, m_pBody->GetPosition().y + m_Dim.y };
	Vector2 p4{ m_pBody->GetPosition().x - m_Dim.x, m_pBody->GetPosition().y + m_Dim.y };
	Renderer::GetInstance().DrawLine(p1, p2);
	Renderer::GetInstance().DrawLine(p2, p3);
	Renderer::GetInstance().DrawLine(p3, p4);
	Renderer::GetInstance().DrawLine(p4, p1);
}