#include"EffectManager.h"
#include"st.h"

void EffectManager::init()
{
	memset(effect, NULL, sizeof(effect));
}

void EffectManager::SetScene(int cnt, Scene* curScene)
{
	init();

	if(cnt > MAX_EFFECT) cnt = MAX_EFFECT;
	for(int i=0; i<cnt; i++)
	{
		effect[i] = Effect::Create();
		if(effect[i])
			st::call()->pNowRunningScene->addObject(effect[i]);
	}

	EffectList[BLOOD].delay					= 0.10f;
	EffectList[BLOOD].maxframe					= 7;
	EffectList[BLOOD].resource					= st::call()->getBmp(IDM_Blood);
	EffectList[BLOOD].RightDirectionOrigin		= 280.f;
	EffectList[BLOOD].srcSize.x				= 40.f;
	EffectList[BLOOD].srcSize.y				= 73.f;

	EffectList[EFFECT].delay					= 0.10f;
	EffectList[EFFECT].maxframe				= 3;
	EffectList[EFFECT].resource				= st::call()->getBmp(IDM_Effect);
	EffectList[EFFECT].RightDirectionOrigin		= 381.f;
	EffectList[EFFECT].srcSize.x				= 127.f;
	EffectList[EFFECT].srcSize.y				= 118.f;
}

void EffectManager::AddEffect(CGPoint pos, DIRECTION dir, EffectType type)
{
	int Empty = -1;
	for(int i=0; i<MAX_BULLET; i++)
	{
		if(effect[i] == NULL) 
		{
			Empty = i;
			continue;
		}
		if(effect[i]->GetActive() ) continue;

		effect[i]->SetType(type);
		effect[i]->SetActive(true);
		effect[i]->SetPosition(pos);
		effect[i]->SetEffectInfo(EffectList[type]);
		effect[i]->SetDirection(dir);
		return;
	}
	if(Empty != -1)
	{
		effect[Empty]->SetType(type);
		effect[Empty]->SetActive(true);
		effect[Empty]->SetPosition(pos);
		effect[Empty] = Effect::Create();
		effect[Empty]->SetEffectInfo(EffectList[type]);
		effect[Empty]->SetDirection(dir);
		st::call()->pNowRunningScene->addObject(effect[Empty]);
	}
}

void EffectManager::dealloc()
{
	for(int i=0; i<2; i++)
	{
		if(EffectList[i].resource != NULL)
			DeleteObject(EffectList[i].resource);
	}
}
