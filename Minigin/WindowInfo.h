#pragma once

struct WindowInfo
{
public:
	WindowInfo(const char* pTitle = "Test", int w = 640, int h = 480, int msPF = 16)
		: pTitle{ pTitle }
		, Width{ w }
		, Height{ h }
		, MsPerFrame{ msPF }
	{}

	const char* pTitle;
	const int Width;
	const int Height;
	//16 for 60 fps, 33 for 30 fps
	const int MsPerFrame;
};