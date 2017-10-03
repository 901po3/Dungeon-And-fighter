#include"Player.h"
#include"st.h"

Object* Player::Create()
{
	Object* player = new Player;
	player->init();
	return player;
}

void Player::init()
{
	m_hResource = st::call()->getBmp(IDM_Player);
	Object::init();
	position			= MakePoint(50, 450);
	m_srcPos			= MakePoint(0.f, 0.f);
	m_srcSize		= MakePoint(0.f, 0.f);
	m_iFrame			= 0;
	m_iState			= IDLE;
	m_iPrevState		= IDLE;
	m_fFrameDelay		= 0.f;
	m_fMoveSpeed		= 400;
	m_offSet			= MakePoint(0.f, 0.f);
	m_fVelocity		= 0;
	SetDirection(RIGHT);
	m_bFired			= false;
	m_bReFired		= false;
	m_bKickOnce		= false;
	m_bKicked		= false;
	m_bHitOn			= false;
	m_iHp			= 300;
	m_fWalkSoundDelay	= 0.f;
}

void Player::dealloc()
{
	DeleteObject(m_hResource);
	m_hResource = NULL;
}

void Player::render()
{
	if(GetActive() == false) return;

	CGPoint pos = GetPostition();
	HDC b = st::call()->backDC;
	HDC m = st::call()->memDC;

	Ellipse(b, pos.x-5, pos.y-5, pos.x+5, pos.y+5);

	pos.x += m_offSet.x;
	pos.y += m_offSet.y;
	pos.y -= m_fZ;

	TCHAR szStr[128] = {0};
	SelectObject(b, st::call()->bru[0]);
	SelectObject(b, st::call()->pen[1]);
	Rectangle(b, 10, 10, 100, 40);
	_stprintf_s(szStr, L" HP : %d", m_iHp);
	TextOut(b, 15, 17, szStr, lstrlen(szStr));

	SelectObject(m, m_hResource);

	BitBlt(b, pos.x, pos.y,
		m_srcSize.x, m_srcSize.y ,
		m, 
		m_srcPos.x + m_iFrame * m_srcSize.x ,m_srcPos.y + m_srcSize.y ,
		SRCAND);

	BitBlt(b, pos.x, pos.y, 
		m_srcSize.x, m_srcSize.y ,
		m,
		m_srcPos.x + m_iFrame * m_srcSize.x ,m_srcPos.y,
		SRCPAINT);
}

void Player::update(float dt)
{
	if(m_iHp <= 0)
	{
		SetActive(false);
		return;
	}
	Animaion(dt);
	Action(dt);
}

