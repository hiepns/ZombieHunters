#include "ControlBar.h"
#include "MenuStart.h"
using namespace cocos2d;
ControlBar::ControlBar(float _scale) {
	act1 = true;
	act2 = true;
	isLeft = false;
	isRight = false;
	isGun = false;
	isAxe = false;
	scale = _scale;
	gunType = 1;
	empty1 = false;
	empty2 = false;
	active = true;
	countCart1 = 0;
	countCart2 = 0;
	countGold = 0;
	countKill = 0;
	level = 1;

	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);
	CCSize screen = CCDirector::sharedDirector()->getWinSize();
	//init control bar
	moveState = _STAND;
	attackState = _NOATTACK;
	holdMove = false;
	moveleftBut = CCSprite::createWithSpriteFrameName("leftBut.png");
	moveleftBut->setScale(scale);
	moveleftBut->setAnchorPoint(CCPointZero);
	moveleftBut->setPosition(ccp(10, 5));
	//move right
	moverightBut = CCSprite::createWithSpriteFrameName("rightBut.png");
	moverightBut->setScale(scale);
	moverightBut->setAnchorPoint(CCPointZero);
	moverightBut->setPosition(ccp(moveleftBut->getContentSize().width * scale + 55, 5));

	//gun
	gun = CCSprite::createWithSpriteFrameName("gunBut.png");
	gun->setScale(scale);
	gun->setAnchorPoint(CCPointZero);
	gun->setPosition(ccp(screen.width - gun->getContentSize().width * gun->getScaleX() - 10, 5));
	//axe
	axe = CCSprite::createWithSpriteFrameName("axeBut.png");
	axe->setScale(scale);
	axe->setAnchorPoint(CCPointZero);
	axe->setPosition(ccp(
			screen.width - gun->getContentSize().width * gun->getScaleX()
			- axe->getContentSize().width * axe->getScaleX() - 25, 5));

	this->addChild(moveleftBut, 1);
	this->addChild(moverightBut, 1);
	this->addChild(gun, 1);
	this->addChild(axe, 1);

	//weapon bar
	wpbar = CCSprite::create("wpbar.png");
	wpbar->setAnchorPoint(ccp(1,1));
	wpbar->setPosition(ccp(screen.width*0.99f,screen.height*0.99f));
	wpbar->setScale(scale);
	wpbar->setOpacity(200);
	this->addChild(wpbar);

	//cartridge1
	cartridge1 = CCSprite::createWithSpriteFrameName("cartridge1.png");
	cartridge1->setAnchorPoint(ccp(0.5f, 0.5f));
	cartridge1->setPosition(ccp(wpbar->getContentSize().width*0.5f,wpbar->getContentSize().height*0.5f));
	wpbar->addChild(cartridge1);
	cartridge1->setVisible(true);
	//cout cartridge 1
	cart1 = CCLabelTTF::create("0", "Arial", 24);
	cart1->setAnchorPoint(ccp(0,0));
	cart1->setPosition(ccp(cart1->getContentSize().width*cart1->getScaleX(),0));
	cartridge1->addChild(cart1);
	//cartridge2
	cartridge2 = CCSprite::createWithSpriteFrameName("cartridge2.png");
	cartridge2->setAnchorPoint(ccp(0.5f, 0.5f));
	cartridge2->setPosition(ccp(wpbar->getContentSize().width*0.5f,wpbar->getContentSize().height*0.5f));
	wpbar->addChild(cartridge2);
	cartridge2->setVisible(false);
	//cout cartridge 2
	cart2 = CCLabelTTF::create("0", "Arial", 24);
	cart2->setAnchorPoint(ccp(0,0));
	cart2->setPosition(ccp(cart2->getContentSize().width*cart2->getScaleX(),0));
	cartridge2->addChild(cart2);
