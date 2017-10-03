#pragma once
#include"Header.h"
#include"Object.h"

//LeftWall				= 30;
//RightWall				= 1380;
//TopWall				= 350;
//BottomWall			= 590;

#define MAX_OBJECT		1024

class Scene
{
	Object* m_pObjectList[MAX_OBJECT];
	Rect rect;
public:
	virtual void init();
	virtual void run(float dt);
	virtual void addObject(Object * obj, Tag tag);
	virtual void addObject(Object * obj);
	virtual void dealloc();
	virtual Object* GetObjectByTag(Tag tag);
	CGPoint PointAdjustMent(CGPoint pos);
	bool PointExitCheck(CGPoint pos);
	void zOrder();
};