#ifndef __WEAPON_H__
#define __WEAPON_H__
#include "cocos2d.h"
using namespace cocos2d;
class Gun: public CCSprite {
public:
	Gun(int gunType);
	CCSprite* changeGun(int gunType);
	CCPoint getpos(int gunType);
//	void update(float dt);

};
class Axe: public CCSprite {
public:
	Axe(int axeType);
	CCSprite* changeAxe(int axeType);
	CCPoint getpos(int gunType);
};
class Bullet: public CCObject {
public:
	Bullet(int _pos);
	void onEnter();
	void onExit();
	float pos;
	bool exist;
	float isRight;
	void updatePos(float dt);
};
#endif
