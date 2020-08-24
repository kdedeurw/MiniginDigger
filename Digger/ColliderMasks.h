#pragma once

enum ColliderMask : unsigned int
{
	NONE = 0x0000U,
	PLAYER = 0x0001U,
	GEM = 0x0002U,
	ENEMY = 0x0004U,
	PROJECTILE = 0x0008U,
};