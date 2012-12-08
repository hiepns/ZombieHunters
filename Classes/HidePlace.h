#ifndef __HIDEPLACE_H__
#define __HIDEPLACE_H__
#include "cocos2d.h"
using namespace cocos2d;

class HidePlace: public CCLayer {
public:
	HidePlace(int _type, float _scale, CCPoint _pos, float _item);
	virtual void onEnter();
	virtual void onExit();
	void update(float dt, float speedC, bool _isDead, float _widthC);
	void initAction();

	CCActionInterval* rotRecycle;
	CCActionInterval* rotRecycleBack;
	CCFiniteTimeAction* rotRecycleAct;

	CCAnimation* animcardoor;
	CCAnimate* animatecardoor;
	CCActionInterval* animatecardoorBack;
	CCFiniteTimeAction* carAct;

	CCAnimation* animboxdoor;
	CCAnimate* animateboxdoor;
	CCActionInterval* animateboxdoorBack;
	CCFiniteTimeAction* phoneboxAct;

	CCSpriteFrameCache* cache;

	void hold_act();
	void call_mov();
//	void hold_mov();
	void reOrderBody();
	float getX();
	virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);

	CCSprite* recycle2;
	CCSprite* recycle1;
	CCSprite* car;
	CCSprite* cardoor;
	CCSprite* phonebox;
	CCSprite* boxdoor;

	CCSize screen;
	int item;
	bool isEmpty;
	bool holdAct;
	bool holdMove;
	bool locked;
	bool isVisible;
	int type;
	bool callMove;

};
#endif
