#include"bullet.h"
#include"st.h"
void Bullet::init()
{
	Object::init();
	m_hResource = st::call()->bm.m_phResource;
	this->SetActive(false);
	m_fZ = 122;
}

void Bullet::update(float dt)
{
	CGPoint pos = GetPostition();

	if(st::call()->pNowRunningScene->PointExitCheck(GetPostition()) )
	{
		SetActive(false);
		return;
	}

	if(GetDirection() == RIGHT)
	{
		pos.x += BULLET_SPEED * dt;
	}
	else if(GetDirection() == LEFT)
	{
		pos.x -= BULLET_SPEED * dt;
	}
	
	Object * obj = st::call()->em.CollisionCheck(pos, 40);
	if(obj != NULL)
	{
		obj->Hit(this->GetDirection() , NORMAL_SHOT);
		SetActive(false); //ÃÑ¾ËÀÇ »óÅÂ
	}
	SetPosition(pos);
}

void Bullet::render()
{
	if(GetActive() == false) return;

	CGPoint pos = GetPostition();
	HDC b = st::call()->backDC;
	HDC m = st::call()->memDC;

	pos.y -= m_fZ;
	SelectObject(m, m_hResource);

	BitBlt(b, pos.x, pos.y, 18, 6 , m, 0 ,6, SRCAND);
	BitBlt(b, pos.x, pos.y, 18, 6 , m, 0 ,0, SRCPAINT);
}

void Bullet::dealloc()
{
	if(m_hResource != NULL)
	{
		DeleteObject(m_hResource);
		m_hResource = NULL;
	}
}