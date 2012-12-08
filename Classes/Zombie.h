#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "cocos2d.h"
#include "ControlBar.h"
#include "libs/PhysicsSprite.h"
#include "Character.h"
using namespace cocos2d;
class Zombie: public cocos2d::CCLayer {
public:

	Zombie(float _scale, CCPoint _pos, int _appType, b2World* _world);
	void dead();
	void bite();

	virtual void onEnter();
	virtual void onExit();
	void initAction();
	void update(float dt, Character* hero);
	void updateMove(float dt, int vCharater);
	void updateState(float dt, int vCharacter);
	void recall();
	void is_attacked(bool isGun);
	void remove_hold_move();
	void after_appear();
	void remove_head();

	float getX();

	void removeBodyDead();

	void getDamageHit();
	void hold_hit();

	CCRepeatForever *rotLeftArmZombie;
	CCRepeatForever *rotRightArmZombie;
	CCFiniteTimeAction *dying;

	CCFiniteTimeAction* rotRightArmHitAct;
	CCActionInterval* rotRightArmHit;
	CCActionInterval* rotRightArmHitBack;
	CCActionInterval* rotRightArmHit1;
	CCActionInterval* rotRightArmHitBack1;

	CCFiniteTimeAction* rotRightArmHitActL;
	CCActionInterval* rotRightArmHitL;
	CCActionInterval* rotRightArmHitBackL;
	CCActionInterval* rotRightArmHitL1;
	CCActionInterval* rotRightArmHitBackL1;

	CCRepeatForever *zombieWalk;
	CCAnimation *animZombieWalk;

	CCAnimate* animateZombieDead;
	CCAnimation *animZombieDead;
	CCFiniteTimeAction* zombieDead;

	CCAnimate* animateisAttackedGun;
	CCAnimation *animisAttackedGun;

	CCAnimate* animateisAttackedAxe;
	CCAnimation *animisAttackedAxe;
	CCActionInterval* movLeft;
	CCActionInterval* movRight;
	bool mov;

	CCAnimate* zombieHitAct;
	CCAnimation *animzombieHit;

	CCFiniteTimeAction* appAct;
	CCAnimate* animateApp;
	CCAnimation *animApp;
	CCSprite* app;

	CCSprite* zombie;

	CCSprite* leftArm;
	CCActionInterval * rotLeftArm;
	CCActionInterval* rotLeftArmBack;

	CCSprite* rightArm;
	CCSprite* pan;
	CCSprite* light;
	CCSprite* hat;
	CCSprite* head;
	CCSprite* blood1;
	CCSprite* blood2;
	CCSprite* blood3;
	CCActionInterval * rotRightArm;
	CCActionInterval* rotRightArmBack;

	CCSpriteFrameCache *cache;
	float scale;
	float delay;

	float hp;
	int damage;
	int appType;
	float moveSpeed;
	CCSize screen;
	bool isRight;
	bool holdMove;
	bool holdDead;
	bool holdHit;
	bool holdMoveAttacked;
	bool isAttackedGun;
	bool isAttackedAxe;
	bool hit;
	bool holdDamage;
	bool holdAppear;
	int state;
	int zombiePerLevel;
	CCPoint pos;

	int gold;
	Character* hero_t;
	b2World* world;

};
static const int _MOVE = 1;
static const int _HIT = 2;
static const int _BITE = 3;
static const int _DEAD = 4;
static const int _WAIT = 6;
static const int _APPEAR = 7;
static const int _isATTACKED = 5;
static const int _ZombieMoveSpeed = 100;
static const int _HP = 500;
#endif
