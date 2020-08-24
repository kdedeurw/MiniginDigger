#include "MiniginPCH.h"
#include "Level.h"
#include "Texture2D.h"
#include "GlobalInput.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "GameState.h"
#include "EntityManager.h"
#include "Player.h"
#include "HoleManager.h"

Level::Level()
	: m_pLevelsTexture{}
	, m_LevelId{}
	, m_CurrentLevel{}
{}

Level::~Level()
{
	m_pLevelsTexture = nullptr;
}

void Level::Initialize()
{
	m_pLevelsTexture = GlobalMemoryPools::GetInstance().CreateTexture2D(ResourceManager::GetInstance().LoadTexture("LevelTiles.jpg"));
	m_pLevelsTexture->InitializeTexture();

	Vector4& dstRec = m_pLevelsTexture->GetDestRect();
	Vector4& srcRect = m_pLevelsTexture->GetSourceRect();
	srcRect.z = dstRec.z = (float)LevelData::TileWidth;
	srcRect.w = dstRec.w = (float)LevelData::TileHeight;

	srcRect.x = float((m_LevelId % 2) * LevelData::TileWidth);
	srcRect.y = float((m_LevelId / 2) * LevelData::TileHeight);

	m_Parser.Parse();
	m_CurrentLevel = m_Parser.GetLevels()[m_LevelId]; //copy level data
}

void Level::Render() const
{
	const Vector2& pos = m_pGameObject->GetFinalPos();
	const Vector2& scale = m_pGameObject->GetTransform().GetScale();
	const Vector4& srcRect = m_pLevelsTexture->GetSourceRect();
	for (int i{}; i < LevelData::Columns; ++i)
	{
		for (int j{}; j < LevelData::Rows * 2; ++j)
		{
			const Vector4 dstRec{ i * LevelData::TileWidth * scale.x, j * LevelData::TileHeight * scale.y, srcRect.z, srcRect.w };
			if (m_CurrentLevel.data[i][j / 2] == LevelTypes::tile || 
				m_CurrentLevel.data[i][j / 2] == LevelTypes::gem || 
				m_CurrentLevel.data[i][j / 2] == LevelTypes::goldbag)
			{
				Renderer::GetInstance().RenderTexture(m_pLevelsTexture->GetSDLTexture(),
					(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x),
					(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y),
					dstRec.z * scale.x, dstRec.w * scale.y,
					srcRect.x, srcRect.y, srcRect.z, srcRect.w,
					m_pGameObject->GetTransform().GetRotation(),
					m_pLevelsTexture->GetFlip());
			}
		}
	}
}

void Level::Update()
{
	for (Player* pPlayer : m_pEntityManager->GetPlayers())
	{
		SetHole(pPlayer->GetGameObject()->GetTransform().GetPosition());
	}
}

void Level::SetFile(const std::string& file)
{
	m_Parser.SetFile(ResourceManager::GetInstance().GetDataPath() + file);
}

const LevelData& Level::GetCurrentLevel() const
{
	return m_CurrentLevel;
}

void Level::SetEntityManager(EntityManager* pEntityManager)
{
	m_pEntityManager = pEntityManager;
}

void Level::Reset()
{
	//TODO
}

void Level::NextLevel()
{
	++m_LevelId;
	if (m_LevelId >= (int)m_Parser.GetLevels().size())
		m_LevelId = 0; //endgame?

	m_CurrentLevel = m_Parser.GetLevels()[m_LevelId]; //copy/overwrite new level data

	Vector4& srcRect = m_pLevelsTexture->GetSourceRect();
	srcRect.x = float((m_LevelId % 2) * LevelData::TileWidth);
	srcRect.y = float((m_LevelId / 2) * LevelData::TileHeight);

	m_pEntityManager->InitLevel();

	m_pHoleManager->InitLevel(m_CurrentLevel);
}

void Level::SetHole(const Vector2& pos)
{
	//tile width = 20.f * 15 = 300.f size
	//tile height = 20.f * 10 = 150.f size

	const int x = (int)(pos.x - 20.f) / (int)(40);
	const int y = (int)(pos.y - 20.f) / (int)(40);

	m_CurrentLevel.data[x][y] = LevelTypes::hole;
}