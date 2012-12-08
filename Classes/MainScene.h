#ifndef __MAIN_H__
#define __MAIN_H__

#include "cocos2d.h"
#include "ControlBar.h"
#include "WorldLayer.h"
#include "HidePlace.h"
#include "Weapon.h"
#include "Zombie.h"
#include "libs/VisibleRect.h"

using namespace cocos2d;
class MainScene: public cocos2d::CCScene {
public:

	WorldLayer *World;
	ControlBar* Control;
	Character* hero;
//	HidePlace* hideplace;

	Bullet* bullet;
//	Zombie* zombie;
//	Zombie* zombie2;
	CCArray* ar_zombie;
	CCArray* ar_hide;
	float velocity;
	float scale;
	float pc;
	MainScene(bool _mute);
	virtual void onEnter();
	virtual void onExit();
	void update(float dt);
	void gameLoop(float dt);
	void levelGenerator(int _level);
	int getNearestZombie(bool _isRight);

	void preload_music();

	CCSpriteFrameCache* cache;

	bool holdLeft;
	bool holdRight;
	bool holdStand;
	bool holdDead;

	bool mute;

	int heroMoveSpeed;
	int gunType;

	int countHideplace;
//	bool addItem;

	CCSize screen;

	b2World* world;

};

#endif 
