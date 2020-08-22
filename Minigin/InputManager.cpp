#include "MiniginPCH.h"
#include "InputManager.h"

#include "Commands.h"

InputManager::InputManager()
{
	m_pMove = new MoveCommand{};
	m_pSprint = new SprintCommand{};
	m_pLook = new LookCommand{};
	m_pKnifeLunge = new KnifeLunge{};
	m_pJump = new JumpCommand{};
	m_pReload = new ReloadCommand{};
	m_pProne = new ProneCommand{};
	m_pSwapWeapons = new SwapWeaponsCommand{};
	m_pMenu = new MenuCommand{};
	m_pMap = new MapCommand{};
	m_pAim = new AimCommand{};
	m_pEquipment1 = new Equipment1Command{};
	m_pFire = new FireCommand{};
	m_pEquipment2 = new Equipment2Command{};
}

InputManager::~InputManager()
{
	if (m_pMove)
		delete m_pMove;
	m_pMove = nullptr;
	if (m_pSprint)
		delete m_pSprint;
	m_pSprint = nullptr;
	if (m_pLook)
		delete m_pLook;
	m_pLook = nullptr;
	if (m_pKnifeLunge)
		delete m_pKnifeLunge;
	m_pKnifeLunge = nullptr;
	if (m_pJump)
		delete m_pJump;
	m_pJump = nullptr;
	if (m_pReload)
		delete m_pReload;
	m_pReload = nullptr;
	if (m_pProne)
		delete m_pProne;
	m_pProne = nullptr;
	if (m_pSwapWeapons)
		delete m_pSwapWeapons;
	m_pSwapWeapons = nullptr;
	if (m_pMenu)
		delete m_pMenu;
	m_pMenu = nullptr;
	if (m_pMap)
		delete m_pMap;
	m_pMap = nullptr;
	if (m_pAim)
		delete m_pAim;
	m_pAim = nullptr;
	if (m_pEquipment1)
		delete m_pEquipment1;
	m_pEquipment1 = nullptr;
	if (m_pFire)
		delete m_pFire;
	m_pFire = nullptr;
	if (m_pEquipment2)
		delete m_pEquipment2;
	m_pEquipment2 = nullptr;
}
