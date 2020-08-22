#pragma once

class GameObject;
class Observer
{
public:
    Observer() = default;
    virtual ~Observer() = default;
    virtual void OnNotify(GameObject* pGameObject, int event) = 0;

protected:

private:
};