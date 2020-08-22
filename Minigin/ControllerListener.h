#pragma once
#include "InputManager.h"
#include "SingletonRef.h"
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

#define XINPUT_LOW_MOTOR_INPUT WORD(65535 * 0.25f);
#define XINPUT_MED_MOTOR_INPUT WORD(65535 * 0.5f);
#define XINPUT_HIGH_MOTOR_INPUT WORD(65535 * 0.75f);

#define XINPUT_MAX_MOTOR_INPUT (WORD)65535

struct Joystick
{
	Joystick(float x_, float y_)
		: x {x_}
		, y {y_}
	{
	}
	float x;
	float y;
};
enum ControllerId
{
	Controller1,
	Controller2,
	Controller3,
	Controller4,
};
enum class Button
{
	A,
	B,
	X,
	Y,
	Left,
	Right,
	Up,
	Down,
	Select,
	Start,
	LeftThumb,
	RightThumb,
	LeftShoulder,
	RightShoulder,
};
enum class Movable
{
	ThumbstickLeft,
	ThumbstickRight,
	TriggerLeft,
	TriggerRight,
};
enum class VibrateStates
{
	None,
	Low,
	Med,
	High,
	Max,
};

class ControllerListener final : public InputManager, public SingletonRef<ControllerListener>
{
public:
	~ControllerListener();

	virtual void ProcessInput() override;
	virtual bool IsAction(Action action) const override;

	void ProcessInputLegacy();
	bool IsPressed(Button button, ControllerId id = ControllerId::Controller1) const;
	bool IsPressedLegacy(Button button, ControllerId id = ControllerId::Controller1) const;
	bool IsMoving(Movable movable, ControllerId id = ControllerId::Controller1) const;
	bool IsMovingLegacy(Movable movable, ControllerId id = ControllerId::Controller1) const;
	//TODO: return thumbstick values

	virtual Command* HandleInput(PlayerId playerId = PlayerId::Player1) const override;

	short unsigned int RegisterControllers();
	void SetControllerBindings(ControllerId id = ControllerId::Controller1);
	void SetControllerBinding(Command* pCommand, Action action, ControllerId id = ControllerId::Controller1);
	void SetActionBindings(ControllerId id = ControllerId::Controller1);
	void SetActionBinding(Action action, Button button, ControllerId id = ControllerId::Controller1);
	void SetActionBinding(Action action, Movable movable, ControllerId id = ControllerId::Controller1);

	void VibrateController(float low, float high, ControllerId id = ControllerId::Controller1) const;
	void VibrateController(VibrateStates state, ControllerId id = ControllerId::Controller1) const;

private:
	friend static ControllerListener& SingletonRef<ControllerListener>::GetInstance();
	ControllerListener();

	short unsigned int m_ConnectedControllers;
	XINPUT_STATE m_ControllerStates[XUSER_MAX_COUNT];
	XINPUT_KEYSTROKE m_ControllerKeystrokes[XUSER_MAX_COUNT];

	std::map<Button, Action> m_ButtonToActionBindings[XUSER_MAX_COUNT];
	std::map<Movable, Action> m_MovableToActionBindings[XUSER_MAX_COUNT];
	std::map<Action, Button> m_ActionToButtonBindings[XUSER_MAX_COUNT];
	std::map<Action, Movable> m_ActionToMovableBindings[XUSER_MAX_COUNT];
	std::map<Action, Command*> m_ControllerBindings[XUSER_MAX_COUNT];
};

