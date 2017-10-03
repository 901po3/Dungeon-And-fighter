#pragma once
#include"Object.h"

class Bullet : public Object
{
public:
	static Object *Create()
	{
		Object *bullet = new Bullet;
		bullet->init();
		return bullet;
	}
	virtual void init();
	virtual void update(float dt);
	virtual void render();
	virtual void dealloc();
};