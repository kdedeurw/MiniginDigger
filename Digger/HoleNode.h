#pragma once
struct HoleNode
{
public:
	HoleNode(int x, int y, int size);
	~HoleNode();

	//TODO: bitmask?
	bool IsLeftConnected;
	bool IsRightConnected;
	bool IsTopConnected;
	bool IsBottomConnected;

	int x, y;
	int Size;
};