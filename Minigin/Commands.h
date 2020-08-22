#pragma once
#include "Command.h"

//Classic COD basic controller commands, debugging purposes
class MoveCommand : public Command
{
public:
	virtual void Execute() override;
};

class SprintCommand : public Command
{
public:
	virtual void Execute() override;
};

class LookCommand : public Command
{
public:
	virtual void Execute() override;
};

class KnifeLunge : public Command
{
public:
	virtual void Execute() override;
};

class JumpCommand : public Command
{
public:
	virtual void Execute() override;
};

class ReloadCommand : public Command
{
public:
	virtual void Execute() override;
};

class ProneCommand : public Command
{
public:
	virtual void Execute() override;
};

class SwapWeaponsCommand : public Command
{
public:
	virtual void Execute() override;
};

class MenuCommand : public Command
{
public:
	virtual void Execute() override;
};

class MapCommand : public Command
{
public:
	virtual void Execute() override;
};

class AimCommand : public Command
{
public:
	virtual void Execute() override;
};

class Equipment1Command : public Command
{
public:
	virtual void Execute() override;
};

class FireCommand : public Command
{
public:
	virtual void Execute() override;
};

class Equipment2Command : public Command
{
public:
	virtual void Execute() override;
};