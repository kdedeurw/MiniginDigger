#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "GameState.h"

SpriteComponent::SpriteComponent()
	: m_PlayOnce{}
	, m_Frame{}
	, m_MaxFrames{}
	, m_Tick{}
	, m_TickRate{}
	, m_YOffset{}
	, m_XOffset{}
{}

SpriteComponent::~SpriteComponent()
{}

void SpriteComponent::Initialize()
{}

//void SpriteComponent::Render() const
//{
//}

void SpriteComponent::Update()
{
	m_Tick += GameState::GetInstance().DeltaTime;
	if (m_PlayOnce)
	{
		if (m_Frame >= m_MaxFrames)
			return;
	}
	if (m_Tick >= m_TickRate)
	{
		++m_Frame;
		if (m_Frame >= m_MaxFrames)
			m_Frame = 0;
		m_Tick = 0;
		
		Vector4& srcRect = m_pTexture->GetSourceRect();
		srcRect.x = srcRect.z * m_Frame + m_XOffset;
	}
}

void SpriteComponent::SetPlayOnce(bool enable)
{
	m_PlayOnce = enable;
}

void SpriteComponent::SetFrame(int frame)
{
	m_Frame = frame;
}

void SpriteComponent::SetStateMultiplier(int state)
{
	Vector4& srcRect = m_pTexture->GetSourceRect();
	srcRect.y = srcRect.w * state + m_YOffset;
}

void SpriteComponent::SetMaxFrames(int max)
{
	m_MaxFrames = max;
}

void SpriteComponent::SetXSize(float xSize)
{
	m_pTexture->GetSourceRect().z = xSize;
	m_pTexture->GetDestRect().z = xSize;
}

void SpriteComponent::SetYSize(float ySize)
{
	m_pTexture->GetSourceRect().w = ySize;
	m_pTexture->GetDestRect().w = ySize;
}

void SpriteComponent::SetSizes(const Vector2& sizes)
{
	SetXSize(sizes.x);
	SetYSize(sizes.y);
}

void SpriteComponent::SetTickRate(float tickRate)
{
	m_TickRate = tickRate;
}

void SpriteComponent::SetYOffset(float offset)
{
	m_YOffset = offset;
}

void SpriteComponent::SetXOffset(float offset)
{
	m_XOffset = offset;
}

void SpriteComponent::Reset()
{
	m_Frame = 0;
	m_Tick = 0;
}