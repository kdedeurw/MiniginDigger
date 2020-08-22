#include "MiniginPCH.h"
#include "Transform.h"

Transform::Transform()
	: m_Position{}
	, m_Rotation{}
	, m_Scale{ 1.f, 1.f }
{}

Transform::Transform(float x, float y, float z, float r)
	: m_Position{ std::move(x), std::move(y), std::move(z) }
	, m_Rotation{ std::move(r) }
{}

Transform::Transform(Vector2 vec2, float z, float r)
	: Transform{ std::move(vec2.x), std::move(vec2.y), std::move(z), std::move(r) }
{}

Transform::Transform(Vector3 vec3, float r)
	: Transform{ std::move(vec3.x), std::move(vec3.y), std::move(vec3.z), std::move(r) }
{}

void Transform::SetPosition(float x, float y, float z)
{
	m_Position.x = std::move(x);
	m_Position.y = std::move(y);
	m_Position.z = std::move(z);
}

void Transform::SetPosition(Vector2 vec2, float z)
{
	m_Position = std::move(Vector3{ std::move(vec2.x), std::move(vec2.y), std::move(z) });
}

void Transform::SetPosition(Vector3 vec3)
{
	m_Position = std::move(vec3);
}

void Transform::Rotate(float r)
{
	m_Rotation += r;
}

void Transform::SetRotation(float r)
{
	m_Rotation = std::move(r);
}

void Transform::SetScale(float x, float y)
{
	m_Scale.x = std::move(x);
	m_Scale.y = std::move(y);
}

void Transform::operator+=(const Vector3& vec3)
{
	m_Position += vec3;
}

void Transform::operator-=(const Vector3& vec3)
{
	m_Position -= vec3;
}

void Transform::operator*=(const float scalar)
{
	m_Position *= scalar;
}

void Transform::operator/=(const float scalar)
{
	m_Position /= scalar;
}
