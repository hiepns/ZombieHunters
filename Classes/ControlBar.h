#ifndef __CONTROL_BAR_H__
#define __CONTROL_BAR_H__

#include "cocos2d.h"
using namespace cocos2d;
class ControlBar: public cocos2d::CCLayer {

public:
	ControlBar(float _scale);
	void onEnter();
	void removeHoldCut();
	CCSprite* moveleftBut;
	CCSprite* moverightBut;
	CCSprite* axe;
	CCSprite* gun;

	CCSprite* wpbar;
	CCSprite* cartridge1;
	CCSprite* cartridge2;
	CCSprite* gold;

	int moveState;
	int attackState;
	bool holdMove;
	void update(float dt, int _countCart1, int _countCart2, int _countGold, int _countKill, int _level);
	virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
	virtual void keyBackClicked();
	void chooseGun();
	bool act1;
	bool act2;

	bool isLeft;
	bool isRight;
	bool isGun;
	bool isAxe;
	bool active;
	float scale;
	int gunType;
	bool empty1;
	bool empty2;
	int countCart1;
	int countCart2;
	int countGold;
	int countKill;
	int level;

	CCLabelTTF* cart1;
	CCLabelTTF* cart2;
	CCLabelTTF* goldttf;

};

static const int _LEFT = 1;
static const int _RIGHT = 3;
static const int _STAND = 2;
static const int _NOATTACK = 0;
static const int _HIDE = 4;
static const int _CUT = 1;
static const int _SHOOT = 2;
#endif
