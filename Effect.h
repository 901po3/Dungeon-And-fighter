#pragma once
#include"Object.h"

class Effect : public Object
{
	int			m_iFrame;
	float		m_fFrameDelay;
	EffectInfo	ei; 
	EffectType	m_type;
public:
	static Effect *Create()
	{
		Effect *effect = new Effect;
		effect->init();
		return effect;
	}
	void SetType(EffectType type)	{ m_type = type;}
	EffectType GetType()			{ return m_type;}
	void init();
	void dealloc();
	void render();
	void update(float dt);
	void SetEffectInfo(EffectInfo _ei);
};