#include "MiniginPCH.h"
#include "KeyboardMouseListener.h"
#include <iostream>
#include "GameState.h"
#include "WindowInfo.h"

KeyboardMouseListener::KeyboardMouseListener()
	: m_MousePos{}
    , m_Keys{}
    , m_MouseButtons{}
    , m_ScrollWheel{}
    , m_IsMouseMoved{}
	, m_KeyToActionBindings{}
	, m_MouseButtonToActionBindings{}
	, m_ScrollWheelToActionBindings{}
	, m_MouseMoveToActionBinding{}
	, m_ActionToKeyBindings{}
	, m_ActionToMouseButtonBindings{}
	, m_ActionToScrollWheelBindings{}
	, m_KeyboardMouseBindings{}
	, m_IsQuit{}
{
	InitializeKeys();
	InitializeMouseButtons();
	SetActionBindings();
	SetKeyBindings();
}

KeyboardMouseListener::~KeyboardMouseListener()
{}

bool KeyboardMouseListener::IsQuit() const
{
	return m_IsQuit;
}

void KeyboardMouseListener::ProcessInput()
{
	m_IsMouseMoved = false;
	m_ScrollWheel = 0;

    SDL_Event e;
    while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			m_IsQuit = true;
			break;
		case SDL_KEYDOWN:
			HandleKeyEvent(e.key, true);
			break;
		case SDL_KEYUP:
			HandleKeyEvent(e.key, false);
			break;
		case SDL_MOUSEMOTION:
			HandleMouseMotionEvent(e.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			HandleMouseEvent(e.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			HandleMouseEvent(e.button, false);
			break;
		case SDL_MOUSEWHEEL:
			HandleMouseWheelEvent(e.wheel);
			break;
		}
    }
}

void KeyboardMouseListener::HandleKeyEvent(SDL_KeyboardEvent& e, bool isPressed)
{
	switch (e.keysym.sym)
	{
	case SDLK_w:
		m_Keys[Key::W] = isPressed;
		break;
	case SDLK_a:
		m_Keys[Key::A] = isPressed;
		break;
	case SDLK_s:
		m_Keys[Key::S] = isPressed;
		break;
	case SDLK_d:
		m_Keys[Key::D] = isPressed;
		break;
	case SDLK_q:
		m_Keys[Key::Q] = isPressed;
		break;
	case SDLK_e:
		m_Keys[Key::E] = isPressed;
		break;
	case SDLK_r:
		m_Keys[Key::R] = isPressed;
		break;
	case SDLK_f:
		m_Keys[Key::F] = isPressed;
		break;
	case SDLK_g:
		m_Keys[Key::G] = isPressed;
		break;
	case SDLK_c:
		m_Keys[Key::C] = isPressed;
		break;
	case SDLK_v:
		m_Keys[Key::V] = isPressed;
		break;
	case SDLK_i:
		m_Keys[Key::I] = isPressed;
		break;
	case SDLK_m:
		m_Keys[Key::M] = isPressed;
		break;
	case SDLK_p:
		m_Keys[Key::P] = isPressed;
		break;
	case SDLK_0:
		m_Keys[Key::Zero] = isPressed;
		break;
	case SDLK_1:
		m_Keys[Key::One] = isPressed;
		break;
	case SDLK_2:
		m_Keys[Key::Two] = isPressed;
		break;
	case SDLK_3:
		m_Keys[Key::Three] = isPressed;
		break;
	case SDLK_4:
		m_Keys[Key::Four] = isPressed;
		break;
	case SDLK_5:
		m_Keys[Key::Five] = isPressed;
		break;
	case SDLK_LEFT:
		m_Keys[Key::ArrowLeft] = isPressed;
		break;
	case SDLK_RIGHT:
		m_Keys[Key::ArrowRight] = isPressed;
		break;
	case SDLK_UP:
		m_Keys[Key::ArrowUp] = isPressed;
		break;
	case SDLK_DOWN:
		m_Keys[Key::ArrowDown] = isPressed;
		break;
	case SDLK_SPACE:
		m_Keys[Key::Space] = isPressed;
		break;
	case SDLK_LSHIFT:
		m_Keys[Key::LShift] = isPressed;
		break;
	case SDLK_TAB:
		m_Keys[Key::Tab] = isPressed;
		break;
	case SDLK_ESCAPE:
		m_Keys[Key::Escape] = isPressed;
		break;
	case SDLK_RETURN:
		m_Keys[Key::Enter] = isPressed;
		break;
	case SDLK_BACKSPACE:
		m_Keys[Key::Backspace] = isPressed;
		break;
	}
}

void KeyboardMouseListener::HandleMouseMotionEvent(SDL_MouseMotionEvent& e)
{
	if (m_MousePos.x != float(e.x) || m_MousePos.y != float(e.y))
		m_IsMouseMoved = true;

	m_MousePos.x = float(e.x);
	m_MousePos.y = GameState::GetInstance().pWindowInfo->Height - float(e.y);
}

