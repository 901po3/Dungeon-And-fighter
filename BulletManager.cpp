#include"BulletManager.h"
#include"st.h"
void BulletManager::init()
{
	memset(bullet, NULL, sizeof(bullet));
}

void BulletManager::SetScene(int cnt, Scene* curScene)
{
	BulletManager::init();
	if(m_phResource == NULL)
		m_phResource = st::call()->getBmp(IDM_Bullet);

	if(cnt > MAX_BULLET) cnt = MAX_BULLET;
	for(int i=0; i<cnt; i++)
	{	
		if(bullet[i] != NULL) continue;

		bullet[i] = Bullet::Create();

		if(bullet[i])
			curScene->addObject(bullet[i]);
	}
}

void BulletManager::AddBullet(CGPoint pos, DIRECTION dir)
{
	int Empty = -1;
	for(int i=0; i<MAX_BULLET; i++)
	{
		if(bullet[i] == NULL) 
		{
			Empty = i;
			continue;
		}
		if(bullet[i]->GetActive() ) continue;

		bullet[i]->SetActive(true);
		bullet[i]->SetPosition(pos);
		bullet[i]->SetDirection(dir);
		return;
	}
	if(Empty != -1)
	{
		bullet[Empty] = Bullet::Create();
		bullet[Empty]->SetActive(false);
		st::call()->pNowRunningScene->addObject(bullet[Empty]);
	}
}

void BulletManager::dealloc()
{
	if(m_phResource != NULL)
		DeleteObject(m_phResource);
}
