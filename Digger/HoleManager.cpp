#include "MiniginPCH.h"
#include "HoleManager.h"
#include "HoleNode.h"
#include "Renderer.h"
#include "Level.h"
#include "EntityManager.h"
#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Texture2D.h"

HoleManager::HoleManager()
	: m_pGrid{}
	, m_pEntityManager{}
	, m_pLevel{}
{}

HoleManager::~HoleManager()
{
	for (int i{}; i < LevelData::Columns; ++i)
	{
		for (int j{}; j < LevelData::Rows; ++j)
		{
			delete m_pGrid[i][j];
			m_pGrid[i][j] = nullptr;
		}
	}

	m_pLevel = nullptr;
	m_pEntityManager = nullptr;
}

void HoleManager::Initialize()
{
	if (m_pGrid[0][0])
		return;

	for (int i{}; i < LevelData::Columns; ++i)
	{
		for (int j{}; j < LevelData::Rows; ++j)
		{
			const int x = i * 40 + 20 + 5;
			const int y = j * 40 + 20 + 5;
			m_pGrid[i][j] = new HoleNode{ x, y, 40 };
		}
	}

	m_pHoleTexture = GlobalMemoryPools::GetInstance().CreateTexture2D(ResourceManager::GetInstance().LoadTexture("Hole.png"));
	m_pHoleTexture->InitializeTexture();
	m_pHoleCracksTexture = GlobalMemoryPools::GetInstance().CreateTexture2D(ResourceManager::GetInstance().LoadTexture("HoleCracks.png"));
	m_pHoleCracksTexture->InitializeTexture();
	m_pHoleOpeningTexture = GlobalMemoryPools::GetInstance().CreateTexture2D(ResourceManager::GetInstance().LoadTexture("HoleOpening.png"));
	m_pHoleOpeningTexture->InitializeTexture();

	InitLevel(m_pLevel->GetCurrentLevel());
}

void HoleManager::Render() const
{
	//DEBUG
	//for (int i{}; i < LevelData::Columns; ++i)
	//{
	//	for (int j{}; j < LevelData::Rows; ++j)
	//	{
	//		const int size = m_pGrid[i][j]->Size;
	//		int x1{ m_pGrid[i][j]->x - size / 2 };
	//		int y1{ m_pGrid[i][j]->y - size / 2 };
	//		int x2{ m_pGrid[i][j]->x + size / 2 };
	//		int y2{ m_pGrid[i][j]->y - size / 2 };
	//		Renderer::GetInstance().DrawLine((float)x1, (float)y1, (float)x2, (float)y2);
	//		x1 += size;
	//		y2 += size;
	//		Renderer::GetInstance().DrawLine((float)x1, (float)y1, (float)x2, (float)y2);
	//
	//		const float x{ (float)m_pGrid[i][j]->x };
	//		const float y{ (float)m_pGrid[i][j]->y };
	//		const float size2{ (float)m_pGrid[i][j]->Size / 2.f };
	//
	//		if (m_pGrid[i][j]->IsLeftConnected)
	//			Renderer::GetInstance().DrawLine(x, y, x - size2, y, RGBAColour{255, 0, 0, 255});
	//
	//		if (m_pGrid[i][j]->IsRightConnected)
	//			Renderer::GetInstance().DrawLine(x, y, x + size2, y, RGBAColour{ 255, 0, 0, 255 });
	//
	//		if (m_pGrid[i][j]->IsTopConnected)
	//			Renderer::GetInstance().DrawLine(x, y, x, y + size2, RGBAColour{ 255, 0, 0, 255 });
	//
	//		if (m_pGrid[i][j]->IsBottomConnected)
	//			Renderer::GetInstance().DrawLine(x, y, x, y - size2, RGBAColour{ 255, 0, 0, 255 });
	//	}
	//}

	const LevelData& level = m_pLevel->GetCurrentLevel();

	//TODO: render tunnels/holes
	for (int i{}; i < LevelData::Columns; ++i)
	{
		for (int j{}; j < LevelData::Rows; ++j)
		{
			if (level.data[i][j] == LevelTypes::hole)
				DrawHole(GetCurrentNode(float(i * 40 + 25), float(j * 40 + 25)));
		}
	}
}

void HoleManager::Update()
{
	//const LevelData& level = m_pLevel->GetCurrentLevel();

	//TODO: update holesystem
	for (Player* pPlayer : m_pEntityManager->GetPlayers())
	{
		const Vector2& playerPos = pPlayer->GetGameObject()->GetTransform().GetPosition();
		SetHole(playerPos.x, playerPos.y, pPlayer->GetDirection());
	}
}

const HoleNode& HoleManager::GetCurrentNode(float x, float y) const
{
	int i = (int)x / 40;
	int j = (int)y / 40;

	if (i >= LevelData::Columns)
		i = LevelData::Columns - 1;
	else if (i < 0)
		i = 0;

	if (j >= LevelData::Rows)
		j = LevelData::Rows - 1;
	else if (j < 0)
		j = 0;

	return *m_pGrid[i][j];
}