//
	gold = CCSprite::createWithSpriteFrameName("gold.png");
	gold->setAnchorPoint(ccp(0,1));
	gold->setScale(scale);
	gold->setPosition(ccp(0,screen.height));
	this->addChild(gold);
	//
	goldttf = CCLabelTTF::create("0", "Arial", 24);
	goldttf->setAnchorPoint(ccp(0,0));
	gold->addChild(goldttf);
	goldttf->setPosition(ccp(gold->getContentSize().width,0));

}
void ControlBar::onEnter() {
	CCLayer::onEnter();
	scheduleUpdate();
}
void ControlBar::update(float dt, int _countCart1, int _countCart2, int _countGold, int countKill, int _level) {
	countCart1 = _countCart1;
	countCart2 = _countCart2;
	countGold = _countGold;
	if (active) {
		if (isLeft && !isRight) {
			moveState = _LEFT;
			moveleftBut->setScale(scale * 1.1f);
			moverightBut->setScale(scale);
		} else if (isRight && !isLeft) {
			moveState = _RIGHT;
			moveleftBut->setScale(scale);
			moverightBut->setScale(scale * 1.1f);
		} else {
			moveState = _STAND;
			moveleftBut->setScale(scale);
			moverightBut->setScale(scale);
		}

		if (isGun && !isAxe) {
			attackState = _SHOOT;
			gun->setScale(scale * 1.1f);
			axe->setScale(scale);
		} else if (isAxe && !isGun) {
			attackState = _CUT;
			axe->setScale(scale * 1.1f);
			gun->setScale(scale);
		} else {
			attackState = _NOATTACK;
			axe->setScale(scale);
			gun->setScale(scale);

		}
	}
	char temp1[10];
	sprintf(temp1, "%2d", countCart1);
	cart1->setString(temp1);
	char temp2[10];
	sprintf(temp2, "%2d", countCart2);
	cart2->setString(temp2);
	char temp3[10];
	sprintf(temp3, "%2d", countGold);
	goldttf->setString(temp3);
}
void ControlBar::ccTouchesBegan(CCSet* touches, CCEvent* event) {
	CCSetIterator it;
	CCTouch* touch;

	for (it = touches->begin(); it != touches->end(); it++) {
		touch = (CCTouch*) (*it);
		CCPoint p = wpbar->convertTouchToNodeSpace(touch);
		if (p.x <= wpbar->getContentSize().width && p.y <= wpbar->getContentSize().height && p.x >= 0 && p.y >= 0) {
//			ccColor3B mycolor { 102, 102, 102 };
//			wpbar->setColor(mycolor);
//			cartridge1->setColor(mycolor);
			chooseGun();
		}

		p = moveleftBut->convertTouchToNodeSpace(touch);
		if (p.x <= moveleftBut->getContentSize().width && p.y <= moveleftBut->getContentSize().height && p.x >= 0
				&& p.y >= 0) {
			isLeft = true;
			moveleftBut->setScale(0.8f);
		}
		p = moverightBut->convertTouchToNodeSpace(touch);
		if (p.x <= moverightBut->getContentSize().width && p.y <= moverightBut->getContentSize().height && p.x >= 0
				&& p.y >= 0) {
			isRight = true;
		}
		p = gun->convertTouchToNodeSpace(touch);
		if (p.x <= gun->getContentSize().width && p.y <= gun->getContentSize().height && p.x >= 0 && p.y >= 0) {
			isGun = true;
		}
		p = axe->convertTouchToNodeSpace(touch);
		if (p.x <= axe->getContentSize().width && p.y <= axe->getContentSize().height && p.x >= 0 && p.y >= 0) {
			isAxe = true;
		}
	}

}

void ControlBar::ccTouchesEnded(CCSet* touches, CCEvent* event) {
	isLeft = false;
	isRight = false;
	isGun = false;
	isAxe = false;
//	CCLog("End");
}

void ControlBar::ccTouchesMoved(CCSet* touches, CCEvent* event) {
	CCSetIterator it;
	CCTouch* touch;

	for (it = touches->begin(); it != touches->end(); it++) {
		touch = (CCTouch*) (*it);
//		CCPoint p = wpbar->convertTouchToNodeSpace(touch);
//		if (p.x <= wpbar->getContentSize().width && p.y <= wpbar->getContentSize().height && p.x >= 0 && p.y >= 0) {
////			ccColor3B mycolor { 102, 102, 102 };
////			wpbar->setColor(mycolor);
////			cartridge1->setColor(mycolor);
//			chooseGun();
//
//		}

		CCPoint p = moveleftBut->convertTouchToNodeSpace(touch);
		if (p.x <= moveleftBut->getContentSize().width && p.y <= moveleftBut->getContentSize().height && p.x >= 0
				&& p.y >= 0) {
			isLeft = true;
		} else {
			isLeft = false;
		}

		p = moverightBut->convertTouchToNodeSpace(touch);
		if (p.x <= moverightBut->getContentSize().width && p.y <= moverightBut->getContentSize().height && p.x >= 0
				&& p.y >= 0) {
			isRight = true;
		} else {
			isRight = false;
		}

		p = gun->convertTouchToNodeSpace(touch);
		if (p.x <= gun->getContentSize().width && p.y <= gun->getContentSize().height && p.x >= 0 && p.y >= 0) {
			isGun = true;
		} else {
			isGun = false;
		}

		p = axe->convertTouchToNodeSpace(touch);
		if (p.x <= axe->getContentSize().width && p.y <= axe->getContentSize().height && p.x >= 0 && p.y >= 0) {
			isAxe = true;
		} else {
			isAxe = false;
		}
	}
}

void ControlBar::chooseGun() {
	if (gunType == 1) {
		gunType = 2;
		cartridge2->setVisible(true);
		cartridge1->setVisible(false);
	} else if (gunType == 2) {
		gunType = 1;
		cartridge1->setVisible(true);
		cartridge2->setVisible(false);
	}
}

void ControlBar::keyBackClicked() {
	CCScene* menu = MenuStart::scene();
	CCDirector::sharedDirector()->replaceScene(menu);
}
