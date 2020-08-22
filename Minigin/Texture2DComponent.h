#pragma once
#include "Component.h"
#include "Texture2D.h"

//would possibly use multiple inheritance with Texture2D, but restrained from it
class Texture2DComponent : public Component
{
public:
	Texture2DComponent();
	~Texture2DComponent();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	Texture2D* GetTexture() const { return m_pTexture; };

	void SetTexture(SDL_Texture* pTexture);
	void SetTexture(const std::string& fileAsset);
	void SetSourceRect(Vector4 srcRect) { m_pTexture->SetSourceRect(srcRect); };
	void SetDestRect(Vector4 dstRect) { m_pTexture->SetDestRect(dstRect); };

	SDL_Texture* GetSDLTexture() const { return m_pTexture->GetSDLTexture(); };
	Vector4& GetSourceRect() { return m_pTexture->GetSourceRect(); };
	Vector4& GetDestRect() { return m_pTexture->GetDestRect(); };
	const Vector2& GetDimensions() const { return m_pTexture->GetDimensions(); };

protected:
	Texture2D* m_pTexture;

};