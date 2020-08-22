#pragma once

struct SDL_Texture;
class Texture
{
public:
	explicit Texture(SDL_Texture* pTexture = nullptr);
	virtual ~Texture();
	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(const Texture&&) = delete;

	SDL_Texture* GetSDLTexture() const { return m_pTexture; };

protected:
	SDL_Texture* m_pTexture;
};