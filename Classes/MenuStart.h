#ifndef __MENU_H__
#define __MENU_H__
#include "cocos2d.h"
using namespace cocos2d;
class MenuStart: public CCLayer {
public:
	virtual bool init();
	static cocos2d::CCScene* scene();

	float scaleY;
	float scaleX;
	CCSprite* bg;
	virtual void onExit();
	virtual void onEnter();
	void play();
	void resume();
	void shop();
	void menuCloseCallback(CCObject* pSender);
	float mute;
	void preload_music();
	void sound_controler();

	CREATE_FUNC(MenuStart)
	;
};
#endif
