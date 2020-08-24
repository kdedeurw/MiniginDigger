#pragma once
#include <Component.h>
#include "Vectors.h"

class DiggerGameObserver;
class Texture2D;
class GameManager final : public Component
{
public:
	void Initialize() override;
	void Render() const override;
	void Update() override;

	void SetDiggerGameObserver(DiggerGameObserver* pDiggerGameObserver);

private:
	Texture2D* m_pAlphabetNumbersTexture, * m_pLifeTexture;
	DiggerGameObserver* m_pDiggerObserver;

	void GameOver();
};