#include "MiniginPCH.h"
#include "Gem.h"
#include "GameObject.h"
#include "Transform.h"
#include "GlobalMemoryPools.h"
#include "Subject.h"
#include "Events.h"

Gem::Gem()
	: m_pSubject{ GlobalMemoryPools::GetInstance().CreateSubject(1) }
{
}

Gem::~Gem()
{
}

void Gem::Initialize()
{
}

void Gem::Render() const
{
}

void Gem::Update()
{
}

void Gem::OnPickedUp()
{
	//Notify observer
	m_pSubject->Notify(m_pGameObject, (int)Event::event_item_pickup_Gem);

	m_pGameObject->SetActive(false);
}

Subject* Gem::GetSubject() const
{
	return m_pSubject;
}