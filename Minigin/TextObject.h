#pragma once
#include "SceneObject.h"
#include "Transform.h"
#include "MiniMath.h"

class Font;
class Texture;
struct SDL_Texture;
class TextObject final : public SceneObject
{
public:
	explicit TextObject(Transform& transform, const std::string& text, Font* pFont);
	virtual ~TextObject();
	TextObject(const TextObject& other) = delete;
	TextObject(TextObject&& other) = delete;
	TextObject& operator=(const TextObject& other) = delete;
	TextObject& operator=(TextObject&& other) = delete;

	void Initialize() override {};
	void Update() override;
	void Render() const override;
	void DrawDebug() const override {};

	//void SetText(std::string text);
	void SetText(const std::string& text);
	void SetColour(RGBAColour colour);

private:
	bool m_NeedsUpdate;
	Font* m_pFont;
	Texture* m_pTextureData;
	RGBAColour m_Colour;
	std::string m_Text;

	Texture* GetTextureData(SDL_Texture* pTexture);
};