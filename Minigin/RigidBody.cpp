#include "MiniginPCH.h"
#include "RigidBody.h"
#include <Box2D.h>
#include "Renderer.h"
#include "PhysicsComponent.h"

RigidBody::RigidBody()
	: m_pBody{}
{}

RigidBody::~RigidBody()
{
	m_pBody = nullptr;
}

void RigidBody::Create(const Vector2& pos, Type type)
{
	b2BodyDef bodyDef{};
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.type = (b2BodyType)type;
	bodyDef.userData = this;
	m_pBody = PhysicsComponent::Getb2World().CreateBody(&bodyDef);
}

void RigidBody::CreateBox(const Vector2& pos, float m, const Vector2& dim, Type type)
{
	Create(pos, type);
	b2PolygonShape shape{};
	shape.SetAsBox(dim.x, dim.y);
	b2FixtureDef fixDef{};
	fixDef.shape = &shape;
	fixDef.density = 1.f;
	m_pBody->CreateFixture(&fixDef);
	m_pBody->SetAwake(true);
	//pBody->SetUserData(this);
	SetMass(m);
	m_Dim = dim;
}

void RigidBody::CreatePolygon(const Vector2&, float, const std::vector<Vector2>&, Type)
{
	//TODO: add
	//Create(pos, type);
	//b2PolygonShape shape{};
}

void RigidBody::CreateCircle(const Vector2&, float, float, Type)
{
	//TODO: add
	//Create(pos, type);
}

void RigidBody::SetType(Type type)
{
	m_pBody->SetType((b2BodyType)type);
}

void RigidBody::DestroyBody()
{
	PhysicsComponent::Getb2World().DestroyBody(m_pBody);
	this->~RigidBody();
}

void RigidBody::ApplyForce(const Vector2& force)
{
	m_pBody->ApplyForce(b2Vec2{ force.x, force.y }, m_pBody->GetWorldCenter(), true);
}

void RigidBody::ApplyLinearImpulse(const Vector2& force)
{
	m_pBody->ApplyLinearImpulseToCenter(b2Vec2{ force.x, force.y }, true);
}

void RigidBody::ApplyTorque(float t)
{
	m_pBody->ApplyTorque(t, true);
}

void RigidBody::SetMass(float m)
{
	b2MassData massData;
	m_pBody->GetMassData(&massData);
	massData.mass = m;
	m_pBody->SetMassData(&massData);
}

void RigidBody::SetLinearVelocity(const Vector2& vel)
{
	m_pBody->SetLinearVelocity(b2Vec2{ vel.x, vel.y });
}

void RigidBody::SetAngularVelocity(float w)
{
	m_pBody->SetAngularVelocity(w);
}

const Vector2 RigidBody::GetPosition()
{
	const b2Vec2& pos = m_pBody->GetPosition();
	return Vector2{ pos.x, pos.y };
}

void RigidBody::DrawDebug() const
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