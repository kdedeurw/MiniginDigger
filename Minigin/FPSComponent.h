#pragma once
#include "Component.h"

class Font;
class Texture2D;
struct SDL_Texture;
class FPSComponent : public Component
{
public:
	FPSComponent();
	~FPSComponent();
	FPSComponent(const FPSComponent& other) = delete;
	FPSComponent(FPSComponent&& other) = delete;
	FPSComponent& operator=(const FPSComponent& other) = delete;
	FPSComponent& operator=(FPSComponent&& other) = delete;

	void Initialize() override {};
	void Update() override;
	void Render() const override;

	void SetFont(Font* pFont);
	unsigned int GetFPS() const;

private:
	bool m_NeedsUpdate;
	float m_ElapsedSec;
	unsigned int m_FpsInterval, m_CurrentFps;
	Font* m_pFont;
	Texture2D* m_pTextureData;
	std::string m_Text;

	void ResetFPS();
	Texture2D* GetTextureData(SDL_Texture* pTexture);
};