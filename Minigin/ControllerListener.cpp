#include "MiniginPCH.h"
#include "ControllerListener.h"

#include <iostream>

#include "Commands.h"
#include <set>
#include "MiniMath.h"

ControllerListener::ControllerListener()
    : m_ConnectedControllers{}
    , m_ControllerStates{}
    , m_ControllerKeystrokes{}
    , m_ButtonToActionBindings{}
    , m_MovableToActionBindings{}
    , m_ControllerBindings{}
{
    RegisterControllers();

    //SetActionBinding(Action::Swapweapons, Button::A);
}

ControllerListener::~ControllerListener()
{
    for (short unsigned int i{}; i < m_ConnectedControllers; ++i)
        VibrateController(VibrateStates::None, ControllerId(i));
    //reset controller's vibration
}

void ControllerListener::ProcessInput()
{
    //memset(&state,0,sizeof(XINPUT_STATE)); ==
    //RtlZeroMemory(&state, sizeof(XINPUT_STATE)); ==
    //ZeroMemory(&state, sizeof(XINPUT_STATE));

    //dwPacketNumber is increased everytime the controller changes input!
    //todo: comparison between number of every former and current frame

    // todo: read the input
    //flush current controller states
    ZeroMemory(m_ControllerStates, sizeof(XINPUT_STATE) * m_ConnectedControllers);

    DWORD dwStateResult;
    for (DWORD i = 0; i < (DWORD)m_ConnectedControllers; i++)
    {
        XINPUT_STATE state;
        dwStateResult = XInputGetState(i, &state);

        if (dwStateResult == ERROR_SUCCESS)
        {
            // Controller is connected
            // Only then store state result
            m_ControllerStates[i] = std::move(state);

            //if (controllerStates[i].Gamepad.wButtons == XINPUT_GAMEPAD_START)
            //    RegisterControllers();
        }
        else
        {
            // Controller is not connected
            //ZeroMemory(&controllerStates[i], sizeof(XINPUT_STATE));
        }
    }
}

void ControllerListener::ProcessInputLegacy()
{
    ZeroMemory(m_ControllerStates, sizeof(XINPUT_STATE) * XUSER_MAX_COUNT);

    DWORD dwStateResult;
    for (DWORD i = 0; i < (DWORD)m_ConnectedControllers; i++)
    {
        XINPUT_STATE state;
        dwStateResult = XInputGetState(i, &state);

        //XINPUT_KEYSTROKE error check is deprecated,
        //This check prevents crashing
        if (dwStateResult == ERROR_SUCCESS)
        {
            m_ControllerStates[i] = std::move(state);
            ZeroMemory(&m_ControllerKeystrokes[i], sizeof(XINPUT_KEYSTROKE));
            if (XInputGetKeystroke(i, 0, &m_ControllerKeystrokes[i]) == ERROR_SUCCESS)
            {
                // Process keystroke input
                // Useless/double check, if statement for debugging purposes
            }
        }
        else
        {
            //ZeroMemory(&controllerStates[i], sizeof(XINPUT_STATE));
        }
    }
}

bool ControllerListener::IsPressed(Button button, ControllerId id) const
{
    // todo: return whether the given button is pressed or not.
    int i{ (int)id };
    switch (button)
    {
    case Button::A:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_A;
    case Button::B:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_B;
    case Button::X:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_X;
    case Button::Y:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_Y;
    case Button::Left:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    case Button::Right:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
    case Button::Up:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
    case Button::Down:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    case Button::Select:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
    case Button::Start:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_START;
    case Button::LeftThumb:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    case Button::RightThumb:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
    case Button::LeftShoulder:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    case Button::RightShoulder:
        return m_ControllerStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
    }
    return false;
}

bool ControllerListener::IsPressedLegacy(Button button, ControllerId id) const
{
    int i{ (int)id };
    switch (button)
    {
    case Button::A:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_A;
    case Button::B:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_B;
    case Button::X:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_X;
    case Button::Y:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_Y;
    case Button::Left:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_DPAD_LEFT;
    case Button::Right:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_DPAD_RIGHT;
    case Button::Up:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_DPAD_UP;
    case Button::Down:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_DPAD_DOWN;
    case Button::Select:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_BACK;
    case Button::Start:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_START;
    case Button::LeftThumb:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_LTHUMB_PRESS;
    case Button::RightThumb:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_RTHUMB_PRESS;
    case Button::LeftShoulder:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_LSHOULDER;
    case Button::RightShoulder:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_RSHOULDER;
    }
    return false;
}

