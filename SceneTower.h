#pragma once
#include"Scene.h"

class SceneTower : public Scene
{
public:
	static Scene* SetActive();
	virtual void init();
};