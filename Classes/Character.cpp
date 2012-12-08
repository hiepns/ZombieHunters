#include "Character.h"
#include "HidePlace.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 32
using namespace cocos2d;
using namespace CocosDenshion;

Character::Character(float _scale, float _pos, bool _mute, b2World* _world) {
	CCSize screen = CCDirector::sharedDirector()->getWinSize();
	moveSpeed = 0;
	damge = 100;
	attackSpeed = 100;
	hp = _HPC;
	gunType = 1;
	countCart1 = 10;
	countCart2 = 10;
	countGold = 0;
	countKill = 0;
	level = 1;
//	state = 0;
	delay = 20.0f / _characterMoveSpeed;
	attackState = _NOATTACK;
	lastAttackState = _NOATTACK;
	tempAttackState = _NOATTACK;
	moveState = _STAND;
	lastMoveState = _STAND;
	tempMoveState = _STAND;
	scale = _scale;
	pos = _pos;
	visible = true;
	isRight = true;
	mute = _mute;
//	holdStand = false;
//	holdRight = false;
//	holdLeft = false;
	holdMoveHide = false;
	holdMoveApp = false;
	holdMoveApp2 = false;
	holdMoveState = false;
	holdAttackState = false;
	holdAttacked = false;
	isAttacked = false;
	holdCut = false;
	holdShoot = false;
	isGun2 = false;
	isDead = false;
	isGun = false;
	callMove = false;
	addItem = false;
	updateRA = true;
	holdDead = false;
	world = _world;
	//attackStateChange = false;
	cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("character.plist", "character.png");
	cache->addSpriteFramesWithFile("weapon.plist", "weapon.png");
	//must init after CCSpriteFrameCache
	initAction();
	//body
	body = CCSprite::createWithSpriteFrameName("stand01.png");
	body->setScale(scale);
	body->setFlipX(true);
	body->setAnchorPoint(ccp(0.5f, 0.0f));
	body->setPosition(ccp(screen.width * 0.5f, pos));
	this->addChild(body, tagBody);
	//head
	head = CCSprite::createWithSpriteFrameName("headMove01.png");
	head->setFlipX(true);
	head->setAnchorPoint(ccp(0.5, 0));
//	head->setPosition(ccp(body->getContentSize().width / 2, body->getContentSize().height * 0.5));
	body->addChild(head, tagHead);
	//leftArm 
	leftArm = CCSprite::createWithSpriteFrameName("leftArm.png");
	leftArm->setFlipX(true);
	leftArm->setAnchorPoint(ccp(1 - 10.0f / (leftArm->getContentSize().width*leftArm->getScaleX()),
			1 - 7.0f /( leftArm->getContentSize().height*leftArm->getScaleY())));

	leftArm->setPosition(ccp(body->getContentSize().width * 0.3, body->getContentSize().height * 0.8));
	body->addChild(leftArm, tagLeftArm); //add to bodyStand, don't need set scale
	//rightArm 
	rightArm = CCSprite::createWithSpriteFrameName("rightArm.png");
	rightArm->setScale(scale);
	rightArm->setFlipX(true);
	rightArm->setAnchorPoint(
			ccp(1 - 50.0f / rightArm->getContentSize().width, 1 - 10.0f / rightArm->getContentSize().height));

	rightArm->setPosition(ccp(body->getPosition().x + body->getContentSize().width * 0.2 * body->getScaleX(),
			body->getPosition().y + body->getContentSize().height * 0.7 * body->getScaleY()));
	this->addChild(rightArm, 1);
	//gun
	gun = CCSprite::createWithSpriteFrameName("gun11.png");
	gun->setFlipX(true);
	gun->setPosition(ccp(rightArm->getContentSize().width* rightArm->getScaleX(),
			rightArm->getContentSize().height * rightArm->getScaleY() * 0.3));
	rightArm->addChild(gun);
	gun->setVisible(false);

	gun2 = CCSprite::createWithSpriteFrameName("gun22.png");
	gun2->setFlipX(true);
	body->addChild(gun2);
	gun2->setVisible(false);
	//light
	light = CCSprite::createWithSpriteFrameName("light11.png");
	light->setAnchorPoint(ccp(0, 0.5));
	light->setFlipX(true);
	light->runAction(animlightAct);
	light->setVisible(false);
	gun->addChild(light, 3);
	//light2
	light2 = CCSprite::createWithSpriteFrameName("light31.png");
	light2->setAnchorPoint(ccp(0, 0.5));
	light2->setFlipX(true);
	light2->runAction(animlightAct2);
	light2->setVisible(false);
	gun2->addChild(light2, 3);

	//axe
	axe = CCSprite::createWithSpriteFrameName("axe.png");
	axe->setFlipX(true);
	rightArm->addChild(axe);
	axe->setVisible(true);

	hp2 = CCSprite::createWithSpriteFrameName("hp02.png");
	hp2->setScale(scale);
	hp2->setPosition(
			ccp(screen.width * 0.5f, pos+(body->getContentSize().height*body->getScaleY()+head->getContentSize().height*head->getScaleY())*0.8f));
	this->addChild(hp2);
	hp1 = CCSprite::createWithSpriteFrameName("hp01.png");
	hp1->setAnchorPoint(CCPointZero);
	hp1->setPosition(CCPointZero);
	hp2->addChild(hp1);

}
Character::~Character() {

}
void Character::onEnter() {
	CCLayer::onEnter();
	scheduleUpdate();
	ar_cartridge = new CCArray();
}

