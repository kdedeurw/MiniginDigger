#pragma once
#include "SingletonRef.h"
#include "KeyboardMouseListener.h"
#include "ControllerListener.h"

class GlobalInput final : public SingletonRef<GlobalInput>
{
public:
	~GlobalInput() = default;
	GlobalInput(const GlobalInput& other) = delete;
	GlobalInput(GlobalInput&& other) = delete;
	GlobalInput& operator=(const GlobalInput& other) = delete;
	GlobalInput& operator=(GlobalInput&& other) = delete;

	//decided to make these public
	KeyboardMouseListener& KeyboardMouseListener{ KeyboardMouseListener::GetInstance() };
	ControllerListener& ControllerListener{ ControllerListener::GetInstance() };

private:
	friend static GlobalInput& SingletonRef<GlobalInput>::GetInstance();
	GlobalInput() = default;
};
