#include"Enemy.h"
#include"st.h"

Enemy* Enemy::Create()
{
	Enemy *enemy = new Enemy;
	enemy->init();
	return enemy;
}

void Enemy::init()
{
	Object::init();
	position					= MakePoint(0.f, 0.f);
	m_hResource					= st::call()->em.m_phResource;
	m_hShadow					= st::call()->em.m_pHShadow;
	m_iHp						= 50;
	m_MaxHp						= 50;
	direction					= LEFT;
	m_srcPos					= MakePoint(0.f, 0.f);
	m_srcSize					= MakePoint(0.f, 0.f);
	m_bMoveHold					= false;
	m_iFrame					= 0;
	m_iState					= IDLE;
	m_iPrevState				= IDLE;
	m_fTime						= 0.f;
	SetZ(HeightGap);
	m_fFrameDelay				= 0.f;
	m_iUpDownSpeed				= RandomRange(100, 200);
	m_iMoveSpeed				= 200;
	m_DestinationPoint			= MakePoint(0.f, 0.f);
	m_HalfDestinationPoint		= MakePoint(0.f, 0.f);
	m_PointOk					= false;
	m_iState					= IDLE;
	m_Active					= false;
	m_bHitOn					= false;
	m_Rage						= false;
	m_RageModeTime				= false;
	m_bAttacked					= false;
	m_iAttackRange				= 60.f;
	m_RageImageSize				= 500.f;
	SetTarget( st::call()->pNowRunningScene->GetObjectByTag(PLAYER));
	m_ShadowPos					= GetPostition();
}

void Enemy::initPosition(CGPoint pos)
{
	SetPosition(pos);
	m_HalfDestinationPoint = pos;
}

void Enemy::dealloc()
{
	if(m_hResource != NULL)
	{
		DeleteObject(m_hResource);
		m_hResource = NULL;
	}
}

void Enemy::render()
{
	if(GetActive() == false) return;

	CGPoint pos = GetPostition();
	HDC b = st::call()->backDC;
	HDC m = st::call()->memDC;

	pos.x += m_offSet.x;
	pos.y += m_offSet.y;
	pos.y -= m_fZ;

	SelectObject(m, m_hResource);

	BitBlt(b, pos.x, pos.y,
		m_srcSize.x, m_srcSize.y ,
		m, 
		m_srcPos.x + 1 + m_iFrame * m_srcSize.x ,m_srcPos.y + m_srcSize.y ,
		SRCAND);

	BitBlt(b, pos.x, pos.y, 
		m_srcSize.x, m_srcSize.y ,
		m,	
		m_srcPos.x + 1 + m_iFrame * m_srcSize.x ,m_srcPos.y,
		SRCPAINT);


	SelectObject(m, m_hShadow);
	BitBlt(b,  m_ShadowPos.x
		, m_ShadowPos.y+m_fZ/5, 75, 19 , m, 0 ,20 , SRCAND);

	BitBlt(b, m_ShadowPos.x
		, m_ShadowPos.y+m_fZ/5, 75, 19 , m, 0 ,40 , SRCPAINT);

	DebugDraw();
}

void Enemy::update(float dt)
{
	Animaion(dt);
	Action(dt);
}

void Enemy::Animaion(float dt)
{
	CGPoint pos = GetPostition();

	switch(m_iState)
	{
	case IDLE:
		{
			m_srcPos		= MakePoint(GetDirection() == RIGHT  ? 0.f : 80.f , 0.f);
			m_srcSize	= MakePoint(79.f, 102.f);
			m_offSet		= MakePoint(GetDirection() == RIGHT  ? -50.f : -30.f, -m_srcSize.y-55);
			m_ShadowPos	= MakePoint(GetDirection() == RIGHT  ? pos.x-35 : pos.x-30, pos.y-15);
			break;
		}
	case WALK:
		{
			if(m_Rage)
			{
				m_srcPos		= MakePoint(GetDirection() == RIGHT  ? 164+m_RageImageSize : 250+m_RageImageSize , 0.f);
				m_srcSize	= MakePoint(86.f, 80.f);
			}
			if(!m_Rage)
			{
				m_srcPos		= MakePoint(GetDirection() == RIGHT  ? 164 : 250 , 0.f);m_srcSize	= MakePoint(86.f, 82.f);
				m_srcSize	= MakePoint(86.f, 82.f);
			}	
			m_offSet		= MakePoint(GetDirection() == RIGHT  ? -27.f : -30.f, -m_srcSize.y-55);
			m_ShadowPos	= MakePoint(pos.x-15, pos.y-15);
			break;
		}
	case HIT:
		{
			if(m_Rage)
			{
				m_srcPos		= MakePoint(GetDirection() == RIGHT  ? 336+m_RageImageSize : 414+m_RageImageSize , 0.f);
				m_srcSize	= MakePoint(75.f, 120.f);
			}
			if(!m_Rage)
			{
				m_srcPos		= MakePoint(GetDirection() == RIGHT  ? 336 : 414 , 0.f);
				m_srcSize	= MakePoint(75.f, 122.f);
			}
			m_offSet		= MakePoint(GetDirection() == RIGHT  ? -50.f : -20.f, -m_srcSize.y-40);
			m_ShadowPos	= MakePoint(pos.x-30, pos.y-15);
			break;
		}
	case ATTACK:
		{
			if(m_Rage)
			{
				m_srcPos		= MakePoint(m_RageImageSize ,GetDirection() == LEFT  ? 524 : 254);
				m_srcSize	= MakePoint(135.f, 123.f);
			}
			if(!m_Rage)
			{
				m_srcPos		= MakePoint(0 ,GetDirection() == LEFT  ? 524 : 254);
				m_srcSize	= MakePoint(135.f, 125);
			}
			m_offSet		= MakePoint(GetDirection() == RIGHT  ? -50.f : -90.f, -m_srcSize.y-40);
			m_ShadowPos	= MakePoint(pos.x-30, pos.y-15);
			break;
		}
	}
}

