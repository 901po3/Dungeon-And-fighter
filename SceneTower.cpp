#include"SceneTower.h"
#include"BKG.h"
#include"Player.h"
#include"st.h"
#include"Enemy.h"

Scene* SceneTower::SetActive()
{
	Scene *tower = new SceneTower;
	tower->init();
	return tower;
}

void SceneTower::init()
{
	st::call()->ReplaceScene(this); //first thing you need to do to change the scene.
	st::call()->sm.play(SND_BGM);
	Scene::init();

	addObject(BKG::Create());
	
	st::call()->bm.SetScene(50, this);

	addObject(Player::Create(), PLAYER);
	st::call()->em.SetScene(10, this);
	
	st::call()->eftm.SetScene(30, this);

	srand((unsigned) time(NULL));
	for(int i=0; i<10; i++)
		st::call()->em.AddEnemy(MakePoint(RandomRange(600, 1700), RandomRange(350, 550)));
}