#include "HidePlace.h"
using namespace cocos2d;

HidePlace::HidePlace(int _type, float _scale, CCPoint _pos, float _item) {
	item = _item;
	isEmpty = true;
	holdAct = false;
	holdMove = false;
	locked = true;
	isVisible = false;
	callMove = false;
	type = _type;
	this->setTouchEnabled(true);
	cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("hideplace.plist", "hideplace.png");

	screen = CCDirector::sharedDirector()->getWinSize();
	if (type == 1) {
		recycle2 = CCSprite::createWithSpriteFrameName("recycle02.png");
		recycle2->setScale(_scale);
		recycle2->setAnchorPoint(ccp(0.5,0));
		recycle2->setPosition(_pos);
		this->addChild(recycle2, 2);

		recycle1 = CCSprite::createWithSpriteFrameName("recycle01.png");
		recycle1->setAnchorPoint(ccp(0,0));
		recycle1->setPosition(ccp(0,recycle2->getContentSize().height*0.9f));
		recycle2->addChild(recycle1, 2);
	} else if (type == 2) {
		//car
		car = CCSprite::createWithSpriteFrameName("car.png");
		car->setScale(_scale);
		car->setPosition(_pos);
		car->setAnchorPoint(ccp(0.5,0));
		this->addChild(car, 2);
		//cardoor
		cardoor = CCSprite::createWithSpriteFrameName("car01.png");
		cardoor->setPosition(ccp(car->getContentSize().width*0.6f,car->getContentSize().height*0.53f));
		cardoor->setAnchorPoint(ccp(1,0.5));
		car->addChild(cardoor);
	} else if (type == 3) {
		//phonebox
		phonebox = CCSprite::createWithSpriteFrameName("phonebox.png");
		phonebox->setScale(_scale);
		phonebox->setPosition(_pos);
		phonebox->setAnchorPoint(ccp(0.5,0));
		this->addChild(phonebox, 2);
		//phonebox door
		boxdoor = CCSprite::createWithSpriteFrameName("phone01.png");
		boxdoor->setPosition(ccp(phonebox->getContentSize().width*0.55f,phonebox->getContentSize().height*0.43f));
		boxdoor->setAnchorPoint(ccp(0.5f,0.5f));
		phonebox->addChild(boxdoor);
	}
	initAction();
}
void HidePlace::initAction() {
	char str[100] = { 0 };
	if (type == 1) {
		rotRecycle = CCRotateBy::create(0.4f, -200);
		rotRecycleBack = rotRecycle->reverse();
		rotRecycleAct = CCSequence::create(rotRecycle,
				CCCallFuncN::create(this, callfuncN_selector(HidePlace::call_mov)), rotRecycleBack,
				CCCallFuncN::create(this, callfuncN_selector(HidePlace::hold_act)), NULL);
		rotRecycleAct->retain();
	} else if (type == 2) {
		//cardoor
		CCArray* cardoorFrames = CCArray::createWithCapacity(4);
		for (int i = 1; i < 4; i++) {
			sprintf(str, "car%02d.png", i);
			CCSpriteFrame *frame = cache->spriteFrameByName(str);
			cardoorFrames->addObject(frame);
		}
		animcardoor = CCAnimation::createWithSpriteFrames(cardoorFrames, 0.15f);
		animatecardoor = CCAnimate::create(animcardoor);
		animatecardoorBack = animatecardoor->reverse();
		carAct = CCSequence::create(animatecardoor, CCCallFuncN::create(this, callfuncN_selector(HidePlace::call_mov)),
				animatecardoorBack, CCCallFuncN::create(this, callfuncN_selector(HidePlace::hold_act)), NULL);
		carAct->retain();
	} else if (type == 3) {
		//phoneboxdoor
		CCArray* boxFrames = CCArray::createWithCapacity(4);
		for (int i = 1; i < 4; i++) {
			sprintf(str, "phone%02d.png", i);
			CCSpriteFrame *frame = cache->spriteFrameByName(str);
			boxFrames->addObject(frame);
		}
		animboxdoor = CCAnimation::createWithSpriteFrames(boxFrames, 0.15f);
		animateboxdoor = CCAnimate::create(animboxdoor);
		animateboxdoorBack = animateboxdoor->reverse();
		phoneboxAct = CCSequence::create(animateboxdoor,
				CCCallFuncN::create(this, callfuncN_selector(HidePlace::call_mov)), animateboxdoorBack,
				CCCallFuncN::create(this, callfuncN_selector(HidePlace::hold_act)), NULL);
		phoneboxAct->retain();
	}

}
void HidePlace::onEnter() {

	CCLayer::onEnter();
	scheduleUpdate();

}
void HidePlace::onExit() {

	CCLayer::onExit();

}
void HidePlace::ccTouchesBegan(CCSet* touches, CCEvent* event) {
	CCSetIterator it;
	CCTouch* touch;
	for (it = touches->begin(); it != touches->end(); it++) {
		touch = (CCTouch*) (*it);
		CCPoint p;
		if (type == 1) {
			p = recycle2->convertTouchToNodeSpace(touch);
			if (p.x <= recycle2->getContentSize().width && p.y <= recycle2->getContentSize().height && p.x >= 0
					&& p.y >= 0) {
				if (isEmpty && !holdAct && !locked) {
					isEmpty = false;
					recycle1->runAction(rotRecycleAct);
					holdAct = true;

				}
				if (!isEmpty && !holdAct) {
					isEmpty = true;
					recycle1->runAction(rotRecycleAct);
					holdAct = true;
				}
			}
		} else if (type == 2) {
			p = cardoor->convertTouchToNodeSpace(touch);
			if (p.x <= cardoor->getContentSize().width && p.y <= cardoor->getContentSize().height && p.x >= 0
					&& p.y >= 0) {
				if (isEmpty && !holdAct && !locked) {
					isEmpty = false;
					cardoor->runAction(carAct);
					holdAct = true;

				}
				if (!isEmpty && !holdAct) {
					isEmpty = true;
					cardoor->runAction(carAct);
					holdAct = true;
				}
			}
		} else if (type == 3) {
			p = boxdoor->convertTouchToNodeSpace(touch);
			if (p.x <= boxdoor->getContentSize().width && p.y <= boxdoor->getContentSize().height && p.x >= 0
					&& p.y >= 0) {
				if (isEmpty && !holdAct && !locked) {
					isEmpty = false;
					boxdoor->runAction(phoneboxAct);

					holdAct = true;

				}
				if (!isEmpty && !holdAct) {
					isEmpty = true;
					boxdoor->runAction(phoneboxAct);
					holdAct = true;
				}
			}
		}

	}

}
void HidePlace::update(float dt, float speedC, bool _isDead, float _widthC) {
	if (type == 1) {
		recycle2->setPositionX(-dt * speedC + recycle2->getPositionX());
		if (recycle2->getPositionX() < -100 || recycle2->getPositionX() > screen.width + 100) {
			recycle2->setVisible(false);
			isVisible = false;
		} else {
			recycle2->setVisible(true);
			isVisible = true;
		}

	} else if (type == 2) {
		car->setPositionX(-dt * speedC + car->getPositionX());
		if (car->getPositionX() < -100 || car->getPositionX() > screen.width + 100) {
			car->setVisible(false);
			isVisible = false;
		} else {
			car->setVisible(true);
			isVisible = true;
		}
	} else if (type == 3) {
		phonebox->setPositionX(-dt * speedC + phonebox->getPositionX());
		if (phonebox->getPositionX() < -100 || phonebox->getPositionX() > screen.width + 100) {
			phonebox->setVisible(false);
			isVisible = false;
		} else {
			phonebox->setVisible(true);
			isVisible = true;
		}
	}

	if (!_isDead && getX() < screen.width * 0.5f + _widthC && getX() > screen.width * 0.5f - _widthC) {
		locked = false;
	} else {
		locked = true;
	}

}

void HidePlace::hold_act() {
	holdAct = false;

}
//void HidePlace::hold_mov() {
//	holdMove = false;
//}

void HidePlace::call_mov() {
	if (!callMove) {
		callMove = true;
	} else {
		callMove = false;
	}

}
void HidePlace::reOrderBody() {

}

float HidePlace::getX() {
	if (type == 1) {
		return recycle2->getPositionX();
	} else if (type == 2) {
		return car->getPositionX();
	} else if (type == 3) {
		return phonebox->getPositionX();
	}
	return -1;
}
