#include "MiniginPCH.h"
#include "HoleNode.h"

HoleNode::HoleNode(int x, int y, int size)
	: IsLeftConnected{}
	, IsRightConnected{}
	, IsTopConnected{}
	, IsBottomConnected{}
	, x{x}
	, y{y}
	, Size{size}
{}

HoleNode::~HoleNode()
{
}