#include "MiniginPCH.h"
#include "Commands.h"

#include <iostream>

void MoveCommand::Execute()
{

	std::cout << "Move\r";
}

void SprintCommand::Execute()
{
	std::cout << "Sprint\r";
}

void LookCommand::Execute()
{
	std::cout << "Look\r";
}

void KnifeLunge::Execute()
{
	std::cout << "KnifeLunge\r";
}

void JumpCommand::Execute()
{
	std::cout << "Jump\r";
}

void ReloadCommand::Execute()
{
	std::cout << "Reload\r";
}

void ProneCommand::Execute()
{
	std::cout << "Prone\r";
}

void SwapWeaponsCommand::Execute()
{
	std::cout << "SwapWeapons\r";
}

void MenuCommand::Execute()
{
	std::cout << "Menu\r";
}

void MapCommand::Execute()
{
	std::cout << "Map\r";
}

void AimCommand::Execute()
{
	std::cout << "Aim\r";
}

void Equipment1Command::Execute()
{
	std::cout << "Equipment1\r";
}

void FireCommand::Execute()
{
	std::cout << "Fire\r";
}

void Equipment2Command::Execute()
{
	std::cout << "Equipment2\r";
}