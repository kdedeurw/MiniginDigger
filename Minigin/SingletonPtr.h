#pragma once

template <typename T>
class SingletonPtr
{
public:
	//control creation
	static T* CreateInstance()
	{
		if (!m_pInstance)
			m_pInstance = new T{};
		return m_pInstance;
	}
	static T* GetInstance()
	{
		return m_pInstance;
	}
	//control deletion
	static void Delete()
	{
		if (m_pInstance)
			delete m_pInstance;
		m_pInstance = nullptr;
	}

	virtual ~SingletonPtr() = default;
	SingletonPtr(const SingletonPtr& other) = delete;
	SingletonPtr(SingletonPtr&& other) = delete;
	SingletonPtr& operator=(const SingletonPtr& other) = delete;
	SingletonPtr& operator=(SingletonPtr&& other) = delete;

protected:
	SingletonPtr() = default;

private:
	static T* m_pInstance;
};

template <typename T>
T* SingletonPtr<T>::m_pInstance{};
//T* SingletonPtr<T>::m_pInstance{ new T{} };