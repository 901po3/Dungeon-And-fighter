#pragma once
#include"Object.h"

class Enemy : public Object
{
private:
	CGPoint		m_srcPos;
	CGPoint		m_srcSize;
	CGPoint		m_offSet;
	int			m_iFrame;
	float		m_fTime;
	float		m_fFrameDelay;
	STATE		m_iPrevState;
	STATE		m_iState;
	int			m_iUpDownSpeed;
	int			m_iMoveSpeed;
	int			m_iHp;
	int			m_MaxHp;
	CGPoint		m_DestinationPoint;
	CGPoint		m_HalfDestinationPoint;
	bool			m_PointOk;
	bool			m_bHitOn;
	bool			m_bAttacked;
	float		m_iAttackRange;
	HBITMAP		m_hShadow; 
	CGPoint		m_ShadowPos;
	bool			m_Rage;
	float		m_RageImageSize;
	float		m_RageModeTime;
public:
	static Enemy* Create();
	virtual void init();
	virtual void dealloc();
	virtual void render();
	virtual void update(float dt);
	void Animaion(float dt);
	void Action(float dt);

	void initPosition(CGPoint pos);

	void Hit(DIRECTION dir, int attackType);
	void DebugDraw();
};