#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include <vector>
#include "Component.h"
#include "Scene.h"

class Transform;
class GameState;
class Texture2D;
class GameObject final : public SceneObject
{
public:
	GameObject(Transform& transform);
	virtual ~GameObject();
	GameObject(const GameObject & other) = delete;
	GameObject(GameObject && other) = delete;
	GameObject& operator=(const GameObject & other) = delete;
	GameObject& operator=(GameObject && other) = delete;

	void Initialize() override;
	void Update() override;
	void Render() const override;
	void DrawDebug() const override;

	Transform& GetTransform() { return m_Transform; };
	GameObject& GetParent() { return *m_pParent; };

	void AddComponent(Component* pComponent);
	template <typename T>
	T* GetComponent() const;
	void RemoveComponent(Component* pComponent);

	void AddChildObject(GameObject* pChild);
	std::vector<GameObject*>& GetChildren() { return m_pChildren; };
	void RemoveChildObject(GameObject* pChild);

	//get all child and parent calculated positions
	Vector2 GetFinalPos() const;
	//get all child and parent calculated transformations
	Transform GetFinalTransform() const;

	void SetActive(bool isActive) { m_IsActive = isActive; };

private:
	bool m_IsActive;
	GameObject* m_pParent;

	std::vector<Component*> m_pComponents;
	std::vector<GameObject*> m_pChildren;

	Texture2D* m_pDebug;
};

template <typename T>
T* GameObject::GetComponent() const
{
	for (Component* pComponent : m_pComponents)
	{
		if (typeid(*pComponent) == typeid(T))
			return dynamic_cast<T*>(pComponent);
	}
	return nullptr;
}