#pragma once
#include "GlobalMemoryPools.h"

class SceneObject;
class Scene
{
public:
	virtual ~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void AddObject(SceneObject* pObject);

	virtual void Initialize();
	virtual void Update();
	virtual void Render() const;

	bool operator==(const Scene& other) const;
	bool operator==(Scene* pOther) const;

protected:
	//friend Scene* GlobalMemoryPools::CreateScene(const std::string& name);
	friend class GlobalMemoryPools;
	explicit Scene(std::string name);

	friend class SceneManager; //why not
	std::string m_Name;
	std::vector<SceneObject*> m_pObjects;

private:
	static unsigned int m_IdCounter;
};