void Enemy::Action(float dt)
{
	if(m_iHp<=0)
			this->SetActive(false);
	CGPoint pos = GetPostition();
	if(m_iPrevState != m_iState)
	{
		m_iFrame = 0;
		m_iPrevState = m_iState;
		m_fFrameDelay = 0;
	}

	m_fTime += (dt * ( m_iUpDownSpeed /100.f));
	if(m_fTime > 4.f ) m_fTime = 0;
	SetZ(sinf(m_fTime * 3.14)* HeightGap) ;

	VectorInfo vi;
	if(GetTarget()->GetActive())
		vi = Distance(m_HalfDestinationPoint, GetTarget()->GetPostition());
	
	if(m_Rage)
	{
		m_iMoveSpeed = 800;
		//m_RageModeTime += dt;
		//if(m_RageModeTime > 15)
		//{
		//	m_Rage = false;
		//	m_RageModeTime = 0;
		//}
	}
	else
	{
		m_iMoveSpeed = 200;
	}


	switch(m_iState)
	{
	case IDLE:
		{
			if(vi.distance < 300.f && vi.distance > m_iAttackRange && GetTarget()->GetActive())
			{
				m_iState = WALK;
				break;
			}

			if(m_PointOk == false)
			{
				m_DestinationPoint = 
					MakePoint(RandomRange(pos.x-280, pos.x+280), RandomRange(pos.y-280, pos.y+280));
				
				if(!st::call()->pNowRunningScene->PointExitCheck(m_DestinationPoint))
					m_PointOk = true;
			}

			if(RandomRange(0, 1000) == 0)
				m_iState = WALK;
			break;
		}
	case WALK:
		{
			m_PointOk = false;
			if((vi.distance > 300.f && !m_bHitOn) || !GetTarget()->GetActive()) vi = Distance(m_HalfDestinationPoint, m_DestinationPoint);

			vi.vector.x > 0 ? SetDirection(RIGHT) : SetDirection(LEFT);

			if(vi.distance < m_iAttackRange)
			{
				m_iState = IDLE;

				if(Distance(m_HalfDestinationPoint, GetTarget()->GetPostition()).distance < m_iAttackRange
					&& GetTarget()->GetActive())
				{
					m_iState = ATTACK;
				}
				break;
			}
			
			m_HalfDestinationPoint.x += vi.vector.x/vi.distance * dt * m_iMoveSpeed;
			m_HalfDestinationPoint.y += vi.vector.y/vi.distance * dt * m_iMoveSpeed;

			m_HalfDestinationPoint = st::call()->pNowRunningScene->PointAdjustMent(m_HalfDestinationPoint);
			break;
		}
	case HIT:
		{
			m_fFrameDelay += dt;
			if(m_fFrameDelay>0.2f) 
			if(m_fFrameDelay>0.5f)
			{
				m_bHitOn	= true;
				m_iState = WALK;
			}
			break;
		}
	case ATTACK:
		{
			//vi.vector.x > 0 ? SetDirection(RIGHT) : SetDirection(LEFT);
			if(GetTarget()->GetActive() == false)
			{
				m_iState = IDLE;
				break;
			}
			m_fFrameDelay += dt;
			if(m_fFrameDelay > 0.5f)	m_iFrame = 0;
			if(m_fFrameDelay > 0.6f)	m_iFrame = 1;
			if(m_fFrameDelay > 0.7f)	m_iFrame = 2;
			if(m_fFrameDelay > 0.8f) 
			{
				if(!m_bAttacked)
					if(Distance(m_HalfDestinationPoint, GetTarget()->GetPostition()).distance < m_iAttackRange)
					{

						if(!m_Rage)
							GetTarget()->Hit(GetDirection(), NORMAL_ATTACK);
						if(m_Rage)
							GetTarget()->Hit(GetDirection(), RAGE_ATTACK);

						m_bAttacked = true;
					}
			}
			if(m_fFrameDelay > 0.9f)
			{
				m_iFrame = 0;
				m_fFrameDelay = 0;
				m_iState = WALK;
				m_bAttacked = false;

			}
			break;
		}
	}

	pos.x += (m_HalfDestinationPoint.x - pos.x)/100;
	pos.y += (m_HalfDestinationPoint.y - pos.y)/100;

	pos = st::call()->pNowRunningScene->PointAdjustMent(pos);
	SetPosition(pos);
}

