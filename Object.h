#pragma once
#include"Header.h"

class Object
{
protected:
	DIRECTION	direction;
	HBITMAP		m_hResource;
	CGPoint		position;
	float		m_fZ;
	bool		m_bMoveHold;
	bool		m_Active;
	Object		*m_Target;
	Tag			m_iTag;
public:

	virtual void init();
	virtual void dealloc() {}
	virtual void render() {};
	virtual void update(float dt) {};

	void SetPosition(CGPoint _pos)		{ position = _pos;}
	CGPoint GetPostition()				{return position;}
	void SetDirection(DIRECTION dir)	{direction = dir;}
	DIRECTION GetDirection()			{return direction;}
	void SetZ(float z)					{m_fZ = z;}
	float GetZ()						{return m_fZ;}
	void SetActive(bool active)			{m_Active = active;}
	bool GetActive()					{return m_Active;}
	void SetTag(Tag tag)				{m_iTag = tag;}
	int	GetTag()						{return m_iTag;}
	void SetTarget(Object *target)		{m_Target = target;}
	Object* GetTarget()				{return m_Target;}

	virtual void Hit(DIRECTION dir, int attackType) {}
};
