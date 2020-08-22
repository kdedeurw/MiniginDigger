#pragma once
#include "Vectors.h"

namespace MiniMath
{
	static const float Pi = 3.14159265359f;
	static const float ToDegrees = 180.f / Pi;
	static const float ToRadians = Pi / 180.f;
}

class MiniMathFunc
{
private:
	MiniMathFunc() = delete;
	~MiniMathFunc() = delete;
public:
	const static size_t MaxColourRange = 255;

	//https://codeyarns.com/2010/08/26/c-static-function-in-header-file/
	//Error C4505, as the static function is defined in the header file,
	//for every .cpp file it is included in, it will compile a definition for that .cpp file
	//if the function is not used in that .cpp file, it will give errors
	//the function cannot be non-static in the header file, as this will give linker errors apparently
	//solutions: create .cpp function-body OR make function inline OR => encapsulate in class <=
	static float Clamp(float value, float min = 0.f, float max = 1.f)
	{
		if (value > max)
			return max;
		else if (value < min)
			return min;
		return value;
	}
};

struct RGBAColour
{
	using uint8 = unsigned char;

	RGBAColour(int r = 255, int g = 255, int b = 255, int a = 255);

	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;
};

/*
static SDL_Color operator+(const SDL_Color& sdl_color, const Vector4& RGBA)
{
	Uint8 r = (Uint8)Math::Clamp(sdl_color.r + RGBA.x, 0, Math::MaxColourRange);
	Uint8 g = (Uint8)Math::Clamp(sdl_color.g + RGBA.y, 0, Math::MaxColourRange);
	Uint8 b = (Uint8)Math::Clamp(sdl_color.b + RGBA.z, 0, Math::MaxColourRange);
	Uint8 a = (Uint8)Math::Clamp(sdl_color.a + RGBA.w, 0, Math::MaxColourRange);
	return SDL_Color{ r, g, b, a };
}
static SDL_Color operator+(const SDL_Color& sdl_color, const Vector3 RGB)
{
	Uint8 r = (Uint8)Clamp(sdl_color.r + RGB.x, 0, MaxColourRange);
	Uint8 g = (Uint8)Clamp(sdl_color.g + RGB.y, 0, MaxColourRange);
	Uint8 b = (Uint8)Clamp(sdl_color.b + RGB.z, 0, MaxColourRange);
	return SDL_Color{ r, g, b, sdl_color.a };
}

static void operator+=(SDL_Color& sdl_color, const Vector4& RGBA)
{
	sdl_color.r = (Uint8)Clamp(sdl_color.r + RGBA.x, 0, MaxColourRange);
	sdl_color.g = (Uint8)Clamp(sdl_color.g + RGBA.y, 0, MaxColourRange);
	sdl_color.b = (Uint8)Clamp(sdl_color.b + RGBA.z, 0, MaxColourRange);
	sdl_color.a = (Uint8)Clamp(sdl_color.a + RGBA.w, 0, MaxColourRange);
}
static void operator+=(SDL_Color& sdl_color, const Vector3 RGB)
{
	sdl_color.r = (Uint8)Clamp(sdl_color.r + RGB.x, 0, MaxColourRange);
	sdl_color.g = (Uint8)Clamp(sdl_color.g + RGB.y, 0, MaxColourRange);
	sdl_color.b = (Uint8)Clamp(sdl_color.b + RGB.z, 0, MaxColourRange);
}

static SDL_Color operator-(const SDL_Color& sdl_color, const Vector4& RGBA)
{
	Uint8 r = (Uint8)Clamp(sdl_color.r - RGBA.x, 0, MaxColourRange);
	Uint8 g = (Uint8)Clamp(sdl_color.g - RGBA.y, 0, MaxColourRange);
	Uint8 b = (Uint8)Clamp(sdl_color.b - RGBA.z, 0, MaxColourRange);
	Uint8 a = (Uint8)Clamp(sdl_color.a - RGBA.w, 0, MaxColourRange);
	return SDL_Color{ r, g, b, a };
}
static SDL_Color operator-(const SDL_Color& sdl_color, const Vector3 RGB)
{
	Uint8 r = (Uint8)Clamp(sdl_color.r - RGB.x, 0, MaxColourRange);
	Uint8 g = (Uint8)Clamp(sdl_color.g - RGB.y, 0, MaxColourRange);
	Uint8 b = (Uint8)Clamp(sdl_color.b - RGB.z, 0, MaxColourRange);
	return SDL_Color{ r, g, b, sdl_color.a };
}

static void operator-=(SDL_Color& sdl_color, const Vector4& RGBA)
{
	sdl_color.r = (Uint8)Clamp(sdl_color.r - RGBA.x, 0, MaxColourRange);
	sdl_color.g = (Uint8)Clamp(sdl_color.g - RGBA.y, 0, MaxColourRange);
	sdl_color.b = (Uint8)Clamp(sdl_color.b - RGBA.z, 0, MaxColourRange);
	sdl_color.a = (Uint8)Clamp(sdl_color.a - RGBA.w, 0, MaxColourRange);
}
static void operator-=(SDL_Color& sdl_color, const Vector3 RGB)
{
	sdl_color.r = (Uint8)Clamp(sdl_color.r - RGB.x, 0, MaxColourRange);
	sdl_color.g = (Uint8)Clamp(sdl_color.g - RGB.y, 0, MaxColourRange);
	sdl_color.b = (Uint8)Clamp(sdl_color.b - RGB.z, 0, MaxColourRange);
}
*/