void Enemy::Hit(DIRECTION dir, int attackType)
{
	CGPoint pos = GetPostition();
	if(m_iHp < m_MaxHp/2)
		m_Rage = true;

	dir==LEFT ? SetDirection(RIGHT) : SetDirection(LEFT);

	if(m_Rage)
	{
		if(RandomRange(0,7) == 0)
			m_iState	= HIT;
	}
	if(!m_Rage)
		m_iState	= HIT;


	switch(attackType)
	{
	case NORMAL_SHOT:
		if(!m_Rage) dir==LEFT ? m_HalfDestinationPoint.x -= 60 : m_HalfDestinationPoint.x += 60;
		if(m_Rage) dir==LEFT ? m_HalfDestinationPoint.x -= 30 : m_HalfDestinationPoint.x += 30;
		st::call()->eftm.AddEffect(pos, GetDirection(), BLOOD);
		st::call()->eftm.AddEffect(pos, GetDirection(), EFFECT);
		st::call()->sm.play(SND_HIT);
		m_iHp -= RandomRange(1,3);	
		break;
	case MAHAKICK:
		if(!m_Rage) dir==LEFT ? m_HalfDestinationPoint.x -= 200 : m_HalfDestinationPoint.x += 200;
		if(m_Rage) dir==LEFT ? m_HalfDestinationPoint.x -= 70 : m_HalfDestinationPoint.x += 70;
		GetDirection() == RIGHT ?	pos.x += 30 : pos.x -= 30;
		pos.y += 50;
		st::call()->eftm.AddEffect(pos, GetDirection(), BLOOD);
		st::call()->eftm.AddEffect(pos, GetDirection(), EFFECT);
		st::call()->sm.play(SND_HIT2);
		m_iHp -= RandomRange(7,12);	
		break;
	}

}


const TCHAR* intoStateString(int _state)
{
	switch(_state)
	{
	case IDLE:	return L"IDLE";
	case WALK:	return L"WALK";
	case JUMP:	return L"JUMP";
	case ATTACK:	return L"ATTACK";
	case HIT:	return L"HIT";
	}
}

void Enemy::DebugDraw()
{
	CGPoint pos = GetPostition();
	HDC b = st::call()->backDC;


	TCHAR szStr[128] = {0};

	SelectObject(b, st::call()->bru[0]);
	SelectObject(b, st::call()->pen[2]);
	Ellipse(b, pos.x-5, pos.y-5, pos.x+5, pos.y+5);

	SelectObject(b, st::call()->pen[3]);
	Ellipse(b, m_HalfDestinationPoint.x-3, m_HalfDestinationPoint.y-3,
		m_HalfDestinationPoint.x+3, m_HalfDestinationPoint.y+3);

	SelectObject(b, st::call()->pen[1]);
	MoveToEx(b, pos.x, pos.y, NULL);
	LineTo(b, m_HalfDestinationPoint.x, m_HalfDestinationPoint.y);

	SetBkMode(b, TRANSPARENT);
	SetTextColor(b, RGB(255, 128, 0));
	SelectObject(b, st::call()->fnt[0]);
	SelectObject(b, st::call()->pen[4]);
	Rectangle(b, pos.x+30, pos.y, pos.x+120, pos.y+50);
	_stprintf_s(szStr, L"pos : %4.2f ,  %4.2f", pos.x, pos.y);
	TextOut(b, pos.x+40, pos.y+5, szStr, lstrlen(szStr));
	_stprintf_s(szStr, L"state : %s", intoStateString(m_iState));
	switch(m_iState)
	{
	case IDLE:
		SetTextColor(b, RGB(100, 255, 100));
		break;
	case WALK:
		SetTextColor(b, RGB(100, 100, 255));
		break;
	case HIT:
		SetTextColor(b, RGB(255, 0, 0));
		break;
	case ATTACK:
		SetTextColor(b, RGB(200, 10, 200));
		break;
	}
	TextOut(b, pos.x+40, pos.y+5+9, szStr, lstrlen(szStr));
	SetTextColor(b, RGB(255, 128, 0));
	_stprintf_s(szStr, L"hp : %d", m_iHp);
	TextOut(b, pos.x+40, pos.y+5+18, szStr, lstrlen(szStr));
	_stprintf_s(szStr, L"height : %2.2f", m_fZ);
	TextOut(b, pos.x+40, pos.y+5+27, szStr, lstrlen(szStr));
	st::call()->ResetGDIObject();

}