void KeyboardMouseListener::HandleMouseEvent(SDL_MouseButtonEvent& e, bool isPressed)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		m_MouseButtons[Mouse::LMB] = isPressed;
		break;
	case SDL_BUTTON_RIGHT:
		m_MouseButtons[Mouse::RMB] = isPressed;
		break;
	case SDL_BUTTON_MIDDLE:
		m_MouseButtons[Mouse::Mouse3] = isPressed;
		break;
	}
}

void KeyboardMouseListener::HandleMouseWheelEvent(SDL_MouseWheelEvent& e)
{
	m_ScrollWheel = e.y; //typedef int by default
}

const Vector2& KeyboardMouseListener::GetMousePos() const
{
	return m_MousePos;
}

bool KeyboardMouseListener::IsScrollWheelMove(ScrollWheel scrollWheel) const
{
	switch (scrollWheel)
	{
	case ScrollWheel::Up:
		return m_ScrollWheel == 1;
		break;
	case ScrollWheel::Down:
		return m_ScrollWheel == -1;
		break;
	}
	return 0;
}

int KeyboardMouseListener::GetScrollWheel() const
{
    return m_ScrollWheel;
}

bool KeyboardMouseListener::IsAction(Action) const
{
	return false;
}

bool KeyboardMouseListener::IsPressed(Key key) const
{
    return m_Keys.at(key);
}

bool KeyboardMouseListener::IsMouseButtonPressed(Mouse button)
{
    return m_MouseButtons.at(button);
}

bool KeyboardMouseListener::IsMouseMoving() const
{
    return m_IsMouseMoved;
}

Command* KeyboardMouseListener::HandleInput(PlayerId) const
{
	return nullptr;
}

void KeyboardMouseListener::SetKeyBindings()
{
	m_KeyboardMouseBindings[Action::Move] = m_pMove;
	m_KeyboardMouseBindings[Action::Sprint] = m_pSprint;
	m_KeyboardMouseBindings[Action::Look] = m_pLook;
	m_KeyboardMouseBindings[Action::LungeKnife] = m_pKnifeLunge;
	m_KeyboardMouseBindings[Action::Jump] = m_pJump;
	m_KeyboardMouseBindings[Action::Reload] = m_pReload;
	m_KeyboardMouseBindings[Action::Prone] = m_pProne;
	m_KeyboardMouseBindings[Action::Swapweapons] = m_pSwapWeapons;
	m_KeyboardMouseBindings[Action::Menu] = m_pMenu;
	m_KeyboardMouseBindings[Action::Map] = m_pMap;
	m_KeyboardMouseBindings[Action::Aim] = m_pAim;
	m_KeyboardMouseBindings[Action::Equipment1] = m_pEquipment1;
	m_KeyboardMouseBindings[Action::Fire] = m_pFire;
	m_KeyboardMouseBindings[Action::Equipment2] = m_pEquipment2;
	m_KeyboardMouseBindings[Action::Left] = nullptr;
	m_KeyboardMouseBindings[Action::Right] = nullptr;
	m_KeyboardMouseBindings[Action::Up] = nullptr;
	m_KeyboardMouseBindings[Action::Down] = nullptr;
}

void KeyboardMouseListener::SetKeyBinding(Command* pCommand, Action action)
{
	m_KeyboardMouseBindings[action] = pCommand;
	std::cout << "\n!No safety checks just yet, be sure what you're doing here!\n";
}

