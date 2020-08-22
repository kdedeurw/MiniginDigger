#include "MiniginPCH.h"
#include "MiniMath.h"
#include "Vectors.h"

RGBAColour::RGBAColour(int r, int g, int b, int a)
	: r{ std::move((uint8)r) }
	, g{ std::move((uint8)g) }
	, b{ std::move((uint8)b) }
	, a{ std::move((uint8)a) }
{}