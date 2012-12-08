#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "cocos2d.h"
#include "libs/CCParallaxScrollNode.h"
#include "ControlBar.h"

class WorldLayer: public cocos2d::CCLayer {
public:
	WorldLayer();
	float sizeWay;
	float scale;
	float velocity;
	int moveState;
	CCSpriteFrameCache* cache;
	CCParallaxScrollNode *parallax;
	void update(float dt, int _moveState, float _velocity);

};
static const char s_sky[] = "sky.png";
static const char s_city[] = "city.png";
static const char s_street[] = "street.png";

#endif
