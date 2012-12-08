#include "WorldLayer.h"

using namespace cocos2d;
//using namespace CocosDenshion;

WorldLayer::WorldLayer() {
	velocity = 2.8f;
	moveState = _STAND;
	CCPoint CCPointZero(0, 0);

	cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("background.plist", "background.png");

	//Background
	CCSize screen = CCDirector::sharedDirector()->getWinSize();
	CCSprite* sky = CCSprite::create(s_sky);
	scale = screen.height / sky->getContentSize().height;

	sky->setScale(scale);
	sky->setAnchorPoint(CCPointZero);
	sky->setPosition(CCPointZero);
	this->addChild(sky, 0);

	CCSprite* sun = CCSprite::createWithSpriteFrameName("sun.png");
	sun->setScale(scale);
	sun->setAnchorPoint(CCPointZero);
	sun->setPosition(ccp(100, screen.height - sun->getContentSize().height * scale));

	this->addChild(sun, 1);

	parallax = CCParallaxScrollNode::create();
	CCSprite * city1 = CCSprite::create(s_city);
	CCSprite * city2 = CCSprite::create(s_city);
	CCSprite * city3 = CCSprite::create(s_city);

	CCSprite * street1 = CCSprite::create(s_street);
	CCSprite * street2 = CCSprite::create(s_street);
	CCSprite * street3 = CCSprite::create(s_street);

	CCSprite * house1 = CCSprite::createWithSpriteFrameName("house_01.png");
	CCSprite * house2 = CCSprite::createWithSpriteFrameName("house_02.png");
	CCSprite * house3 = CCSprite::createWithSpriteFrameName("house_03.png");
	CCSprite * house4 = CCSprite::createWithSpriteFrameName("house_04.png");
	CCSprite * house5 = CCSprite::createWithSpriteFrameName("house_05.png");
//	CCSprite * hedge1 = CCSprite::create(s_hedge1);
//	house1->addChild(hedge1);
//	hedge1->setPosition(ccp(0, 0));
	sizeWay = street1->getContentSize().height * street1->getScaleY();
	parallax->addChild(house1, 2, ccp(1, .015f),
	ccp(house1->getContentSize().width * 1.5, street1->getContentSize().height * 0.9),
	ccp(house1->getContentSize().width * 1.5 * 5, 0));
	parallax->addChild(house2, 2, ccp(1, .015f),
	ccp(house1->getContentSize().width * 1.5 * 2, street1->getContentSize().height * 0.9),
	ccp(house1->getContentSize().width * 1.5 * 5, 0));
	parallax->addChild(house3, 2, ccp(1, .015f),
	ccp(house1->getContentSize().width * 1.5 * 3, street1->getContentSize().height * 0.9),
	ccp(house1->getContentSize().width * 1.5 * 5, 0));
	parallax->addChild(house4, 2, ccp(1, .015f),
	ccp(house1->getContentSize().width * 1.5 * 4, street1->getContentSize().height * 0.9),
	ccp(house1->getContentSize().width * 1.5 * 5, 0));
	parallax->addChild(house5, 2, ccp(1, .015f),
	ccp(house1->getContentSize().width * 1.5 * 5, street1->getContentSize().height * 0.9),
	ccp(house1->getContentSize().width * 1.5 * 5, 0));

	parallax->addInfiniteScrollXWithZ(0, ccp(0.5, 0.2), ccp(0, street1->getContentSize().height * 0.9), city1, city2,
	city3, NULL);

	parallax->addInfiniteScrollXWithZ(3, ccp(1, 1), ccp(0.05, 0.1), street1, street2, street3, NULL);
	this->addChild(parallax, 2);
	parallax->setScale(scale);
//	schedule(schedule_selector(WorldLayer::update));
	scheduleUpdate();
	CCParticleSystem* m_emitter = CCParticleSnow::create();
	m_emitter->retain();
	m_emitter->setLife(3);
	m_emitter->setLifeVar(1);

	// gravity
	m_emitter->setGravity(ccp(0,-10));

	// speed of particles
	m_emitter->setSpeed(130);
	m_emitter->setSpeedVar(30);
	m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
	this->addChild(m_emitter, 1);

}

void WorldLayer::update(float dt, int _moveState, float _velocity) {
	moveState = _moveState;
	velocity = _velocity;
	if (moveState == _LEFT) {
		parallax->updateWithVelocity(ccp(1 * velocity, 0), dt, scale);
	} else if (moveState == _RIGHT) {
		parallax->updateWithVelocity(ccp(-1 * velocity, 0), dt, scale);
	}

}

