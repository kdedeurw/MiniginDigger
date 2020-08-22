#pragma once
#include "Vectors.h"

class b2Body;
class RigidBody
{
public:
	enum class Type
	{
		Static,
		Kinematic,
		Dynamic,
	};

	RigidBody();
	virtual ~RigidBody();

	void DrawDebug() const;

	void CreateBox(const Vector2& pos, float m, const Vector2& dim, Type type = Type::Static);
	void CreatePolygon(const Vector2& pos, float m, const std::vector<Vector2>& points, Type type = Type::Static);
	void CreateCircle(const Vector2& pos, float m, float r, Type type = Type::Static);
	void SetType(Type type);
	void DestroyBody();

	//only applies force in center
	void ApplyForce(const Vector2& force);
	void ApplyLinearImpulse(const Vector2& force);
	void ApplyTorque(float t);
	void SetMass(float m);
	void SetLinearVelocity(const Vector2& vel);
	void SetAngularVelocity(float w);

	const Vector2 GetPosition();

private:
	b2Body* m_pBody;
	Vector2 m_Dim;

	void Create(const Vector2& pos, Type type);
};

