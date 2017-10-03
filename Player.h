#pragma once
#include"Object.h"

class Player : public Object
{
private:
	CGPoint			m_srcPos;
	CGPoint			m_srcSize;
	CGPoint			m_offSet;		//그림 그릴 위치 보정
	int				m_iFrame;
	float			m_fFrameDelay;
	float			m_fWalkSoundDelay;
	STATE			m_iPrevState;
	STATE			m_iState;

	bool			m_bFired;
	bool			m_bReFired;
	bool			m_bKicked;
	bool			m_bKickOnce;
	bool			m_bHitOn;
	CGPoint			m_JumpPos;

	float			m_fMoveSpeed;
	float			m_fVelocity;
	int				m_iHp;
public:
	static Object* Create();
	virtual void init();
	virtual void dealloc();
	virtual void render();
	virtual void update(float dt);
	void Animaion(float dt);
	void Action(float dt);
	void Hit(DIRECTION dir, int attackType);
};