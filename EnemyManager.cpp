#include"EnemyManager.h"
#include"st.h"

void EnemyManager::init()
{
	memset(EnemyList, NULL, sizeof(EnemyList));
}

void EnemyManager::SetScene(int cnt, Scene* curScene)
{
	EnemyManager::init();
	m_phResource	= st::call()->getBmp(IDM_Enemy);
	m_pHShadow	= st::call()->getBmp(IDM_Shadow);

	if(cnt > MAX_BULLET) cnt = MAX_BULLET;
	for(int i=0; i<cnt; i++)
	{	
		if(EnemyList[i] != NULL) continue;

		EnemyList[i] = Enemy::Create();

		if(EnemyList[i])
			curScene->addObject(EnemyList[i], ENEMY);
	}
}

void EnemyManager::AddEnemy(CGPoint pos)
{
	int Empty = -1;
	for(int i=0; i<MAX_ENEMY; i++)
	{
		if(EnemyList[i] == NULL) 
		{
			Empty = i;
			continue;
		}
		if(EnemyList[i]->GetActive() ==true) continue;

		EnemyList[i]->SetActive(true);
		EnemyList[i]->initPosition(pos);
		return;
	}
	if(Empty != -1)
	{
		EnemyList[Empty] = Enemy::Create();
		st::call()->pNowRunningScene->addObject(EnemyList[Empty], ENEMY);
	}
}

void EnemyManager::dealloc()
{
	for(int i =0; i<MAX_BULLET; i++)
	{
		EnemyList[i]->dealloc();
	}
}

Object *EnemyManager::CollisionCheck(CGPoint pos, int Area)
{
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		if(EnemyList[i] == NULL) continue;
		if(EnemyList[i]->GetActive() == false) continue;
		
		if(Distance(EnemyList[i]->GetPostition() , pos).distance < Area)
		{
			return EnemyList[i];
		}
	}
	return NULL;
}

bool EnemyManager::MultiCollisionCheck(CGPoint pos, int Area, DIRECTION dir, int AttackType)
{
	bool check = false;
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		if(EnemyList[i] == NULL) continue;
		if(EnemyList[i]->GetActive() == false) continue;
		
		if(Distance(EnemyList[i]->GetPostition() , pos).distance < Area)
		{
			EnemyList[i]->Hit(dir, AttackType);
			check = true;
		}
	}
	return check;
}