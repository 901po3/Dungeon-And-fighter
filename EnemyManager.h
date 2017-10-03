#pragma once
#include"Enemy.h"
#include"Scene.h"

class EnemyManager
{
private:
	Enemy *EnemyList[MAX_ENEMY];
public:
	HBITMAP m_phResource;
	HBITMAP	m_pHShadow;

	void init();
	void SetScene(int cnt, Scene* curScene);
	void AddEnemy(CGPoint pos);
	void dealloc();
	Object *CollisionCheck(CGPoint pos, int Area);
	bool MultiCollisionCheck(CGPoint pos, 
		int Area, DIRECTION dir, int AttackType);
};