#pragma once
#include "Texture.h"
#include "MiniMath.h"
#include "RenderFlip.h"

struct SDL_Texture;
class Texture2D final : public Texture
{
public:
	//friend class Texture2DComponent;

	explicit Texture2D(SDL_Texture* pTexture = nullptr);
	virtual ~Texture2D();
	Texture2D(const Texture2D &) = delete;
	Texture2D(Texture2D &&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&&) = delete;

	bool InitializeTexture();

	void SetFlip(RenderFlip flip);
	//easily switch textures
	void SetTexture(SDL_Texture* pTexture);
	//load in new texture (new textures take time to load)
	void SetTexture(const std::string& fileAsset);
	//the rectangle from where the renderer should sample the texture from (entire texture dimensions by default)
	//x being X sample offset, y being Y sample offset, z being Width (default texture width), w being Height (default texture height)
	//aka Sample Destination quad on texture
	void SetSourceRect(Vector4 srcRect);
	//the rectangle in 2D world space where the renderer should draw the texture (entire texture dimensions by default)
	//x being X pos offset, y being Y pos offset, z being Width (default source width), w being Height (default source height)
	//aka Render Destination quad in world space
	void SetDestRect(Vector4 dstRect);

	const RenderFlip GetFlip() const;
	Vector4& GetSourceRect();
	Vector4& GetDestRect();
	const Vector2& GetDimensions() const;

protected:
	RenderFlip m_Flip;
	Vector2 m_Dimensions;
	Vector4 m_SourceRect;
	Vector4 m_DestRect;

private:
};