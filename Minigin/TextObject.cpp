#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GlobalMemoryPools.h"

TextObject::TextObject(Transform& transform, const std::string& text, Font* pFont)
	: SceneObject{ transform }
	, m_NeedsUpdate{ true } //do not set this to false to start off, bc the Render function will crash bc invalid pTextureData (nullptr)
	, m_Text{ text }
	, m_pFont{ pFont }
	, m_pTextureData{ GlobalMemoryPools::GetInstance().CreateTexture2D(nullptr) } //get memory address, this will be overwritten at will
	, m_Colour{}
{}

TextObject::~TextObject()
{
	m_pFont = nullptr;
	delete m_pTextureData;
	m_pTextureData = nullptr;
}

void TextObject::Update()
{
	if (m_NeedsUpdate)
	{
		SDL_DestroyTexture(m_pTextureData->GetSDLTexture()); //destroy old texture before losing pointer
		SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), 
			SDL_Color{ m_Colour.r, m_Colour.g, m_Colour.b, m_Colour.a });
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

void TextObject::Render() const
{
	if (m_pTextureData != nullptr)
	{
		const Vector3& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(m_pTextureData->GetSDLTexture(), pos.x, pos.y);
	}
}

//void TextObject::SetText(std::string text)
//{
//	m_Text = std::move(text);
//	m_NeedsUpdate = true;
//}

// This implementation uses the "dirty flag" pattern
void TextObject::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void TextObject::SetColour(RGBAColour colour)
{
	m_Colour = std::move(colour);
}

Texture* TextObject::GetTextureData(SDL_Texture* pTexture)
{
	return reinterpret_cast<Texture*>(new (m_pTextureData) Texture{ pTexture });
	//will only call Texture's constructor, but use Texture2D's allocator
	//TODO: fix small wastage of 8 bytes per object
}