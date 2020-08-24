#include "MiniginPCH.h"
#include "CollisionFunctionality.h"

bool CollisionFunc::IsOverlapping(const Vector4& rect1, const Vector4& rect2)
{
	// If one rectangle is on left side of the other
	if ((rect1.x + rect1.z) < rect2.x || (rect2.x + rect2.z) < rect1.x)
	{
		return false;
	}

	// If one rectangle is under the other
	if (rect1.y > (rect2.y + rect2.w) || rect2.y > (rect1.y + rect1.w))
	{
		return false;
	}

	return true;
}