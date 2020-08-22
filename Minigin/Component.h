#pragma once

//size of class:
//86x (4 bytes == sizeof(void*)) = 8bytes (4bytes extra for vf ptr)
//64x (8 bytes == sizeof(void*)) = 16bytes (8bytes extra for vf ptr)

class GameObject;
class Transform;
class Component
{
public:
	Component()
		: m_pGameObject{}
	{}
	virtual ~Component()
	{
		m_pGameObject = nullptr;
	}
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Initialize() = 0;
	virtual void Render() const = 0;
	virtual void Update() = 0;

protected:
	friend class GameObject;
	GameObject* m_pGameObject;

private:
};

