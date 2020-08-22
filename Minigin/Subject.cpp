#include "MiniginPCH.h"
#include "Subject.h"
#include "Observer.h"

Subject::Subject(unsigned int numObs)
	: m_pObservers{ new Observer*[numObs] }
	, m_NumObservers{}
	, m_MaxNumObservers{ numObs }
{}

Subject::~Subject()
{
	for (unsigned int i{}; i < m_MaxNumObservers; ++i)
	{
		m_pObservers[i] = nullptr;
	}
	m_pObservers = nullptr;
}

void Subject::AddObserver(Observer* pObserver)
{
	if (m_NumObservers >= m_MaxNumObservers)
		return;

	for (unsigned int i{}; i < m_NumObservers; ++i)
	{
		if (m_pObservers[i] == pObserver)
			return;
	}
	m_pObservers[m_NumObservers] = pObserver;
	++m_NumObservers;
}

void Subject::RemoveObserver(Observer* pObserver)
{
	for (unsigned int i{}; i < m_NumObservers; ++i)
	{
		if (m_pObservers[i] == pObserver)
		{
			m_pObservers[i] = m_pObservers[m_NumObservers]; //swap out last one with one to remove
			--m_NumObservers; //decrease count
			break;
		}
	}
}

void Subject::Notify(GameObject* pGameObject, int event)
{
	for (unsigned int i{}; i < m_NumObservers; i++)
	{
		m_pObservers[i]->OnNotify(pGameObject, event);
	}
}