void Character::update(float dt, int _moveState, int _moveSpeed, int _attackState, int _gunType) {
	if ((holdMoveHide || holdMoveApp2) && updateRA) {
		if (isRight) {
			rightArm->setAnchorPoint(ccp(1 - 50.0f / rightArm->getContentSize().width,
					1 - 10.0f / rightArm->getContentSize().height));
			rightArm->setPosition(ccp(
							body->getPosition().x
							+ body->getContentSize().width * 0.2 * body->getScaleX(),
							body->getPosition().y
							+ body->getContentSize().height * 0.7 * body->getScaleY()));
		} else {
			leftArm->setAnchorPoint(ccp(10.0f / leftArm->getContentSize().width,
							1 - 7.0f / leftArm->getContentSize().height));
			rightArm->setPosition(ccp(
							body->getPosition().x
							- body->getContentSize().width * 0.2 * body->getScaleX(),
							body->getPosition().y
							+ body->getContentSize().height * 0.7 * body->getScaleY()));
		}
	}
	gunType = _gunType;
	if (gunType == 1) {
		damge = 100;
	} else if (gunType == 2) {
		damge = 200;
	}
	if (gunType == 2 && gun->isVisible()) {
		leftArm->setVisible(false);
		rightArm->setVisible(false);
		gun2->setVisible(true);
	} else if (gunType == 1 && gun2->isVisible()) {
		leftArm->setVisible(true);
		rightArm->setVisible(true);
		gun->setVisible(true);
		gun2->setVisible(false);
	}

	if (visible) {
		if (lastMoveState != _moveState) {
			holdMoveState = false;
			lastMoveState = _moveState;

		}
		if (lastAttackState != _attackState) {
			holdAttackState = false;
			lastAttackState = _attackState;
			tempAttackState = _attackState;

		}

		switch (tempMoveState) {
			case -1:
			break;
			case _STAND:

			dirBody(isRight);
			updateMoveState(dt, _STAND);
			moveState = _STAND;
			tempMoveState = -1;
			break;
			case _LEFT:
			isRight = false;

			dirBody(isRight);
			updateMoveState(dt, _LEFT);
			moveState = _LEFT;
			tempMoveState = -1;

			break;
			case _RIGHT:
			isRight = true;

			dirBody(isRight);
			updateMoveState(dt, _RIGHT);
			moveState = _RIGHT;
			tempMoveState = -1;

			break;
		}

		//********************attack***************
					switch (tempAttackState) {
						case -1:
						if (!holdShoot && !holdCut) {
							if (!holdMoveState) {
								tempMoveState = _moveState;
								//				CCLog("%d", tempAttackState);
							}
							holdMoveState = true;
						}
						break;
						case _NOATTACK:
						if (!holdCut && !holdShoot) {
							attackState = _NOATTACK;
							updateAttackState(dt, _NOATTACK);
							tempAttackState = -1;
							//			CCLog("123");
						}
						break;
						case _SHOOT:
						updateAttackState(dt, _SHOOT);
						tempAttackState = -1;
						break;
						case _CUT:

						moveState = _STAND;
						updateAttackState(dt, _CUT);
						head->setVisible(false);

						tempAttackState = -1;
						break;
					}
					switch (moveState) {
						case _STAND:
						moveSpeed = 0;
						break;
						case _LEFT:
						moveSpeed = -_moveSpeed;
						break;
						case _RIGHT:
						moveSpeed = _moveSpeed;
						break;
					}
				} else {
					moveState = _STAND;
				}
				if(callMove) {
					if (!holdMoveApp) {
						body->stopAllActions();
						body->setDisplayFrame(cache->spriteFrameByName("jumpbody.png"));
						body->runAction(movbodyAct);
						holdMoveApp = true;

					}
				} else if(!callMove) {
					if (!holdMoveHide) {
						body->stopAllActions();
						body->setDisplayFrame(cache->spriteFrameByName("jumpbody.png"));
						body->runAction(movbodyAct);
						holdMoveHide = true;

					}
				}

				if (hp <= 0) {
//		CCLog("Dead");
					isDead = true;
					moveSpeed = 0;
					head->setVisible(false);
					rightArm->setVisible(false);
					leftArm->setVisible(false);
					gun2->setVisible(false);
					hp1->setVisible(false);
					hp2->setVisible(false);
//					body->setVisible(false);
					if(!holdDead) {
						body->stopAllActions();
						body->runAction(deadAct);
						holdDead = true;
						SimpleAudioEngine::sharedEngine()->stopAllEffects();
						SimpleAudioEngine::sharedEngine()->playEffect(
								CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/dead.ogg"));
					}

				}
				if (isAttacked) {
					if(!holdAttacked) {
						tempMoveState = _STAND;
						isAttacked = false;
						CCFiniteTimeAction* action = CCSequence::create(CCDelayTime::create(1.0f),
								CCCallFuncN::create(this, callfuncN_selector(Character::call_item)), NULL);
						this->runAction(action);
						holdAttacked = true;
					}
				}

//				for (b2Body *b = world->GetBodyList(); b; b = b->GetNext()) {
//					if (b->GetUserData() != NULL) {
//						CCSprite *ballData = (CCSprite *) b->GetUserData();
//						if(moveState==_LEFT) {
//							ballData->setPosition(ccp(b->GetPosition().x * PTM_RATIO+dt*moveSpeed*PTM_RATIO,
//											b->GetPosition().y * PTM_RATIO));
//						} else if(moveState==_RIGHT) {
//							ballData->setPosition(ccp(b->GetPosition().x * PTM_RATIO-dt*moveSpeed*PTM_RATIO,
//											b->GetPosition().y * PTM_RATIO));
//						}
//						ballData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
//					}
//				}

				hp1->setScaleX((float)hp/_HPC);

				for (b2Body *b = world->GetBodyList(); b; b = b->GetNext()) {
					if (b->GetUserData() != NULL) {
						CCSprite *data = (CCSprite *) b->GetUserData();
						if(!data->isVisible()) {
							world->DestroyBody(b);

						}

					}
				}
			}

void Character::hold_fire() {
	holdShoot = false;
	holdCut = false;
	attackState = _NOATTACK;
	if (attackState == _NOATTACK) {
		holdMoveState = false;
	}
	light->setVisible(false);

//	CCLog("disable HoldShoot");
}
void Character::hold_cut() {
	holdCut = false;
	holdShoot = false;
	attackState = _NOATTACK;
	if (attackState == _NOATTACK) {
		holdMoveState = false;
	}
	leftArm->setVisible(true);
	body->setDisplayFrame(cache->spriteFrameByName("stand01.png"));
//	CCLog("disable HoldCut");

}

void Character::getDamageCut() {
	attackState = _CUT;
	if (!mute) {
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
		SimpleAudioEngine::sharedEngine()->playEffect(
				CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/axe.ogg"));
	}
}

void Character::getDamageShoot() {
	attackState = _SHOOT;
	if (gunType == 1) {
		if (countCart1 > 0) {
			countCart1--;
			if (!mute) {
				SimpleAudioEngine::sharedEngine()->stopAllEffects();
				SimpleAudioEngine::sharedEngine()->playEffect(
						CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/gun1.ogg"));
			}
		}

	} else if (gunType == 2) {
		if (countCart2 > 0) {
			countCart2--;
			if (!mute) {
				SimpleAudioEngine::sharedEngine()->stopAllEffects();
				SimpleAudioEngine::sharedEngine()->playEffect(
						CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/gun2.ogg"));
			}
		}
	}
	CCSprite* cartridge = CCSprite::create("cartridge2.png");
	cartridge->runAction(
			CCSequence::create(CCDelayTime::create(2.0f),
					CCCallFuncN::create(this, callfuncN_selector(Character::remove_cartridge)), NULL));
	ar_cartridge->addObject(cartridge);
	if (isRight) {
		cartridge->setPosition(ccp(body->getPosition().x + body->getContentSize().width * 0.8 * body->getScaleX(),
				body->getPosition().y + body->getContentSize().height * 0.7 * body->getScaleY()));
	} else {
		cartridge->setPosition(ccp(body->getPosition().x - body->getContentSize().width * 0.5 * body->getScaleX(),
						body->getPosition().y + body->getContentSize().height * 0.7 * body->getScaleY()));
	}
	this->addChild(cartridge, 1);

	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(cartridge->getPositionX() / PTM_RATIO, cartridge->getPositionY() / PTM_RATIO);
	ballBodyDef.userData = cartridge;
	b2Body* body = world->CreateBody(&ballBodyDef);
	if (isRight) {
		body->ApplyForce(b2Vec2(-90, 50), b2Vec2(50, 50));
	} else {
		body->ApplyForce(b2Vec2(90, 50), b2Vec2(50, 50));
	}

	b2PolygonShape box;
	box.SetAsBox(cartridge->getContentSize().width / 2.0f / PTM_RATIO,
			cartridge->getContentSize().height / 2.0f / PTM_RATIO);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.4f;
	fixtureDef.restitution = 0.1f;
	body->CreateFixture(&fixtureDef);
}

void Character::initAction() {
	char str[100] = { 0 };
	delay_func = CCDelayTime::create(0.2);
	delay_func->retain();
	//rotate RightArm Shoot is Left
	rotRightArmShootL = CCRotateBy::create(delay * 0.9, 20);
	rotRightArmShootBackL = rotRightArmShootL->reverse();
	rotRightArmShootLAct = CCSequence::create(rotRightArmShootL,
			CCCallFuncN::create(this, callfuncN_selector(Character::getDamageShoot)), rotRightArmShootBackL,
			CCCallFuncN::create(this, callfuncN_selector(Character::hold_fire)), NULL);
	rotRightArmShootLAct->retain();
	//rotate RightArmShoot is Right
	rotRightArmShoot = CCRotateBy::create(delay * 0.9, -20);
	rotRightArmShootBack = rotRightArmShoot->reverse();
	rotRightArmShootAct = CCSequence::create(rotRightArmShoot,
			CCCallFuncN::create(this, callfuncN_selector(Character::getDamageShoot)), rotRightArmShootBack,
			CCCallFuncN::create(this, callfuncN_selector(Character::hold_fire)), NULL);
	rotRightArmShootAct->retain();
	//action Cut is Left
	rotRightArmCutL1 = CCRotateBy::create(delay, 70);
	rotRightArmCutBackL1 = rotRightArmCutL1->reverse();
	rotRightArmCutL = CCRotateBy::create(delay, -80);
	rotRightArmCutBackL = rotRightArmCutL->reverse();
	rotRightArmCutActL = CCSequence::create(rotRightArmCutL1, rotRightArmCutBackL1, rotRightArmCutL,
			CCCallFuncN::create(this, callfuncN_selector(Character::getDamageCut)), delay_func, rotRightArmCutBackL,
			CCCallFuncN::create(this, callfuncN_selector(Character::hold_cut)), NULL);
	rotRightArmCutActL->retain();
	//action Cut is Right
	rotRightArmCut1 = CCRotateBy::create(delay, -70);
	rotRightArmCutBack1 = rotRightArmCut1->reverse();
	rotRightArmCut = CCRotateBy::create(delay, 80);
	rotRightArmCutBack = rotRightArmCut->reverse();
	rotRightArmCutAct = CCSequence::create(rotRightArmCut1, rotRightArmCutBack1, rotRightArmCut,
			CCCallFuncN::create(this, callfuncN_selector(Character::getDamageCut)), delay_func, rotRightArmCutBack,
			CCCallFuncN::create(this, callfuncN_selector(Character::hold_cut)), NULL);
	rotRightArmCutAct->retain();
	//bodyShoot
	CCArray* bodyShootFrames = CCArray::createWithCapacity(5);
	for (int i = 1; i < 5; i++) {
		sprintf(str, "shoot%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		bodyShootFrames->addObject(frame);
	}
	animbodyShoot = CCAnimation::createWithSpriteFrames(bodyShootFrames, delay);
	animbodyShootAct = CCAnimate::create(animbodyShoot);
	animbodyShootAct->retain();
	//bodyCut
	CCArray* bodyCutFrames = CCArray::createWithCapacity(7);
	for (int i = 1; i < 7; i++) {
		if (i == 6) {
			CCSpriteFrame *frame = cache->spriteFrameByName("hit01.png");
			bodyCutFrames->addObject(frame);
		} else {
			sprintf(str, "hit%02d.png", i);
			CCSpriteFrame *frame = cache->spriteFrameByName(str);
			bodyCutFrames->addObject(frame);
		}

	}
	animbodyCut = CCAnimation::createWithSpriteFrames(bodyCutFrames, delay);
	animbodyCutAct = CCAnimate::create(animbodyCut);
	animbodyCutAct->retain();
	//isAttacked
	CCArray* bodyAttackedFrames = CCArray::createWithCapacity(4);
	for (int i = 1; i < 4; i++) {
		sprintf(str, "attacked%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		bodyAttackedFrames->addObject(frame);
	}
	animAttacked = CCAnimation::createWithSpriteFrames(bodyAttackedFrames, delay);
	animAttackedAct = CCAnimate::create(animAttacked);
	animAttackedAct->retain();
	//light
	CCArray* lightFrames = CCArray::createWithCapacity(3);
	for (int i = 1; i < 3; i++) {
		sprintf(str, "light1%d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		lightFrames->addObject(frame);
	}
	animlight = CCAnimation::createWithSpriteFrames(lightFrames, delay / 2);
	animlightAct = CCRepeatForever::create(CCAnimate::create(animlight));
	animlightAct->retain();
	//light2
	CCArray* light2Frames = CCArray::createWithCapacity(3);
	for (int i = 1; i < 3; i++) {
		sprintf(str, "light3%d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		light2Frames->addObject(frame);
	}
	animlight2 = CCAnimation::createWithSpriteFrames(light2Frames, delay / 2);
	animlightAct2 = CCRepeatForever::create(CCAnimate::create(animlight2));
	animlightAct2->retain();
	//move leftArm Stand
	movLeftArmStand = CCRotateBy::create(delay * 5, -5);
	movLeftArmStandBack = movLeftArmStand->reverse();
	movLeftArmStandAct = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(movLeftArmStand, movLeftArmStandBack, NULL));
	movLeftArmStandAct->retain();
	// rotate LeftArm move
	rotLeftArmMove = CCRotateBy::create(delay * 3, 15);
	rotLeftArmMoveBack = rotLeftArmMove->reverse();
	rotLeftArmMoveAct = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(rotLeftArmMove, rotLeftArmMoveBack, NULL));
	rotLeftArmMoveAct->retain();
	//rightArmMove no shoot
	rotRightArmMove = CCRotateBy::create(delay * 3, -12);
	rotRightArmMoveBack = rotRightArmMove->reverse();
	rotRightArmMoveAct = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(rotRightArmMove, rotRightArmMoveBack, NULL));
	rotRightArmMoveAct->retain();
	//bodyStand no attack
	CCArray* bodyStandFrames = CCArray::createWithCapacity(5);
	for (int i = 1; i < 5; i++) {
		sprintf(str, "stand%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		bodyStandFrames->addObject(frame);
	}

	animbodyStand = CCAnimation::createWithSpriteFrames(bodyStandFrames, delay + 0.1);
	animbodyStandAct = CCRepeatForever::create(CCAnimate::create(animbodyStand));
	animbodyStandAct->retain();
	//move HeadStand
	movHeadStand = CCMoveBy::create(0.3f, ccp(0, -2));
	movHeadStandBack = movHeadStand->reverse();
	movHeadStandAct = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(movHeadStand, movHeadStandBack, NULL));
	movHeadStandAct->retain();
	//move RightArm Stand

	movRightArmStand = CCRotateBy::create(delay * 5, -5);
	movRightArmStandBack = movRightArmStand->reverse();
	movRightArmStandAct = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(movRightArmStand, movRightArmStandBack, NULL));
	movRightArmStandAct->retain();
	//body Move
	CCArray* bodyMoveFrames = CCArray::createWithCapacity(7);
	for (int i = 1; i < 7; i++) {
		sprintf(str, "move%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		bodyMoveFrames->addObject(frame);
	}
	animBodyMove = CCAnimation::createWithSpriteFrames(bodyMoveFrames, delay);
	animBodyMoveAct = CCRepeatForever::create(CCAnimate::create(animBodyMove));
	animBodyMoveAct->retain();
	//head move
	CCArray* headMoveFrames = CCArray::createWithCapacity(7);
	for (int i = 1; i < 7; i++) {
		sprintf(str, "headMove%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		headMoveFrames->addObject(frame);
	}
	animHeadMove = CCAnimation::createWithSpriteFrames(headMoveFrames, delay);
	animHeadMoveAct = CCRepeatForever::create(CCAnimate::create(animHeadMove));
	animHeadMoveAct->retain();
	//dead
	CCArray* deadFrames = CCArray::createWithCapacity(9);
	for (int i = 1; i < 9; i++) {
		sprintf(str, "dead%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		deadFrames->addObject(frame);
	}
	animDead = CCAnimation::createWithSpriteFrames(deadFrames, delay);
	deadAct = CCAnimate::create(animDead);
	deadAct->retain();

	//rotGun2
	rotGun2 = CCRotateBy::create(0.5f, 10);
	rotGun2Back = rotGun2->reverse();
	rotGun2Act = CCRepeatForever::create((CCActionInterval*) CCSequence::create(rotGun2, rotGun2Back, NULL));
	rotGun2Act->retain();
	//rotGun2 left
	rotGun2L = CCRotateBy::create(0.5f, -10);
	rotGun2BackL = rotGun2L->reverse();
	rotGun2ActL = CCRepeatForever::create((CCActionInterval*) CCSequence::create(rotGun2L, rotGun2BackL, NULL));
	rotGun2ActL->retain();

	//rotGun2 Stand
	rotGun2S = CCRotateBy::create(0.5f, 5);
	rotGun2BackS = rotGun2S->reverse();
	rotGun2ActS = CCRepeatForever::create((CCActionInterval*) CCSequence::create(rotGun2S, rotGun2BackS, NULL));
	rotGun2ActS->retain();
	//rotGun2 left stand
	rotGun2SL = CCRotateBy::create(0.5f, -5);
	rotGun2BackSL = rotGun2SL->reverse();
	rotGun2ActSL = CCRepeatForever::create((CCActionInterval*) CCSequence::create(rotGun2SL, rotGun2BackSL, NULL));
	rotGun2ActSL->retain();
	//rotGun2 Shoot
	rotGun2Shoot = CCRotateBy::create(0.5f, 30);
	rotGun2ShootBack = rotGun2Shoot->reverse();
	rotGun2ShootAct = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(rotGun2Shoot, rotGun2ShootBack, NULL));
	rotGun2ShootAct->retain();
	//rotGun2 Shoot Left
	rotGun2ShootL = CCRotateBy::create(0.5f, -30);
	rotGun2ShootBackL = rotGun2ShootL->reverse();
	rotGun2ShootActL = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(rotGun2ShootL, rotGun2ShootBackL, NULL));
	rotGun2ShootActL->retain();

	//hide Action

	zoomin = CCScaleBy::create(3, 3);
	zoomin->retain();

	movbody = CCMoveBy::create(0.15f, ccp(0,100*scale));
	movbodyBack = movbody->reverse();
	movbodyAct = CCSequence::create(movbody, CCCallFuncN::create(this, callfuncN_selector(Character::hideAction)),
			movbodyBack, CCCallFuncN::create(this, callfuncN_selector(Character::call_update)), NULL);
	movbodyAct->retain();

}

void Character::updateMoveState(float dt, int _moveState) {
	body->setVisible(true);
	head->setVisible(true);
	if (!isGun2) {
		rightArm->setVisible(true);
		leftArm->setVisible(true);
	}
	dirBody(isRight);
	if (_moveState == _STAND) {

		//leftArm
		leftArm->setRotation(5);
		leftArm->stopAllActions();
		leftArm->runAction(movLeftArmStandAct);
		//gun2
		gun2->setRotation(0);

		if (isRight) {
			gun2->stopAllActions();
			gun2->runAction(rotGun2ActS);
		} else {
			gun2->stopAllActions();
			gun2->runAction(rotGun2ActSL);
		}
		//light
		light->setVisible(false);
		light2->setVisible(false);
		//bodyStand no attack
		body->stopAllActions();
		body->runAction(animbodyStandAct);
		//headStand no attack
		head->setPosition(ccp(body->getContentSize().width / 2, body->getContentSize().height * 0.5-2));
		head->stopAllActions();
		head->runAction(movHeadStandAct);

		//rightArmStand no attack
		rightArm->stopAllActions();
		rightArm->runAction(movRightArmStandAct);

	} else {
		//bodyMove

		body->stopAllActions();
		body->runAction(animBodyMoveAct);
		//headMove

		head->stopAllActions();
		head->runAction(animHeadMoveAct);
		//leftArmMove

		leftArm->stopAllActions();
		leftArm->runAction(rotLeftArmMoveAct);
		//rightArmMove no shoot

		rightArm->stopAllActions();
		rightArm->runAction(rotRightArmMoveAct);
		//gun2
		gun2->setRotation(0);

		if (isRight) {
			gun2->stopAllActions();
			gun2->runAction(rotGun2Act);
		} else {
			gun2->stopAllActions();
			gun2->runAction(rotGun2ActL);
		}

	}
}

void Character::updateAttackState(float dt, int _attackState) {

	if (!isGun2) {
		rightArm->setVisible(true);
		leftArm->setVisible(true);
	}
	if (_attackState == _NOATTACK) {
		light->setVisible(false);
		light2->setVisible(false);
		body->setVisible(true);
		head->setVisible(true);

//		dirBody(isRight);
//			axe->setVisible(false);
//			gun->setVisible(true);
	} else if (_attackState == _SHOOT && !holdCut && !(gunType == 1 && countCart1 <= 0)
			&& !(gunType == 2 && countCart2 <= 0)) {
		body->setVisible(true);
		head->setVisible(true);
		if (gunType == 1) {
			gun->setVisible(true);

		} else {
			gun2->setVisible(true);
			rightArm->setVisible(false);
			leftArm->setVisible(false);
			isGun2 = true;

		}
		isGun = true;
		if (!holdShoot) {
			dirBody(isRight);

			light->setVisible(true);
			light2->setVisible(true);
//			attackState = _SHOOT;
			axe->setVisible(false);
			if (!isRight) {

				rightArm->stopAllActions();
				rightArm->runAction(rotRightArmShootLAct);

				body->stopAllActions();
				gun2->stopAllActions();
				gun2->setRotation(0);
				gun2->runAction(rotGun2ShootAct);
				if (moveState == _STAND) {
					body->runAction(animbodyShootAct);

				} else {
					body->runAction(animBodyMoveAct);

				}

			} else {

				rightArm->stopAllActions();
				rightArm->runAction(rotRightArmShootAct);
				gun2->stopAllActions();
				gun2->setRotation(0);
				gun2->runAction(rotGun2ShootActL);
				body->stopAllActions();
				if (moveState == _STAND) {
					body->runAction(animbodyShootAct);

				} else {
					body->runAction(animBodyMoveAct);

				}

			}

		}
		holdShoot = true;

	} else if (_attackState == _CUT && !holdShoot) {
		isGun2 = false;
		isGun = false;
		rightArm->setVisible(true);
		head->setVisible(false);
		leftArm->setVisible(false);
		if (!holdCut) {
			dirBody(isRight);
			gun->setVisible(false);
			gun2->setVisible(false);
			axe->setVisible(true);
			if (!isRight) {
				rightArm->stopAllActions();
				rightArm->runAction(rotRightArmCutActL);
				leftArm->setVisible(false);

				body->stopAllActions();
				body->runAction(animbodyCutAct);

			} else {
				rightArm->stopAllActions();
				rightArm->runAction(rotRightArmCutAct);
				leftArm->setVisible(false);

				body->stopAllActions();
				body->runAction(animbodyCutAct);

			}
		}
		holdCut = true;
	}
}

void Character::onExit() {
	CCLayer::onExit();

}
void Character::dirBody(bool _isRight) {
//	body->setDisplayFrame(cache->spriteFrameByName("stand01.png"));
//	head->setPosition(ccp(body->getContentSize().width / 2, body->getContentSize().height * 0.5));

	if (!_isRight) {
		body->setFlipX(false);
		head->setFlipX(false);

//		head->setPosition(ccp(body->getContentSize().width / 2-2, body->getContentSize().height * 0.5-2));

		gun->setFlipX(false);
		gun2->setFlipX(false);
		gun2->setAnchorPoint(ccp(1.0f,1.0f));
		gun2->setPosition(ccp(body->getContentSize().width*0.9f,body->getContentSize().height*0.85f));
		gun->setPosition(ccp(0, rightArm->getContentSize().height * rightArm->getScaleY() * 0.3));

		light->setFlipX(false);
		light->setRotation(-20);
		light->setAnchorPoint(ccp(1, 0.5));
		light->setPosition(ccp(0, gun->getContentSize().height * gun->getScaleY()*0.4));

		light2->setFlipX(false);
		light2->setRotation(0);
		light2->setAnchorPoint(ccp(1, 0.5));
		light2->setPosition(ccp(0, gun2->getContentSize().height*0.6));

		leftArm->setFlipX(false);
		leftArm->setAnchorPoint(ccp(10.0f / leftArm->getContentSize().width,
				1 - 7.0f / leftArm->getContentSize().height));
		leftArm->setPosition(ccp(body->getContentSize().width * 0.7, body->getContentSize().height * 0.8));

		rightArm->setFlipX(false);
		rightArm->setRotation(15);
		rightArm->setAnchorPoint(ccp(50.0f / rightArm->getContentSize().width,
				1 - 10.0f / rightArm->getContentSize().height));

		rightArm->setPosition(ccp(
				body->getPosition().x
				- body->getContentSize().width * 0.2 * body->getScaleX(),
				body->getPosition().y
				+ body->getContentSize().height * 0.7 * body->getScaleY()));

		axe->setFlipX(false);
//axe->setAnchorPoint(ccp(1,0));
		axe->setPosition(ccp(-rightArm->getContentSize().width*0.3,
				rightArm->getContentSize().height));

	} else {
		body->setFlipX(true);

		head->setFlipX(true);
		if(moveState==_STAND) {
			head->setPosition(ccp(body->getContentSize().width / 2+5, body->getContentSize().height * 0.5-2));
		}

		rightArm->setFlipX(true);
		rightArm->setRotation(-15);
		rightArm->setAnchorPoint(
				ccp(1 - 50.0f / rightArm->getContentSize().width,
						1 - 10.0f / rightArm->getContentSize().height));

		rightArm->setPosition(
				ccp(
						body->getPosition().x
						+ body->getContentSize().width * 0.2 * body->getScaleX(),
						body->getPosition().y
						+ body->getContentSize().height * 0.7 * body->getScaleY()));

		leftArm->setFlipX(true);
		leftArm->setAnchorPoint(
				ccp(1 - 10.0f / leftArm->getContentSize().width,
						1 - 7.0f / leftArm->getContentSize().height));

		leftArm->setPosition(
				ccp(body->getContentSize().width * 0.3, body->getContentSize().height * 0.8));

		gun->setFlipX(true);
		gun2->setFlipX(true);
		gun2->setAnchorPoint(ccp(0.0f,1.0f));
		gun2->setPosition(ccp(body->getContentSize().width*0.1f,body->getContentSize().height*0.85f));
		gun->setPosition(
				ccp(rightArm->getContentSize().width,
						rightArm->getContentSize().height * rightArm->getScaleY() * 0.3));

		light->setFlipX(true);
		light->setAnchorPoint(ccp(0, 0.5));
		light->setPosition(
				ccp(gun->getContentSize().width * gun->getScaleX(),
						gun->getContentSize().height * gun->getScaleY()*0.4));
		light->setRotation(20);

		light2->setFlipX(true);
		light2->setAnchorPoint(ccp(0, 0.5));
		light2->setPosition(
				ccp(gun2->getContentSize().width * gun2->getScaleX(),
						gun2->getContentSize().height*0.6));
		light2->setRotation(0);

		axe->setFlipX(true);
		//axe->setAnchorPoint(ccp(0.35,0.05));
				axe->setPosition(
						ccp(rightArm->getContentSize().width * 1.3, rightArm->getContentSize().height));
			}
		}
void Character::hideAction() {

	if (visible) {
		this->setVisible(true);
		holdMoveApp2 = true;
	} else {
		this->setVisible(false);
		CCFiniteTimeAction* action = CCSequence::create(CCDelayTime::create(1.0f),
				CCCallFuncN::create(this, callfuncN_selector(Character::call_item)), NULL);
		this->runAction(action);

	}
	SimpleAudioEngine::sharedEngine()->playEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/opendoor.ogg"));
}
void Character::call_update() {
	tempMoveState = _STAND;
	holdMoveApp2 = false;
}
void Character::call_item() {

	addItem = true;
}
void Character::hold_attacked() {
	holdAttacked = false;
//	holdMoveState = false;
}
void Character::remove_cartridge() {
	ar_cartridge->reverseObjects();
	CCSprite* temp = (CCSprite*) ar_cartridge->lastObject();
	temp->setVisible(false);
	ar_cartridge->removeLastObject(true);
	ar_cartridge->reverseObjects();

}