bool ControllerListener::IsMoving(Movable movable, ControllerId id) const
{
    int i{ (int)id };
    switch (movable)
    {
    case Movable::ThumbstickLeft:
        return ((m_ControllerStates[i].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || m_ControllerStates[i].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            || (m_ControllerStates[i].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || m_ControllerStates[i].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
    case Movable::ThumbstickRight:
        return ((m_ControllerStates[i].Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || m_ControllerStates[i].Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
            || (m_ControllerStates[i].Gamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || m_ControllerStates[i].Gamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
    case Movable::TriggerLeft:
        return m_ControllerStates[i].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    case Movable::TriggerRight:
        return m_ControllerStates[i].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    }
    return false;
}

bool ControllerListener::IsMovingLegacy(Movable movable, ControllerId id) const
{
    int i{ (int)id };
    switch (movable)
    {
    case Movable::ThumbstickLeft:
        return m_ControllerKeystrokes[i].VirtualKey >= VK_PAD_LTHUMB_UP && m_ControllerKeystrokes[i].VirtualKey <= VK_PAD_LTHUMB_DOWNLEFT;
    case Movable::ThumbstickRight:
        return m_ControllerKeystrokes[i].VirtualKey >= VK_PAD_RTHUMB_UP || m_ControllerKeystrokes[i].VirtualKey <= VK_PAD_RTHUMB_UP;
    case Movable::TriggerLeft:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_LTRIGGER;
    case Movable::TriggerRight:
        return m_ControllerKeystrokes[i].VirtualKey == VK_PAD_RTRIGGER;
    }
    return false;
}

Command* ControllerListener::HandleInput(PlayerId id) const
{
    //todo: use bitmask switch statement

    switch (m_ControllerStates[id].Gamepad.wButtons)
    {
    case XINPUT_GAMEPAD_A:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::A));
    case XINPUT_GAMEPAD_B:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::B));
    case XINPUT_GAMEPAD_X:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::X));
    case XINPUT_GAMEPAD_Y:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::Y));
    case XINPUT_GAMEPAD_DPAD_LEFT:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::Left));
    case XINPUT_GAMEPAD_DPAD_RIGHT:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::Right));
    case XINPUT_GAMEPAD_DPAD_UP:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::Up));
    case XINPUT_GAMEPAD_DPAD_DOWN:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::Down));
    case XINPUT_GAMEPAD_BACK:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::Select));
    case XINPUT_GAMEPAD_START:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::Start));
    case XINPUT_GAMEPAD_LEFT_THUMB:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::LeftThumb));
    case XINPUT_GAMEPAD_RIGHT_THUMB:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::RightThumb));
    case XINPUT_GAMEPAD_LEFT_SHOULDER:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::LeftShoulder));
    case XINPUT_GAMEPAD_RIGHT_SHOULDER:
        return m_ControllerBindings[id].at(m_ButtonToActionBindings[id].at(Button::RightShoulder));
    }

    if (m_ControllerStates[id].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
        return m_ControllerBindings[id].at(m_MovableToActionBindings[id].at(Movable::TriggerLeft));
    if (m_ControllerStates[id].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
        return m_ControllerBindings[id].at(m_MovableToActionBindings[id].at(Movable::TriggerRight));

    if ((m_ControllerStates[id].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || m_ControllerStates[id].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        || (m_ControllerStates[id].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || m_ControllerStates[id].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
        return m_ControllerBindings[id].at(m_MovableToActionBindings[id].at(Movable::ThumbstickLeft));
    if ((m_ControllerStates[id].Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || m_ControllerStates[id].Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        || (m_ControllerStates[id].Gamepad.sThumbRX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || m_ControllerStates[id].Gamepad.sThumbRY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
        return m_ControllerBindings[id].at(m_MovableToActionBindings[id].at(Movable::ThumbstickRight));

    return nullptr;
}

short unsigned int ControllerListener::RegisterControllers()
{
    m_ConnectedControllers = 0;
    DWORD dwResult;
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        dwResult = XInputGetState(i, &state);

        if (dwResult == ERROR_SUCCESS)
        {
            std::cout << "Controller: " << i+1 << " is connected\n";
            ++m_ConnectedControllers;
            SetControllerBindings(ControllerId(i));
            SetActionBindings(ControllerId(i));
        }
        else
            std::cout << "Controller: " << i+1 << " is disconnected\n";
    }
    std::cout << m_ConnectedControllers << " controller(s) connected\n";
    return m_ConnectedControllers;
}

void ControllerListener::SetControllerBindings(ControllerId id)
{
    m_ControllerBindings[id][Action::Move] = m_pMove;
    m_ControllerBindings[id][Action::Sprint] = m_pSprint;
    m_ControllerBindings[id][Action::Look] = m_pLook;
    m_ControllerBindings[id][Action::LungeKnife] = m_pKnifeLunge;
    m_ControllerBindings[id][Action::Jump] = m_pJump;
    m_ControllerBindings[id][Action::Reload] = m_pReload;
    m_ControllerBindings[id][Action::Prone] = m_pProne;
    m_ControllerBindings[id][Action::Swapweapons] = m_pSwapWeapons;
    m_ControllerBindings[id][Action::Menu] = m_pMenu;
    m_ControllerBindings[id][Action::Map] = m_pMap;
    m_ControllerBindings[id][Action::Aim] = m_pAim;
    m_ControllerBindings[id][Action::Equipment1] = m_pEquipment1;
    m_ControllerBindings[id][Action::Fire] = m_pFire;
    m_ControllerBindings[id][Action::Equipment2] = m_pEquipment2;
    m_ControllerBindings[id][Action::Left] = nullptr;
    m_ControllerBindings[id][Action::Right] = nullptr;
    m_ControllerBindings[id][Action::Up] = nullptr;
    m_ControllerBindings[id][Action::Down] = nullptr;
}

void ControllerListener::SetControllerBinding(Command* pCommand, Action action, ControllerId id)
{
    m_ControllerBindings[id][action] = pCommand;
    std::cout << "\n!No safety checks just yet, be sure what you're doing here!\n";
}

void ControllerListener::SetActionBindings(ControllerId id)
{
    //Thumbsticks
    m_MovableToActionBindings[id][Movable::ThumbstickLeft] = Action::Move;
    m_ButtonToActionBindings[id][Button::LeftThumb] = Action::Sprint;
    m_MovableToActionBindings[id][Movable::ThumbstickRight] = Action::Look;
    m_ButtonToActionBindings[id][Button::RightThumb] = Action::LungeKnife;

    //ABXY
    m_ButtonToActionBindings[id][Button::A] = Action::Jump;
    m_ButtonToActionBindings[id][Button::X] = Action::Reload;
    m_ButtonToActionBindings[id][Button::B] = Action::Prone;
    m_ButtonToActionBindings[id][Button::Y] = Action::Swapweapons;

    //DPAD
    m_ButtonToActionBindings[id][Button::Left] = Action::Left;
    m_ButtonToActionBindings[id][Button::Right] = Action::Right;
    m_ButtonToActionBindings[id][Button::Up] = Action::Up;
    m_ButtonToActionBindings[id][Button::Down] = Action::Down;

    //Middle buttons
    m_ButtonToActionBindings[id][Button::Start] = Action::Menu;
    m_ButtonToActionBindings[id][Button::Select] = Action::Map;

    //Back buttons
    m_ButtonToActionBindings[id][Button::LeftShoulder] = Action::Aim;
    m_MovableToActionBindings[id][Movable::TriggerLeft] = Action::Equipment1;
    m_ButtonToActionBindings[id][Button::RightShoulder] = Action::Fire;
    m_MovableToActionBindings[id][Movable::TriggerRight] = Action::Equipment2;

    //Reverted
    m_ActionToMovableBindings[id][Action::Move] = Movable::ThumbstickLeft;
    m_ActionToButtonBindings[id][Action::Sprint] = Button::LeftThumb;
    m_ActionToMovableBindings[id][Action::Look] = Movable::ThumbstickRight;
    m_ActionToButtonBindings[id][Action::LungeKnife] = Button::RightThumb;
    m_ActionToButtonBindings[id][Action::Jump] = Button::A;
    m_ActionToButtonBindings[id][Action::Reload] = Button::X;
    m_ActionToButtonBindings[id][Action::Prone] = Button::B;
    m_ActionToButtonBindings[id][Action::Swapweapons] = Button::Y;
    m_ActionToButtonBindings[id][Action::Menu] = Button::Start;
    m_ActionToButtonBindings[id][Action::Map] = Button::Select;
    m_ActionToButtonBindings[id][Action::Aim] = Button::LeftShoulder;
    m_ActionToMovableBindings[id][Action::Equipment1] = Movable::TriggerLeft;
    m_ActionToButtonBindings[id][Action::Fire] = Button::RightShoulder;
    m_ActionToMovableBindings[id][Action::Equipment2] = Movable::TriggerRight;
    m_ActionToButtonBindings[id][Action::Left] = Button::Left;
    m_ActionToButtonBindings[id][Action::Right] = Button::Right;
    m_ActionToButtonBindings[id][Action::Up] = Button::Up;
    m_ActionToButtonBindings[id][Action::Down] = Button::Down;
}

void ControllerListener::SetActionBinding(Action action, Button button, ControllerId id)
{
    if (action != Action::Move && action != Action::Look)
    {
        auto it = m_ButtonToActionBindings[id].find(button);
        //Button previousButton = it->first; //store old button
        Action previousAction = it->second; //store old action
        it->second = action; //overwrite old action to new action

        auto it2 = m_ActionToButtonBindings[id].find(action);
        Button newButton = it2->second; //store button of previous action
        m_ButtonToActionBindings[id].find(newButton)->second = previousAction;

        //m_ButtonToActionBindings[id].find(button)->second = action;
        if (action == Action::Equipment1 || action == Action::Equipment2)
            std::cout << "\n!Bound (if possible) movable action to static button instead of movable trigger!\n";
    }
    else std::cout << "\n!No suitable binding found!\nUse a movable for dynamic bindings\n";
}

void ControllerListener::SetActionBinding(Action action, Movable movable, ControllerId id)
{
    if (movable == Movable::TriggerLeft || movable == Movable::TriggerRight)
    {
        if (action == Action::Move || action == Action::Look)
        {
            std::cout << "\n!No suitable binding found!\nUse trigger action for non-vector movable bindings";
            return;
        }
        if (action != Action::Equipment1 && action != Action::Equipment2)
        {
            m_MovableToActionBindings[id][movable] = action;
            std::cout << "\n!Bound action to (if possible) movable trigger instead of static button!\n"
                        << "The trigger will act as a button when trigger deadzone threshold is met\n";
        }
    }
    else if ((action == Action::Move || action == Action::Look) && (movable == Movable::ThumbstickLeft || movable == Movable::ThumbstickRight))
    {
        m_MovableToActionBindings[id][movable] = action;
    }
    else std::cout << "\n!No suitable binding found!\nUse a joystick action for vector movable bindings\n";

}

void ControllerListener::VibrateController(float low, float high, ControllerId id) const
{
    XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = XINPUT_MAX_MOTOR_INPUT * (WORD)MiniMathFunc::Clamp(high);
    vibration.wRightMotorSpeed = XINPUT_MAX_MOTOR_INPUT * (WORD)MiniMathFunc::Clamp(low);
    XInputSetState(id, &vibration);
}

void ControllerListener::VibrateController(VibrateStates state, ControllerId id) const
{
    XINPUT_VIBRATION vibration;
    switch (state)
    {
    case VibrateStates::None:
        vibration.wLeftMotorSpeed = 0;
        vibration.wRightMotorSpeed = 0;
        break;
    case VibrateStates::Low:
        vibration.wLeftMotorSpeed = XINPUT_LOW_MOTOR_INPUT;
        vibration.wRightMotorSpeed = XINPUT_LOW_MOTOR_INPUT;
        break;
    case VibrateStates::Med:
        vibration.wLeftMotorSpeed = XINPUT_MED_MOTOR_INPUT;
        vibration.wRightMotorSpeed = XINPUT_MED_MOTOR_INPUT;
        break;
    case VibrateStates::High:
        vibration.wLeftMotorSpeed = XINPUT_HIGH_MOTOR_INPUT;
        vibration.wRightMotorSpeed = XINPUT_HIGH_MOTOR_INPUT;
        break;
    case VibrateStates::Max:
        vibration.wLeftMotorSpeed = XINPUT_MAX_MOTOR_INPUT;
        vibration.wRightMotorSpeed = XINPUT_MAX_MOTOR_INPUT;
        break;
    }
    XInputSetState(id, &vibration);
}

bool ControllerListener::IsAction(Action) const
{
    return false;
}