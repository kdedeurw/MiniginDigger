#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "GameObject.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Math.h"
#include "GameState.h"
#include "GlobalMemoryPools.h"

FPSComponent::FPSComponent()
	: m_FpsInterval{}
	, m_CurrentFps{}
	, m_pFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 24) }
	, m_Text{ "" }
	, m_ElapsedSec{}
	, m_NeedsUpdate{ true } //do not set this to false to start off, bc the Render function will crash bc invalid pTextureData
	, m_pTextureData{ GlobalMemoryPools::GetInstance().CreateTexture2D(nullptr) } //get memory address, this will be overwritten at will
{
	ResetFPS();
}

FPSComponent::~FPSComponent()
{
	m_pFont = nullptr;
	delete m_pTextureData;
	m_pTextureData = nullptr;
}

void FPSComponent::Update()
{
	++m_FpsInterval;

	GameState& gs = GameState::GetInstance();
	m_ElapsedSec += gs.DeltaTime;
	if (m_ElapsedSec >= 1.f)
	{
		ResetFPS();
		m_ElapsedSec = 0.f;
	}

	if (m_NeedsUpdate)
	{
		SDL_DestroyTexture(m_pTextureData->GetSDLTexture()); //destroy old texture before losing pointer
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
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
		m_pTextureData = GetTextureData(pTexture);
		m_NeedsUpdate = false;
	}
}

void FPSComponent::Render() const
{
	const Vector2 pos = m_pGameObject->GetFinalPos();
	const float rot = m_pGameObject->GetTransform().GetRotation();
	if (m_pTextureData != nullptr)
	{
		Renderer::GetInstance().RenderTexture(m_pTextureData->GetSDLTexture(), pos.x, pos.y, rot);
	}
}

void FPSComponent::SetFont(Font* pFont)
{
	m_pFont = pFont;
	m_NeedsUpdate = true;
}

unsigned int FPSComponent::GetFPS() const
{
	return m_CurrentFps;
}

void FPSComponent::ResetFPS()
{
	std::cout << m_FpsInterval << '\n';
	m_CurrentFps = m_FpsInterval;
	m_Text = std::move("FPS " + std::to_string(m_FpsInterval));
	m_FpsInterval = 0;
	m_NeedsUpdate = true;
}

Texture2D* FPSComponent::GetTextureData(SDL_Texture* pTexture)
{
	return reinterpret_cast<Texture2D*>(new (m_pTextureData) Texture2D{ pTexture });
}