void Player::Animaion(float dt)
{
	if(m_iPrevState != m_iState)
	{
		m_iFrame = 0;
		m_iPrevState = m_iState;
		m_fFrameDelay = 0;
	}
	
	switch(m_iState)
	{
	case IDLE:
		{
			m_srcPos	= MakePoint(GetDirection() == RIGHT ? 0.f : 610.f, 10.f);
			m_srcSize	= MakePoint(54.f, 144.f);
			m_offSet	= MakePoint(-m_srcSize.x/2 , -m_srcSize.y);

			m_fFrameDelay += dt;
			if(m_fFrameDelay>0.1f)
			{
				m_fFrameDelay = 0.f;
				m_iFrame++;
				if(m_iFrame>3)
					m_iFrame = 0;
			}
			break;
		}
	case WALK:
		{
			m_srcPos	= MakePoint(GetDirection() == RIGHT ? 0.f : 610.f, 310.f);
			m_srcSize	= MakePoint(75.f, 143.f);
			m_offSet	= MakePoint(-m_srcSize.x/2 , -m_srcSize.y);

			m_fFrameDelay += dt;
			if(m_fFrameDelay>0.1f)
			{
				m_fFrameDelay = 0.f;
				m_iFrame++;
				if(m_iFrame>7)
					m_iFrame = 0;
			}
			m_fWalkSoundDelay -= dt;
			if(m_fWalkSoundDelay <= 0.f)
			{
				st::call()->sm.play(SND_WALK);
				m_fWalkSoundDelay = 0.4f;
			}
			break;
		}
	case JUMP:		
		{
			m_srcPos	= MakePoint(GetDirection() == RIGHT ? 0.f : 610.f, 610.f);
			m_srcSize	= MakePoint(85.f, 135.f);
			m_offSet	= MakePoint(-m_srcSize.x/2 , -m_srcSize.y);

			m_fFrameDelay += dt;

			if(m_fFrameDelay < 0.1f)
			{
				m_fVelocity = UP_VELOCITY;
				m_bMoveHold = true;
				break;
			}

			if(m_bMoveHold == true)
			{
				st::call()->sm.play(SND_JUMP);
				m_bMoveHold = false;
			}
			if(m_fFrameDelay > 0.1f) m_iFrame = 1;
			if(m_fFrameDelay > 0.2f) m_iFrame = 2;
			if(m_fFrameDelay > 0.3f) m_fFrameDelay = 0.1f;

			m_fVelocity		-= GRAVITY * dt;
			m_fZ				+= m_fVelocity * dt;

			if(m_fVelocity < 0)
			{
				m_fVelocity = 0.f;
				m_iState = DROP;
			}
			break;
		}
	case DROP:
		{
			m_srcPos	= MakePoint(GetDirection() == RIGHT ? 0.f : 610.f, 610.f);
			m_srcSize	= MakePoint(85.f, 135.f);
			m_offSet	= MakePoint(-m_srcSize.x/2 , -m_srcSize.y);

			m_fFrameDelay += dt;

			if(m_fZ > 0)
			{
				m_fVelocity	+= GRAVITY * dt;
				m_fZ			-= m_fVelocity * dt;
				if(m_fFrameDelay > 0.1f) m_iFrame = 3;
				if(m_fFrameDelay > 0.2f) m_iFrame = 4;
				if(m_fFrameDelay > 0.3f) m_fFrameDelay = 0.1f;
			}
			else
			{
				m_fZ = 0.f;

				if(m_bMoveHold == false)
				{
					m_bMoveHold		= true;
					m_fFrameDelay		= 0;
					m_fVelocity		= 0;
					m_fZ				= 0;
					st::call()->sm.play(SND_LAND);
				}

				m_iFrame = 5;
				if(m_fFrameDelay > 0.1f) m_iFrame = 6;
				if(m_fFrameDelay > 0.2f)
				{
					m_iState = IDLE;
					m_bMoveHold = false;
				}
			}
			break;
		}
	case ATTACK:
		{
			m_srcPos	= MakePoint(GetDirection() == RIGHT ? 0.f : 610.f, 910.f);
			m_srcSize	= MakePoint(122.f, 155.f);
			m_offSet	= MakePoint(GetDirection() == RIGHT ? -30.f : -90.f , -m_srcSize.y);

			m_fFrameDelay += dt;

			if(m_fFrameDelay == 0.f) m_bFired	= true;
			if(m_fFrameDelay > 0.1f) m_iFrame = 0;
			if(m_fFrameDelay > 0.2f) m_iFrame = 1;
			if(m_fFrameDelay > 0.3f)
			{
				m_iFrame = 2;

				if(m_bReFired == false)
				{
					st::call()->sm.play(SND_FIRE);
					CGPoint pos = GetPostition();
					GetDirection() == RIGHT ? pos.x += 95 : pos.x -= 95;
					m_bReFired = true;
					st::call()->bm.AddBullet(pos, GetDirection());
				}
			}
			if(m_fFrameDelay > 0.4f) m_iFrame = 3;
			if(m_fFrameDelay > 0.5f)	
			{
				if(m_bFired) 	m_bFired = false;
				m_iFrame = 4;
			}
			if(m_fFrameDelay > 0.7f)
			{
				m_iState = IDLE;
			}

			break;
		}
	case KICK:
		{
			m_srcPos	= MakePoint(0, GetDirection() == RIGHT  ? 1300.f : 1600.f);
			m_srcSize	= MakePoint(169.f, 130.f);
			m_offSet	= MakePoint(GetDirection() == RIGHT ? -50.f : -110.f , -m_srcSize.y);

			m_fFrameDelay += dt;
			if(m_fFrameDelay>0.1f) m_iFrame = 0;
			if(m_fFrameDelay>0.2f) m_iFrame = 1;
			if(m_fFrameDelay>0.3f) m_iFrame = 2;
			if(m_fFrameDelay>0.4f)
			{
				m_iFrame = 3;
				if(m_fFrameDelay <0.48f)
				{
					for(int i=0; i<MAX_ENEMY; i++)
					{
						CGPoint pos = GetPostition();
						GetDirection() == LEFT ? pos.x -= 40 : pos.x += 40;

						if(m_bKickOnce == false)
						{	
							if(st::call()->em.MultiCollisionCheck(pos, 55, GetDirection(), MAHAKICK))
								m_bKickOnce = true;
						}
					}
				}
			}
			if(m_fFrameDelay>0.5f) m_iFrame = 4;
			if(m_fFrameDelay>0.7f)
			{
				m_iState = IDLE;
				m_bMoveHold = false;
				m_bKickOnce = false;
			}
			break;
		}
	case HIT:
		{	
			if(m_iState == ATTACK || m_iState == KICK || m_iState == JUMP)
			{
				m_bHitOn = false;
				break;
			}
			m_srcPos	= MakePoint(GetDirection() == RIGHT  ? 0.f : 152.f ,1900.f);
			m_srcSize	= MakePoint(76.f, 128.f);
			m_offSet	= MakePoint(GetDirection() == RIGHT ? -50.f : -50.f , -m_srcSize.y);

			m_fFrameDelay += dt;
			if(m_fFrameDelay>0.01f)	m_iFrame = 0;
			if(m_fFrameDelay>0.2f) 	m_iFrame = 1;
			if(m_fFrameDelay>0.4f)
			{
				m_iState = IDLE;
				m_bHitOn = false;
				m_bFired = false;
				m_bKicked = false;
				m_bFired = false;
				m_bMoveHold = false;
			}
			break;
		}
	}
}

