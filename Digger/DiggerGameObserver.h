#pragma once
#include <Observer.h>

class DiggerGameObserver final : public Observer
{
public:
	DiggerGameObserver();
	~DiggerGameObserver() = default;

	void OnNotify(GameObject* pGameObject, int event) override;

	unsigned int GetScore() const;
	unsigned int GetLives() const;
	unsigned int GetGems() const;

private:
	unsigned int m_Score, m_Lives, m_Gems;
	unsigned int m_EnemiesAlive, m_NobbinsKilled, m_HobbinsKilled;
};