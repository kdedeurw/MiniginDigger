#pragma once

template <typename T>
class SingletonRef
{
public:
	static T& GetInstance()
	{
		static T instance{};
		return instance;
	}

	virtual ~SingletonRef() = default;
	SingletonRef(const SingletonRef& other) = delete;
	SingletonRef(SingletonRef&& other) = delete;
	SingletonRef& operator=(const SingletonRef& other) = delete;
	SingletonRef& operator=(SingletonRef&& other) = delete;

protected:
	SingletonRef() = default;
};