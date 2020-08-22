#pragma once
#include "Texture2DComponent.h"

class SpriteComponent : public Texture2DComponent
{
public:
	SpriteComponent();
	virtual ~SpriteComponent();

	virtual void Initialize() override;
	//virtual void Render() const override;
	virtual void Update() override;

	void SetPlayOnce(bool enable);
	void SetFrame(int frame);
	void SetStateMultiplier(int state);
	void SetMaxFrames(int max);
	//set width of 1 frame (in pixels) -> srcRect.x
	void SetXSize(float xSize);
	//set height of 1 frame (in pixels) - srcRect.y
	void SetYSize(float ySize);
	void SetSizes(const Vector2& sizes);
	void SetTickRate(float tickRate);
	void SetYOffset(float offset);
	void SetXOffset(float offset);

	void Reset();

protected:
	bool m_PlayOnce;
	int m_Frame;
	int m_MaxFrames;
	float m_Tick;
	float m_TickRate;
	float m_YOffset;
	float m_XOffset;
};

