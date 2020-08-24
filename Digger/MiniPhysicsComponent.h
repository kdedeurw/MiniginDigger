#pragma once
#include <Component.h>
#include <functional>
#include "Vectors.h"

class MiniPhysicsManager;
class MiniPhysicsComponent : public Component
{
public:
	MiniPhysicsComponent();
	~MiniPhysicsComponent() = default;

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	void SetDimensions(Vector2 dim);
	void SetCollisionMasks(unsigned int mask, unsigned int ignoreMask);
	void SetCollisionChecks(bool isCollision, bool isTrigger);
	void SetTriggerFunction(std::function<void(void)> triggerFunc);

	void OnTrigger();

protected:
	friend class MiniPhysicsManager;
	bool m_IsCollision, m_IsTrigger;
	unsigned int m_Mask, m_IgnoreMask;
	std::function<void(void)> m_TriggerFunction;
	Vector2 m_Dimensions;

	static MiniPhysicsManager* m_pManager;
};