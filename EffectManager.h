#pragma once
#include"Effect.h"
#include"Scene.h"

#define MAX_EFFECT		64

class EffectManager
{
	Effect *effect[MAX_EFFECT];
	EffectInfo EffectList[2];

public:

	void init();
	void SetScene(int cnt, Scene* curScene);
	void AddEffect(CGPoint pos, DIRECTION dir, EffectType type);
	void dealloc();
};