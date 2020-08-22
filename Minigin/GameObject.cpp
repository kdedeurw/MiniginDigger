#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Components.h"

GameObject::GameObject(Transform& transform)
	: SceneObject{ transform }
	, m_pComponents{}
	, m_pChildren{}
	, m_pParent{}
	, m_pDebug{}
	, m_IsActive{ true }
{}

GameObject::~GameObject()
{
	m_pParent = nullptr;
	//delete all children
	for (GameObject* pChild : m_pChildren)
		delete pChild;
	m_pChildren.clear();
	//delete all components
	for (Component* pComponent : m_pComponents)
		delete pComponent;
	m_pComponents.clear();

	delete m_pDebug;
	m_pDebug = nullptr;
}

void GameObject::Initialize()
{
	for (Component* pComponent : m_pComponents)
	{
		pComponent->Initialize();
	}

	for (GameObject* pObject : m_pChildren)
	{
		pObject->Initialize();
	}
}

void GameObject::Update()
{
	if (!m_IsActive)
		return;

	for (Component* pComponent : m_pComponents)
	{
		pComponent->Update();
	}

	for (GameObject* pObject : m_pChildren)
	{
		pObject->Update();
	}
}

void GameObject::Render() const
{
	if (!m_IsActive)
		return;

	if (m_pParent)
	{
		for (Component* pComponent : m_pComponents)
		{
			pComponent->Render();
		}
	}
	else
	{
		for (Component* pComponent : m_pComponents)
		{
			pComponent->Render();
		}
	}

	for (GameObject* pObject : m_pChildren)
	{
		pObject->Render();
	}

	//DrawDebug();
}

void GameObject::AddComponent(Component* pComponent)
{
	if (std::find(m_pComponents.begin(), m_pComponents.end(), pComponent) == m_pComponents.end())
	{
		m_pComponents.push_back(pComponent);
		pComponent->m_pGameObject = this;
	}
}

void GameObject::RemoveComponent(Component* pComponent)
{
	const std::vector<Component*>::iterator it = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);
	if (it != m_pComponents.end())
		m_pComponents.erase(it);
}

void GameObject::AddChildObject(GameObject* pChild)
{
	if (std::find(m_pChildren.begin(), m_pChildren.end(), pChild) == m_pChildren.end())
	{
		m_pChildren.push_back(pChild);
		pChild->m_pParent = this;
	}
}

void GameObject::RemoveChildObject(GameObject* pChild)
{
	const std::vector<GameObject*>::iterator it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	if (it != m_pChildren.end())
	{
		m_pChildren.erase(it);
		pChild->m_pParent = nullptr;
	}
}

Vector2 GameObject::GetFinalPos() const
{
	if (!m_pParent)
		return m_Transform.GetPosition();

	Vector2 finalPos = m_Transform.GetPosition(); //vector3 : vector2
	GameObject* pParent{ m_pParent };
	do {
		const float parentRot = pParent->GetTransform().GetRotation() * MiniMath::ToRadians;
		const float sinCalc{ sinf(parentRot) };
		const float cosCalc{ cosf(parentRot) };
		const float newX{ finalPos.x * cosCalc - finalPos.y * sinCalc };//store new X, but don't overwrite original yet
		finalPos.y = finalPos.y * cosCalc + finalPos.x * sinCalc;
		finalPos.x = newX;
		finalPos += pParent->GetTransform().GetPosition();

		pParent = pParent->m_pParent;
	} while (pParent);
	return finalPos;
}

Transform GameObject::GetFinalTransform() const
{
	if (!m_pParent)
		return m_Transform;

	Transform finalTrans = m_Transform;
	Vector2& finalPos{ finalTrans.GetPosition() };
	Vector2& finalScale{ finalTrans.GetScale() };
	float& finalRot{ finalTrans.GetRotation() };

	GameObject* pParent{ m_pParent };
	do {
		const float parentRot = pParent->GetTransform().GetRotation() * MiniMath::ToRadians;
		const float sinCalc{ sinf(parentRot) };
		const float cosCalc{ cosf(parentRot) };
		const float newX{ finalPos.x * cosCalc - finalPos.y * sinCalc };
		finalPos.y = finalPos.y * cosCalc + finalPos.x * sinCalc;
		finalPos.x = newX;

		finalPos += pParent->GetTransform().GetPosition();
		finalScale += pParent->GetTransform().GetScale();
		finalRot += pParent->GetTransform().GetRotation();

		pParent = pParent->m_pParent;
	} while (pParent);
	return finalTrans;
}

void GameObject::DrawDebug() const
{
	Renderer::GetInstance().DrawDebug(GetFinalPos(), m_Id);
}