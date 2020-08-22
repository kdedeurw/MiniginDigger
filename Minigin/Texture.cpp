#include "MiniginPCH.h"
#include "Texture.h"
#include <SDL.h>

Texture::Texture(SDL_Texture* pTexture)
	: m_pTexture{ pTexture }
{}

Texture::~Texture()
{
	SDL_DestroyTexture(m_pTexture);
	m_pTexture = nullptr;
};