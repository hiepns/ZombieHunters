#ifndef __SHOP_H__
#define __SHOP_H__

#include "cocos2d.h"

using namespace cocos2d;
class Shop: public CCLayer {
public:
	static CCScene* scene();
	virtual bool init();
	void update(float dt);
	virtual void onEnter();

	virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
	float scale;
	bool mute;
	CCSize screen;
	CCSprite* shop;
	CCSprite* slot1;
	CCSprite* slot2;
	CCSprite* slot3;
	CCSprite* slot4;
	CCSprite* slot5;
	CCSprite* slot6;
	CCSprite* slot7;

	CREATE_FUNC(Shop)
	;
};

#endif
