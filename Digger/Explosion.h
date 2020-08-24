#pragma once
#include <Component.h>
class Explosion : public Component
{
public:
	Explosion() = default;
	~Explosion() = default;

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	void SetLifeTime(float lifeTime);

private:
	float m_LifeTime;
};