void KeyboardMouseListener::SetActionBindings()
{
	//WASD
	m_KeyToActionBindings[Key::W] = Action::Move;
	m_KeyToActionBindings[Key::A] = Action::Move;
	m_KeyToActionBindings[Key::S] = Action::Move;
	m_KeyToActionBindings[Key::D] = Action::Move;

	//QWERTY
	m_KeyToActionBindings[Key::Space] = Action::Jump;
	m_KeyToActionBindings[Key::R] = Action::Reload;
	m_KeyToActionBindings[Key::C] = Action::Prone;
	m_KeyToActionBindings[Key::LShift] = Action::Sprint;
	m_KeyToActionBindings[Key::V] = Action::LungeKnife;
	m_KeyToActionBindings[Key::M] = Action::Map;

	//NUMPAD
	m_KeyToActionBindings[Key::One] = Action::Equipment1;
	m_KeyToActionBindings[Key::Two] = Action::Equipment2;

	//MISC
	m_KeyToActionBindings[Key::Escape] = Action::Menu;

	//ArrowKeys
	m_KeyToActionBindings[Key::ArrowLeft] = Action::Left;
	m_KeyToActionBindings[Key::ArrowRight] = Action::Right;
	m_KeyToActionBindings[Key::ArrowUp] = Action::Up;
	m_KeyToActionBindings[Key::ArrowDown] = Action::Down;

	//MouseButtons
	m_MouseButtonToActionBindings[Mouse::RMB] = Action::Aim;
	m_MouseButtonToActionBindings[Mouse::LMB] = Action::Fire;

	//MouseMove
	//m_MouseMoveToActionBinding[m_MousePos] = Action::Look;

	//ScrollWheel
	m_ScrollWheelToActionBindings[ScrollWheel::Up] = Action::Swapweapons;
	m_ScrollWheelToActionBindings[ScrollWheel::Down] = Action::Swapweapons;

	//Reverted
	m_ActionToKeyBindings[Action::Move] = Key::W;
	m_ActionToKeyBindings[Action::Move] = Key::A;
	m_ActionToKeyBindings[Action::Move] = Key::S;
	m_ActionToKeyBindings[Action::Move] = Key::D;
	m_ActionToKeyBindings[Action::Sprint] = Key::LShift;
	//m_MouseMoveToActionBinding[Action::Look];
	m_ActionToKeyBindings[Action::LungeKnife] = Key::V;
	m_ActionToKeyBindings[Action::Jump] = Key::Space;
	m_ActionToKeyBindings[Action::Reload] = Key::R;
	m_ActionToKeyBindings[Action::Prone] = Key::C;
	m_ActionToScrollWheelBindings[Action::Swapweapons] = ScrollWheel::Up;
	m_ActionToScrollWheelBindings[Action::Swapweapons] = ScrollWheel::Down;
	m_ActionToKeyBindings[Action::Menu] = Key::Escape;
	m_ActionToKeyBindings[Action::Map] = Key::M;
	m_ActionToMouseButtonBindings[Action::Aim] = Mouse::RMB;
	m_ActionToKeyBindings[Action::Equipment1] = Key::One;
	m_ActionToMouseButtonBindings[Action::Fire] = Mouse::LMB;
	m_ActionToKeyBindings[Action::Equipment2] = Key::Two;
	m_ActionToKeyBindings[Action::Left] = Key::ArrowLeft;
	m_ActionToKeyBindings[Action::Right] = Key::ArrowRight;
	m_ActionToKeyBindings[Action::Up] = Key::ArrowUp;
	m_ActionToKeyBindings[Action::Down] = Key::ArrowDown;
}

void KeyboardMouseListener::SetActionBinding(Action action, Key key)
{
	//TODO: moving and looking with WASD/ArrowKeys?

	auto it = m_KeyToActionBindings.find(key);
	//Key previousKey = it->first; //store old button
	Action previousAction = it->second; //store old action
	it->second = action; //overwrite old action to new action

	auto it2 = m_ActionToKeyBindings.find(action);
	Key newKey = it2->second; //store button of previous action
	m_KeyToActionBindings.find(newKey)->second = previousAction;
}

void KeyboardMouseListener::SetActionBinding(Action action, Mouse mouseButton)
{
	if (action == Action::Move || action == Action::Look)
	{
		std::cout << "\n!No suitable binding found for mousebutton!\n";
		return;
	}

	m_MouseButtonToActionBindings[mouseButton] = action;
}

void KeyboardMouseListener::SetActionBinding(Action action, ScrollWheel scrollWheel)
{
	if (action == Action::Move || action == Action::Look)
	{
		std::cout << "\n!No suitable binding found for scrollwheel!\n";
		return;
	}

	m_ScrollWheelToActionBindings[scrollWheel] = action;
}

void KeyboardMouseListener::InitializeKeys()
{
	m_Keys[Key::W] = false;
	m_Keys[Key::A] = false;
	m_Keys[Key::S] = false;
	m_Keys[Key::D] = false;
	m_Keys[Key::Q] = false;
	m_Keys[Key::E] = false;
	m_Keys[Key::R] = false;
	m_Keys[Key::F] = false;
	m_Keys[Key::G] = false;
	m_Keys[Key::C] = false;
	m_Keys[Key::V] = false;
	m_Keys[Key::I] = false;
	m_Keys[Key::M] = false;
	m_Keys[Key::P] = false;
	m_Keys[Key::Zero] = false;
	m_Keys[Key::One] = false;
	m_Keys[Key::Two] = false;
	m_Keys[Key::Three] = false;
	m_Keys[Key::Four] = false;
	m_Keys[Key::Five] = false;
	m_Keys[Key::ArrowLeft] = false;
	m_Keys[Key::ArrowRight] = false;
	m_Keys[Key::ArrowUp] = false;
	m_Keys[Key::ArrowDown] = false;
	m_Keys[Key::Space] = false;
	m_Keys[Key::LShift] = false;
	m_Keys[Key::Tab] = false;
	m_Keys[Key::Escape] = false;
	m_Keys[Key::Enter] = false;
	m_Keys[Key::Backspace] = false;
}

void KeyboardMouseListener::InitializeMouseButtons()
{
	m_MouseButtons[Mouse::LMB] = false;
	m_MouseButtons[Mouse::RMB] = false;
	m_MouseButtons[Mouse::Mouse3] = false;
}