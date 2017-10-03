#include"Scene.h"
#include"st.h"

void Scene::init()
{
	memset(m_pObjectList, NULL,sizeof(m_pObjectList));
	rect.LEFT		= LeftWall;
	rect.RIGHT		= RightWall;
	rect.TOP		= TopWall;
	rect.BOTTOM		= BottomWall;
}

void Scene::run(float dt)
{
	zOrder();
	for(int i=0; i<MAX_OBJECT; i++)
	{
		if(m_pObjectList[i] == NULL) continue;
		if(m_pObjectList[i]->GetActive() == false) continue;

		m_pObjectList[i]->update(dt);
		m_pObjectList[i]->render();
	}
	InvalidateRect(st::call()->hWnd, NULL, false);
}

void Scene::addObject(Object * obj, Tag tag)
{
	for(int i=0; i<MAX_OBJECT; i++)
	{
		if(m_pObjectList[i] != NULL) continue;
		
		m_pObjectList[i] = obj;
		m_pObjectList[i]->SetTag(tag);
		break;
	}
}


void Scene::addObject(Object * obj)
{
	addObject(obj, NOTHING);
}

void Scene::dealloc()
{
	for(int i=0; i<MAX_OBJECT; i++)
	{
		if(m_pObjectList[i] == NULL) continue;

		m_pObjectList[i]->dealloc();
		delete m_pObjectList[i];
		m_pObjectList[i] = NULL;
	}
}

CGPoint Scene::PointAdjustMent(CGPoint pos)
{
	pos.x = Clamp(pos.x, rect.LEFT, rect.RIGHT);
	pos.y = Clamp(pos.y, rect.TOP, rect.BOTTOM);
	return pos;
}

bool Scene::PointExitCheck(CGPoint pos)
{
	if(pos.x >= LeftWall && pos.x <= RightWall
		&& pos.y >= TopWall && pos.y <= BottomWall) return false;
	return true;
}

Object* Scene::GetObjectByTag(Tag tag)
{
	for(int i=0; i<MAX_OBJECT; i++)
	{
		if(m_pObjectList[i] == NULL) continue;
		if(m_pObjectList[i]->GetTag() == tag)
			return m_pObjectList[i];
	}
	return NULL;
}

void Scene::zOrder()
{
	int i,j;
	int step;
	Object* saveValue;
	int saveIndex;

	for(step=1; step<MAX_OBJECT; step = 3*step+1);
	for(step/=3; step>0; step/=3)
	{
		for(i=0; i<step; i++)
		{
			for(j=step+i; j<MAX_OBJECT; j+=step)
			{
				saveValue = m_pObjectList[j];
				if(saveValue == NULL) continue;
				saveIndex = j;
				while(saveIndex > step-1 && saveValue->GetPostition().y 
					< m_pObjectList[saveIndex-step]->GetPostition().y)
				{
					m_pObjectList[saveIndex] = m_pObjectList[saveIndex-step];
					saveIndex -= step;
				}
				m_pObjectList[saveIndex] = saveValue;
			}
		}
	}
}