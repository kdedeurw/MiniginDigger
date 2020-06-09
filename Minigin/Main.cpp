#include "MiniginPCH.h"
#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "BubbleBobbleGame.h"

int main(int, char* [])
{
	BubbleBobbleGame game;
	game.Start();
	return 0;
}