void HoleManager::InitLevel(const LevelData& level)
{
	for (int i{}; i < LevelData::Columns; ++i)
	{
		for (int j{}; j < LevelData::Rows; ++j)
		{
			if (level.data[i][j] == LevelTypes::hole || level.data[i][j] == LevelTypes::spawner || level.data[i][j] == LevelTypes::player)
			{
				//if a hole exists, it is always part of a tunnel
				//check neighbours:
				//1: check if the hole isn't siding on the edges, they don't have neighbour(s) on that edge
				//2: check if their neighbouring tile is a hole, if so, 'connect'
				if ((i - 1 >= 0) && (level.data[i - 1][j] == LevelTypes::hole || level.data[i - 1][j] == LevelTypes::spawner || level.data[i - 1][j] == LevelTypes::player))
					m_pGrid[i][j]->IsLeftConnected = true;

				if ((i + 1 <= LevelData::Columns) && (level.data[i + 1][j] == LevelTypes::hole || level.data[i + 1][j] == LevelTypes::spawner || level.data[i + 1][j] == LevelTypes::player))
					m_pGrid[i][j]->IsRightConnected = true;

				if ((j + 1 <= LevelData::Rows) && (level.data[i][j + 1] == LevelTypes::hole || level.data[i][j + 1] == LevelTypes::spawner || level.data[i][j + 1] == LevelTypes::player))
					m_pGrid[i][j]->IsTopConnected = true;

				if ((j - 1 >= 0) && (level.data[i][j - 1] == LevelTypes::hole || level.data[i][j - 1] == LevelTypes::spawner || level.data[i][j - 1] == LevelTypes::player))
					m_pGrid[i][j]->IsBottomConnected = true;
			}
		}
	}
}

void HoleManager::SetLevel(Level* pLevel)
{
	m_pLevel = pLevel;
}

void HoleManager::SetEntityManager(EntityManager* pEntityManager)
{
	m_pEntityManager = pEntityManager;
}

void HoleManager::DrawHole(const HoleNode& node) const
{
	const Vector2& pos = m_pGameObject->GetFinalPos();
	const Vector2& scale = m_pGameObject->GetTransform().GetScale();
	const Vector4& cracksSrcRect = m_pHoleCracksTexture->GetSourceRect();
	if (!node.IsLeftConnected)
	{
		const Vector4 dstRec{ node.x - (LevelData::TileWidth/2) * scale.x, node.y * scale.y, cracksSrcRect.z, cracksSrcRect.w };
		Renderer::GetInstance().RenderTexture(m_pHoleCracksTexture->GetSDLTexture(),
			(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x),
			(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y),
			dstRec.z * scale.x, dstRec.w * scale.y,
			cracksSrcRect.x, cracksSrcRect.y, cracksSrcRect.z, cracksSrcRect.w,
			0.f,
			m_pHoleCracksTexture->GetFlip());
	}
	if (!node.IsRightConnected)
	{
		const Vector4 dstRec{ node.x + (LevelData::TileWidth / 2) * scale.x, node.y * scale.y, cracksSrcRect.z, cracksSrcRect.w };
		Renderer::GetInstance().RenderTexture(m_pHoleCracksTexture->GetSDLTexture(),
			(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x),
			(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y),
			dstRec.z * scale.x, dstRec.w * scale.y,
			cracksSrcRect.x, cracksSrcRect.y, cracksSrcRect.z, cracksSrcRect.w,
			0.f,
			m_pHoleCracksTexture->GetFlip());
	}
	if (!node.IsTopConnected)
	{
		const Vector4 dstRec{ node.x * scale.x, node.y + (LevelData::TileHeight / 2) * scale.y, cracksSrcRect.z, cracksSrcRect.w };
		Renderer::GetInstance().RenderTexture(m_pHoleCracksTexture->GetSDLTexture(),
			(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x),
			(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y),
			dstRec.z * scale.x, dstRec.w * scale.y,
			cracksSrcRect.x, cracksSrcRect.y, cracksSrcRect.z, cracksSrcRect.w,
			90.f,
			m_pHoleCracksTexture->GetFlip());
	}
	if (!node.IsBottomConnected)
	{
		const Vector4 dstRec{ node.x * scale.x, node.y - (LevelData::TileHeight / 2) * scale.y, cracksSrcRect.z, cracksSrcRect.w };
		Renderer::GetInstance().RenderTexture(m_pHoleCracksTexture->GetSDLTexture(),
			(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x),
			(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y),
			dstRec.z * scale.x, dstRec.w * scale.y,
			cracksSrcRect.x, cracksSrcRect.y, cracksSrcRect.z, cracksSrcRect.w,
			90.f,
			m_pHoleCracksTexture->GetFlip());
	}

	const Vector4& srcRect = m_pHoleTexture->GetSourceRect();
	const Vector4 holeDstRect{ (float)node.x, (float)node.y, srcRect.z, srcRect.w };
	Renderer::GetInstance().RenderTexture(m_pHoleTexture->GetSDLTexture(),
		(pos.x + holeDstRect.x) - (holeDstRect.z / 2 * scale.x),
		(pos.y + holeDstRect.y) + (holeDstRect.w / 2 * scale.y),
		holeDstRect.z * scale.x, holeDstRect.w * scale.y,
		srcRect.x, srcRect.y, srcRect.z, srcRect.w,
		0.f,
		m_pHoleTexture->GetFlip());
}

void HoleManager::SetHole(float x, float y, const Vector2& dir)
{
	//const LevelData& level = m_pLevel->GetCurrentLevel();

	int i = (int)x / 40;
	int j = (int)y / 40;

	if (i >= LevelData::Columns)
		i = LevelData::Columns - 1;
	else if (i < 0)
		i = 0;

	if (j >= LevelData::Rows)
		j = LevelData::Rows - 1;
	else if (j < 0)
		j = 0;

	//if (level.data[i][j] == LevelTypes::hole)
	//	return;

	//x
	if (dir.x != 0.f)
	{
		m_pGrid[i][j]->IsRightConnected = true;
		m_pGrid[i][j]->IsLeftConnected = true;
	}

	//y
	if (dir.y != 0.f)
	{
		m_pGrid[i][j]->IsTopConnected = true;
		m_pGrid[i][j]->IsBottomConnected = true;
	}
}