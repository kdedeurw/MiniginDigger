#pragma once
#include <map>
#include <windows.h>

class Command;

enum PlayerId
{
	Player1,
	Player2,
	Player3,
	Player4,
	KeyboardAndMouse,
};

enum class Action
{
	Move,
	Sprint,
	Look,
	LungeKnife,
	Jump,
	Reload,
	Prone,
	Swapweapons,
	Menu,
	Map,
	Aim,
	Equipment1,
	Fire,
	Equipment2,
	Left,
	Right,
	Up,
	Down,
};

class InputManager
{
public:
	virtual ~InputManager();

	virtual void ProcessInput() = 0;
	virtual bool IsAction(Action action) const = 0;
	virtual Command* HandleInput(PlayerId playerId = PlayerId::KeyboardAndMouse) const = 0;

protected:
	Command* m_pMove;
	Command* m_pSprint;
	Command* m_pLook;
	Command* m_pKnifeLunge;
	Command* m_pJump;
	Command* m_pReload;
	Command* m_pProne;
	Command* m_pSwapWeapons;
	Command* m_pMenu;
	Command* m_pMap;
	Command* m_pAim;
	Command* m_pEquipment1;
	Command* m_pFire;
	Command* m_pEquipment2;

	InputManager();

};