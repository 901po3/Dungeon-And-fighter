#include"Effect.h"
#include"st.h"

void Effect::init()
{
	Object::init();
	m_iFrame			= 0;
	m_fFrameDelay		= 0.f;
	SetActive(false);
	SetTarget( st::call()->pNowRunningScene->GetObjectByTag(PLAYER));
}

void Effect::render()
{
	if(GetActive() == false) return;
	
	CGPoint pos = GetPostition();
	HDC b = st::call()->backDC;
	HDC m = st::call()->memDC;
	
	if(m_type == BLOOD)
		m_fZ = 150;
	if(m_type == EFFECT)
		m_fZ = 180;

	pos.y -= m_fZ;
	SelectObject(m, ei.resource);

	float srcPosX = 0;
	if(GetDirection() == RIGHT)
	{
		srcPosX = ei.RightDirectionOrigin;
		if(m_type == BLOOD)
			pos.x -= 10;
		if(m_type == EFFECT)
			pos.x -= 30;
	}
	else if(GetDirection() == LEFT)
	{
		if(m_type == BLOOD)
			pos.x -= 35;
		if(m_type == EFFECT)
			pos.x -= 95;
	}

	BitBlt(b, pos.x, pos.y, 
		ei.srcSize.x, ei.srcSize.y 
		, m,
		srcPosX + m_iFrame * ei.srcSize.x ,ei.srcSize.y, 
		SRCAND);

	BitBlt(b, pos.x, pos.y, 
		ei.srcSize.x, ei.srcSize.y 
		, m,
		srcPosX + m_iFrame * ei.srcSize.x ,0,
		SRCPAINT);
}

void Effect::update(float dt)
{
	m_fFrameDelay += dt;
	if(m_fFrameDelay > ei.delay) 
	{
		m_fFrameDelay = 0;
		m_iFrame++;

		if(m_iFrame >= ei.maxframe)
		{ 
			m_fFrameDelay		= 0;
			m_iFrame			= 0;
			SetActive(false);
		}
	}
}

void Effect::SetEffectInfo(EffectInfo _ei)
{
	ei.delay					= _ei.delay;
	ei.maxframe				= _ei.maxframe;
	ei.resource				= _ei.resource;
	ei.RightDirectionOrigin	= _ei.RightDirectionOrigin;
	ei.srcSize				= _ei.srcSize;
}


void Effect::dealloc()
{

}