void Player::Action(float dt)
{
	if(m_bMoveHold == true)
		return;
	
	CGPoint pos = GetPostition();

	bool bOnAir		= (m_iState != JUMP && m_iState != DROP);
	bool bOnAttack	= (m_iState == ATTACK);
	bool nOnKick	= (m_iState == KICK);
	if(bOnAir && !bOnAttack && !nOnKick && !m_bHitOn) m_iState = IDLE;

	if(!bOnAttack)
	{
		if( GetAsyncKeyState(VK_LEFT) )		
		{
			pos.x -= m_fMoveSpeed *dt;

			if(bOnAir) m_iState = WALK;
			m_bHitOn = false;
			SetDirection(LEFT);
		}
		if( GetAsyncKeyState(VK_RIGHT))
		{
			pos.x+= m_fMoveSpeed *dt;

			if(bOnAir) m_iState = WALK;
			m_bHitOn = false;
			SetDirection(RIGHT);
		}
		if( GetAsyncKeyState(VK_UP))	
		{
			pos.y-= m_fMoveSpeed *dt/2;

			if(bOnAir) m_iState = WALK;
			m_bHitOn = false;
		}
		if( GetAsyncKeyState(VK_DOWN))	
		{
			pos.y+= m_fMoveSpeed *dt/2;

			if(bOnAir) m_iState = WALK;
			m_bHitOn = false;
		}
		if( GetAsyncKeyState('C'))	
		{
			if(bOnAir) m_iState = JUMP;
			m_bHitOn = false;
		}
	}
	if( GetAsyncKeyState('X') && bOnAir && m_bFired == false)	
	{
		m_bHitOn = false;
		if(m_bReFired = true)
		{
			m_bFired = true;
			m_fFrameDelay = 0.3f;
			m_bReFired = false;
		}
		m_iState = ATTACK;

	}
	if( GetAsyncKeyState('Z') && bOnAir && m_bFired == false && m_bKicked == false)	
	{
		m_bHitOn = false;
		m_bKicked = true;
		m_iState = KICK;
		m_bMoveHold = true;
	}
	else if(!GetAsyncKeyState('Z') && m_bKicked == true)
	{
		m_bHitOn = false;
		m_bKicked = false;
	}

	if(st::call()->pNowRunningScene->PointExitCheck(pos) )
	{
		pos = st::call()->pNowRunningScene->PointAdjustMent(pos);
	}

	SetPosition(pos);
}

void Player::Hit(DIRECTION dir, int attackType)
{
	m_bHitOn = true;
	m_bMoveHold = true;
	CGPoint pos = GetPostition();

	switch(attackType)
	{
	case NORMAL_ATTACK:
		st::call()->sm.play(SND_HIT2);
		GetDirection() == LEFT ? pos.x -= 45 : pos.y += 45;
		st::call()->eftm.AddEffect(pos, GetDirection(), BLOOD);
		st::call()->eftm.AddEffect(pos, GetDirection(), EFFECT);
		st::call()->sm.play(SND_HIT2);
		m_iHp -= RandomRange(2, 4);	
		break;
	case RAGE_ATTACK:
		st::call()->sm.play(SND_HIT2);
		GetDirection() == LEFT ? pos.x -= 45 : pos.y += 45;
		st::call()->eftm.AddEffect(pos, GetDirection(), BLOOD);
		st::call()->eftm.AddEffect(pos, GetDirection(), EFFECT);
		st::call()->sm.play(SND_HIT2);
		m_iHp -= RandomRange(15, 30);	
		break;
	}


	if(m_iState == KICK)
		return;
	if(m_iState == JUMP || m_iState == DROP)
		return;
	m_iState	= HIT;
}

