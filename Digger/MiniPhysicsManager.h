#pragma once
#include "Component.h"
#include <vector>
#include "FixedSizeAllocator.h"

class MiniPhysicsComponent;
class MiniPhysicsManager : public Component
{
public:
	MiniPhysicsManager();
	~MiniPhysicsManager();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() const override;

	void AddPhysicsComponent(MiniPhysicsComponent* pComp);

private:
	//friend MiniPhysicsManager& SingletonRef<MiniPhysicsManager>::GetInstance();
	//MiniPhysicsManager();
	//singletonref remnants

	std::vector<MiniPhysicsComponent*> m_pPhysicsComponents;
};