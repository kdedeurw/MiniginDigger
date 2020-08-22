#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Font.h"
#include "GameState.h"
#include "WindowInfo.h"

void Renderer::Init(SDL_Window * window)
{
	m_pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Renderer::Render() const
{
	SDL_RenderClear(m_pRenderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_pRenderer);
}

void Renderer::Destroy()
{
	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

void Renderer::DrawDebug(const Vector2& pos, unsigned int id) const
{
	DrawPoint(pos);
	SDL_Surface* pSurface = TTF_RenderText_Blended(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24)->GetFont(), std::to_string(id).c_str(),
		SDL_Color{ 255,255,255,255 });
	if (pSurface == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), pSurface);
	if (pTexture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(pSurface);
	RenderTexture(pTexture, pos);
	SDL_DestroyTexture(pTexture);
}

void Renderer::DrawPoint(float x, float y, RGBAColour colour) const
{
	SDL_SetRenderDrawColor(m_pRenderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderDrawPoint(m_pRenderer, std::move((int)x), std::move(GameState::GetInstance().pWindowInfo->Height - (int)y));
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 1);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, RGBAColour colour)
{
	SDL_SetRenderDrawColor(m_pRenderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderDrawLine(m_pRenderer, std::move((int)x1), std::move(GameState::GetInstance().pWindowInfo->Height - (int)y1), std::move((int)x2), std::move(GameState::GetInstance().pWindowInfo->Height - (int)y2));
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 1);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float angle, RenderFlip flip) const
{
	int w, h;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);
	RenderTexture(pTexture, std::move(x), std::move(y), std::move((float)w), std::move((float)h), std::move(angle), flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float angle, RenderFlip flip) const
{
	SDL_Rect dst;
	dst.x = std::move((int)x);
	dst.y = std::move(GameState::GetInstance().pWindowInfo->Height - (int)y);
	dst.w = std::move((int)width);
	dst.h = std::move((int)height);
	SDL_RenderCopyEx(m_pRenderer, pTexture, nullptr, &dst, angle, nullptr, (SDL_RendererFlip)flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float srcX, float srcY, float angle, RenderFlip flip) const
{
	int w, h;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);
	RenderTexture(pTexture, std::move(x), std::move(y), std::move(width), std::move(height), std::move(srcX), std::move(srcY), std::move((float)w), std::move((float)h), std::move(angle), flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float srcX, float srcY, float srcW, float srcH, float angle, RenderFlip flip) const
{
	SDL_Rect dst;
	dst.x = std::move((int)x);
	dst.y = std::move(GameState::GetInstance().pWindowInfo->Height - (int)y);
	dst.w = std::move((int)width);
	dst.h = std::move((int)height);
	SDL_Rect src;
	src.x = std::move((int)srcX);
	src.y = std::move((int)srcY);
	src.w = std::move((int)srcW);
	src.h = std::move((int)srcH);
	SDL_RenderCopyEx(m_pRenderer, pTexture, &src, &dst, angle, nullptr, (SDL_RendererFlip)flip);
}