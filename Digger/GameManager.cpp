#include "MiniginPCH.h"
#include "GameManager.h"
#include "DiggerGameObserver.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GlobalMemoryPools.h"
#include "Texture2D.h"

void GameManager::Initialize()
{
	m_pAlphabetNumbersTexture = GlobalMemoryPools::GetInstance().CreateTexture2D(ResourceManager::GetInstance().LoadTexture("AlphabetNumbers.jpg"));
	m_pAlphabetNumbersTexture->InitializeTexture();
	m_pAlphabetNumbersTexture->SetSourceRect(Vector4{ 0.f, 0.f, 12.f, 15.f });

	m_pLifeTexture = GlobalMemoryPools::GetInstance().CreateTexture2D(ResourceManager::GetInstance().LoadTexture("Life.png"));
	m_pLifeTexture->InitializeTexture();
}

void GameManager::Render() const
{
	const unsigned int score = m_pDiggerObserver->GetScore();

	const Vector2& pos = m_pGameObject->GetFinalPos();
	const Vector2& scale = m_pGameObject->GetTransform().GetScale();
	Vector4 srcRect{ m_pAlphabetNumbersTexture->GetSourceRect() };
	srcRect.y = 45.f; //numbers section

	//draw score
	int offset{};
	for (int five{ 10000 }; five > 0; five /= 10)
	{
		srcRect.x = float(score % five);
		//draw tenthousands
		const Vector4 dstRec{ offset * srcRect.w, 0.f, srcRect.z, srcRect.w };
		Renderer::GetInstance().RenderTexture(m_pAlphabetNumbersTexture->GetSDLTexture(),
			(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x),
			(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y),
			dstRec.z * scale.x, dstRec.w * scale.y,
			srcRect.x, srcRect.y, srcRect.z, srcRect.w,
			m_pGameObject->GetTransform().GetRotation(),
			m_pAlphabetNumbersTexture->GetFlip());

		++offset;
	}

	//draw lives
	Vector4 srcRect2{ m_pLifeTexture->GetSourceRect() };
	for (unsigned int lives{}; lives < m_pDiggerObserver->GetLives(); ++lives)
	{
		const Vector4 dstRec{ 100.f + lives * srcRect2.w * 2.5f, 0.f, srcRect2.z, srcRect2.w };
		Renderer::GetInstance().RenderTexture(m_pLifeTexture->GetSDLTexture(),
			(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x),
			(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y),
			dstRec.z * scale.x, dstRec.w * scale.y,
			srcRect2.x, srcRect2.y, srcRect2.z, srcRect2.w,
			m_pGameObject->GetTransform().GetRotation(),
			m_pLifeTexture->GetFlip());
	}
}

void GameManager::Update()
{
	if (m_pDiggerObserver->GetLives() <= 0)
		GameOver();
}

void GameManager::SetDiggerGameObserver(DiggerGameObserver* pDiggerGameObserver)
{
	m_pDiggerObserver = pDiggerGameObserver;
}

void GameManager::GameOver()
{
	std::cout << "\n!Game Over!\n";

	//TODO: trigger level, entitymanager and holemanager
}