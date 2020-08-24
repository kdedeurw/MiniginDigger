#pragma once
#include <Component.h>

class Subject;
class Gem : public Component
{
public:
	Gem();
	~Gem();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	void OnPickedUp();
	Subject* GetSubject() const;

private:
	Subject* m_pSubject;
};