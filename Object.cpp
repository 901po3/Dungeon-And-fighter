#include"Object.h"

void Object::init()
{
	m_bMoveHold		= false;
	m_fZ			= 0;
	position		= MakePoint(0.f, 0.f);
	direction		= RIGHT;
	m_Active		= true;
	m_iTag			= NOTHING;
	m_Target		= NULL;
}