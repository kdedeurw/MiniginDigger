#pragma once

class Observer;
class GameObject;
class Subject
{
public:
    Subject(unsigned int numObs = 5);
    virtual ~Subject();

    void AddObserver(Observer* pObserver);
    void RemoveObserver(Observer* pObserver);

    void Notify(GameObject* pGameObject, int event);

private:
    Observer** m_pObservers;
    unsigned int m_NumObservers;
    unsigned int m_MaxNumObservers;

};
