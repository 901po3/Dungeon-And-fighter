#pragma once
#include"Object.h"

class BKG : public Object
{
public:
	static Object* Create();
	virtual void init();
	virtual void dealloc();
	virtual void render();
	virtual void update(float dt);
};