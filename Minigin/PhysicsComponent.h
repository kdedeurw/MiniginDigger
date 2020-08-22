#pragma once
#include "Component.h"
#include "Vectors.h"
#include <vector>

class b2World;
class b2Body;
class PhysicsComponent : public Component
{
public:
	enum class Type
	{
		Static,
		Kinematic,
		Dynamic,
	};

	PhysicsComponent();
	~PhysicsComponent();

	static void SetGravity(float x, float y);
	static void SetGravity(const Vector2& gravity);

	static b2World& Getb2World() { return m_Box2DWorld; };
	static void UpdatePhysics();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() const override;

	void SetActive(bool isActive);

	void CreateBox(const Vector2& pos, float m, const Vector2& dim, bool isCollider = false, short cBits = 0, short mBits = 0, Type type = Type::Static);
	void CreatePolygon(const Vector2& pos, float m, const std::vector<Vector2>& points, bool isCollider = false, short cBits = 0, short mBits = 0, Type type = Type::Static);
	void CreateCircle(const Vector2& pos, float m, float r, bool isCollider = false, short cBits = 0, short mBits = 0, Type type = Type::Static);
	void SetType(Type type);

	//only applies force in center
	void ApplyForce(const Vector2& force);
	void ApplyTorque(float t);
	void SetMass(float m);
	void SetLinearVelocity(const Vector2& vel);
	void SetAngularVelocity(float w);

protected:
	static b2World m_Box2DWorld;

private:
	b2Body* m_pBody;
	Vector2 m_Dim;

	static int m_VelIterations;
	static int m_PosIterations;
	static float m_PhysicsTickRate;

	void Create(const Vector2& pos, Type type);

	void DrawDebug() const;

};