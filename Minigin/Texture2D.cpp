#include "MiniginPCH.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include <SDL.h>

Texture2D::Texture2D(SDL_Texture* pTexture)
	: Texture{ pTexture }
	, m_Flip{ RenderFlip::None }
	//chose not to initialize other member variables since they need to be forcefully set in Init
{}

Texture2D::~Texture2D()
{};

const Vector2& Texture2D::GetDimensions() const
{
	return m_Dimensions;
}

bool Texture2D::InitializeTexture()
{
	int width, height;
	if (!SDL_QueryTexture(m_pTexture, nullptr, nullptr, &width, &height))
	{
		m_Dimensions.x = (float)width;
		m_Dimensions.y = (float)height;
		SetSourceRect(Vector4{ 0.f, 0.f, m_Dimensions.x, m_Dimensions.y });
		SetDestRect(Vector4{ 0.f, 0.f, m_Dimensions.x, m_Dimensions.y });
		return true;
	}
	return false;
}

void Texture2D::SetFlip(RenderFlip flip)
{
	m_Flip = flip;
}

void Texture2D::SetTexture(SDL_Texture* pTexture)
{
	m_pTexture = pTexture;
	InitializeTexture();
}

void Texture2D::SetTexture(const std::string& fileAsset)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileAsset);
	InitializeTexture();
}

void Texture2D::SetSourceRect(Vector4 srcRect)
{
	m_SourceRect = std::move(srcRect);//std::move accepts rvalue, so we move copied srcRect in m_SourceRect
}

void Texture2D::SetDestRect(Vector4 dstRect)
{
	m_DestRect = std::move(dstRect);//std::move accepts rvalue, so we move copied srcRect in m_DestRect
}

const RenderFlip Texture2D::GetFlip() const
{
	return m_Flip;
}

Vector4& Texture2D::GetSourceRect()
{
	return m_SourceRect;
}

Vector4& Texture2D::GetDestRect()
{
	return m_DestRect;
}