#pragma once
#include"bullet.h"
#include"Scene.h"

class BulletManager
{
private:
	Object *bullet[MAX_BULLET];
public:
	HBITMAP m_phResource;

	void init();
	void SetScene(int cnt, Scene* curScene);
	void AddBullet(CGPoint pos, DIRECTION dir);
	void dealloc();
};