#pragma once
#include "Transform.h"

class SceneObject
{
	friend class Scene;
public:
	SceneObject(Transform& transform)
		: m_Transform{ transform }
		, m_Id{ ++m_Ids }
	{}
	virtual ~SceneObject() = default;
	SceneObject(const SceneObject& other) = delete;
	SceneObject(SceneObject&& other) = delete;
	SceneObject& operator=(const SceneObject& other) = delete;
	SceneObject& operator=(SceneObject&& other) = delete;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() const = 0;
	virtual void DrawDebug() const = 0;

	Transform& GetTransform() const { return m_Transform; };

	const unsigned int GetId() const { return m_Id; };

protected:
	Transform& m_Transform;
	unsigned int m_Id;

private:
	static unsigned int m_Ids;

};