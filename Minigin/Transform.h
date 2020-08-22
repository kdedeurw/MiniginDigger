#pragma once
#include "Vectors.h"

class Transform final
{
public:
	Transform();
	//move constructor
	Transform(float x, float y, float z, float r = 0.0f);
	//move constructor
	Transform(Vector2 vec2, float z, float r = 0.0f);
	//move constructor
	Transform(Vector3 vec3, float r = 0.0f);

	~Transform() = default;
	Transform(const Transform& other) = default;
	Transform(Transform&& other) = default;
	Transform& operator=(const Transform& other) = delete;
	Transform& operator=(Transform&& other) = delete;

	Vector3& GetPosition() { return m_Position; }
	float& GetRotation() { return m_Rotation; };
	Vector2& GetScale() { return m_Scale; };

	void SetPosition(float x, float y, float z = 0.f);
	void SetPosition(Vector2 vec2, float z = 0.f);
	void SetPosition(Vector3 vec3);

	void Rotate(float r);
	void SetRotation(float r);

	void SetScale(float x, float y);
	void SetScale(Vector2 scale) { m_Scale = std::move(scale); };

	//add to position == translate
	void operator+=(const Vector3& vec3);
	//subtract from position == translate
	void operator-=(const Vector3& vec3);
	//multiply with position
	void operator*=(const float scalar);
	//divide from position
	void operator/=(const float scalar);

private:
	float m_Rotation;
	Vector2 m_Scale;
	Vector3 m_Position;
};
