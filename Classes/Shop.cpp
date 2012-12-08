#include "Shop.h"
#include "ControlBar.h"
#include "Character.h"
using namespace cocos2d;

ControlBar* control;
Character* hero;

CCScene* Shop::scene() {
	CCScene* scene = CCScene::create();
	scene->retain();
	Shop *layer = Shop::create();
	scene->addChild(layer);
	return scene;
}
bool Shop::init() {
	CCLayer::init();
	mute = false;
	this->setTouchEnabled(true);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("controlbar.plist", "controlbar.png");
	cache->addSpriteFramesWithFile("shop.plist", "shop.png");
	screen = CCDirector::sharedDirector()->getWinSize();
	shop = CCSprite::create("shopbg.png");
	scale = (float) screen.height / shop->getContentSize().height;
	shop->setScale(scale);
	shop->setAnchorPoint(CCPointZero);
	this->addChild(shop);

	//ControlBar
	control = new ControlBar(scale);
	addChild(control, 10);

	hero = new Character(scale * 1.3f, 100 / 3, mute, NULL);
	addChild(hero, 3);
	hero->updateRA = false;

	//slot1
	slot1 = CCSprite::spriteWithSpriteFrameName("empty.png");
	slot1->setScale(scale);
	slot1->setAnchorPoint(ccp(0.5,1));
	slot1->setPosition(ccp(shop->getContentSize().width*0.21f,shop->getContentSize().height*0.8f));
	shop->addChild(slot1);
	slot1->setDisplayFrame(cache->spriteFrameByName("item_gun12.png"));
	//slot2
	slot2 = CCSprite::spriteWithSpriteFrameName("empty.png");
	slot2->setScale(scale);
	slot2->setAnchorPoint(ccp(0.5,1));
	slot2->setPosition(
			ccp(slot1->getPositionX()+slot1->getContentSize().width+10*scale,shop->getContentSize().height*0.8f));
	shop->addChild(slot2);
	//slot3
	slot3 = CCSprite::spriteWithSpriteFrameName("empty.png");
	slot3->setScale(scale);
	slot3->setAnchorPoint(ccp(0.5,1));
	slot3->setPosition(
			ccp(slot2->getPositionX()+slot2->getContentSize().width+10*scale,shop->getContentSize().height*0.8f));
	shop->addChild(slot3);
	//slot4
	slot4 = CCSprite::spriteWithSpriteFrameName("empty.png");
	slot4->setScale(scale);
	slot4->setAnchorPoint(ccp(0.5,1));
	slot4->setPosition(
			ccp(slot3->getPositionX()+slot3->getContentSize().width+10*scale,shop->getContentSize().height*0.8f));
	shop->addChild(slot4);
	//slot5
	slot5 = CCSprite::spriteWithSpriteFrameName("empty.png");
	slot5->setScale(scale);
	slot5->setAnchorPoint(ccp(0.5,1));
	slot5->setPosition(
			ccp(slot4->getPositionX()+slot4->getContentSize().width+10*scale,shop->getContentSize().height*0.8f));
	shop->addChild(slot5);
	//slot6
	slot6 = CCSprite::spriteWithSpriteFrameName("hp.png");
	slot6->setScale(scale);
	slot6->setAnchorPoint(ccp(0.5,1));
	slot6->setPosition(
			ccp(slot5->getPositionX()+slot5->getContentSize().width+10*scale,shop->getContentSize().height*0.8f));
	shop->addChild(slot6);
	//slot7
	slot7 = CCSprite::spriteWithSpriteFrameName("life.png");
	slot7->setScale(scale);
	slot7->setAnchorPoint(ccp(0.5,1));
	slot7->setPosition(
			ccp(slot6->getPositionX()+slot6->getContentSize().width+10*scale,shop->getContentSize().height*0.8f));
	shop->addChild(slot7);

	return true;
}
void Shop::onEnter() {
	CCLayer::onEnter();
	scheduleUpdate();

}
void Shop::update(float dt) {
	if (control->moveState == _LEFT) {
		if (shop->getPositionX() < 0) {
			shop->setPositionX(shop->getPositionX() + dt * _characterMoveSpeed);
		}
	} else if (control->moveState == _RIGHT) {
		if (shop->getPositionX() > -shop->getContentSize().width + screen.width) {
			shop->setPositionX(shop->getPositionX() - dt * _characterMoveSpeed);
		}

	}
	hero->update(dt, control->moveState, _characterMoveSpeed, control->attackState, control->gunType);
	control->update(dt, hero->countCart1, hero->countCart2, hero->countGold, hero->countKill, hero->level);
}

void Shop::ccTouchesBegan(CCSet* touches, CCEvent* event) {
//	CCSetIterator it;
//	CCTouch* touch;

//	for (it = touches->begin(); it != touches->end(); it++) {
//		touch = (CCTouch*) (*it);
//		CCPoint p = wpbar->convertTouchToNodeSpace(touch);
//		if (p.x <= wpbar->getContentSize().width && p.y <= wpbar->getContentSize().height && p.x >= 0 && p.y >= 0) {
////			ccColor3B mycolor { 102, 102, 102 };
////			wpbar->setColor(mycolor);
////			cartridge1->setColor(mycolor);
//			chooseGun();
//		}
//
//		p = moveleftBut->convertTouchToNodeSpace(touch);
//		if (p.x <= moveleftBut->getContentSize().width && p.y <= moveleftBut->getContentSize().height && p.x >= 0
//				&& p.y >= 0) {
//			isLeft = true;
//			moveleftBut->setScale(0.8f);
//		}
//		p = moverightBut->convertTouchToNodeSpace(touch);
//		if (p.x <= moverightBut->getContentSize().width && p.y <= moverightBut->getContentSize().height && p.x >= 0
//				&& p.y >= 0) {
//			isRight = true;
//		}
//		p = gun->convertTouchToNodeSpace(touch);
//		if (p.x <= gun->getContentSize().width && p.y <= gun->getContentSize().height && p.x >= 0 && p.y >= 0) {
//			isGun = true;
//		}
//		p = axe->convertTouchToNodeSpace(touch);
//		if (p.x <= axe->getContentSize().width && p.y <= axe->getContentSize().height && p.x >= 0 && p.y >= 0) {
//			isAxe = true;
//		}
//	}

}
void Shop::ccTouchesEnded(CCSet* touches, CCEvent* event) {

}
