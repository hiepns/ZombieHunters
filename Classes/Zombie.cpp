#include "Zombie.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"

#define PTM_RATIO 32

using namespace cocos2d;
using namespace CocosDenshion;

Zombie::Zombie(float _scale, CCPoint _pos, int _appType, b2World* _world) {
	world = _world;
	pos = _pos;
	hp = _HP;
	damage = 50;
	moveSpeed = _ZombieMoveSpeed;
	delay = 10.0f / moveSpeed;
//	delay = 0.15f;
	appType = _appType;
	gold = 10;

	scale = _scale;
	isRight = true;
	holdMove = false;
	holdDead = false;
	mov = false;
	isAttackedGun = false;
	isAttackedAxe = false;
	holdMoveAttacked = false;
	holdDamage = false;
	hit = false;
	holdHit = false;
	holdAppear = false;
	zombiePerLevel = 3;
	state = _WAIT;
	screen = CCDirector::sharedDirector()->getWinSize();
	cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("zombie1.plist", "zombie1.png");

	/************Zombie******************/
	zombie = CCSprite::createWithSpriteFrameName("zombieWalk01.png");
	zombie->setScale(scale);
	zombie->setAnchorPoint(ccp(0.5, 0));
	zombie->setPosition(pos);
	zombie->setVisible(false);
	this->addChild(zombie, 3);
	//leftArm
	leftArm = CCSprite::createWithSpriteFrameName("zombieRightArm.png");
	leftArm->setScale(scale);
	leftArm->setVisible(false);
	this->addChild(leftArm, 5);
	//rightArm
	rightArm = CCSprite::createWithSpriteFrameName("zombieLeftArm.png");
	rightArm->setAnchorPoint(ccp(1, 1));
	rightArm->setScale(scale);
	rightArm->setVisible(false);
	this->addChild(rightArm, 1);
	//pan
	pan = CCSprite::createWithSpriteFrameName("Zombie-hit-pan.png");
	rightArm->addChild(pan);
	pan->setVisible(false);
	//light
	light = CCSprite::createWithSpriteFrameName("hitLight.png");
	pan->addChild(light);
	light->setVisible(false);
	//stone
	app = CCSprite::createWithSpriteFrameName("appear13.png");
	app->setScale(scale);
	app->setAnchorPoint(ccp(0.5, 0));
	app->setPosition(pos);
	app->setVisible(false);
	this->addChild(app, 4);
	//hat
//	hat = CCSprite::create("hat1.png");
//	zombie->addChild(hat);
//	hat->setAnchorPoint(ccp(0.5,0));
//	hat->setPosition(ccp(zombie->getContentSize().width*0.5f,zombie->getContentSize().height));
	initAction();
//	ccColor3B mycolor { 255, 99, 71 };
//	zombie->setColor(mycolor);

}
void Zombie::initAction() {
	char str[100] = { 0 };

	//move
	CCArray* ZombieWalkFrames = CCArray::createWithCapacity(8);
	for (int i = 1; i < 8; i++) {
		sprintf(str, "zombieWalk%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		ZombieWalkFrames->addObject(frame);
	}
	animZombieWalk = CCAnimation::createWithSpriteFrames(ZombieWalkFrames, delay);
	zombieWalk = CCRepeatForever::create(CCAnimate::create(animZombieWalk));
	zombieWalk->retain();

	// rotate left arm
	rotLeftArm = CCRotateBy::create(delay * 3, -20);
	rotLeftArmBack = rotLeftArm->reverse();
	rotLeftArmZombie = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(rotLeftArm, rotLeftArmBack, NULL));
	rotLeftArmZombie->retain();
	//rotate right arm
	rotRightArm = CCRotateBy::create(delay * 3, 20);
	rotRightArmBack = rotRightArm->reverse();
	rotRightArmZombie = CCRepeatForever::create(
			(CCActionInterval*) CCSequence::create(rotRightArm, rotRightArmBack, NULL));
	rotRightArmZombie->retain();
	//zombie dead
	CCArray* ZombieDeadFrames = CCArray::createWithCapacity(8);
	for (int i = 1; i < 8; i++) {
		sprintf(str, "zombieDead%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		ZombieDeadFrames->addObject(frame);
	}
	animZombieDead = CCAnimation::createWithSpriteFrames(ZombieDeadFrames, 0.15f);
	animateZombieDead = CCAnimate::create(animZombieDead);
	zombieDead = CCSequence::create(animateZombieDead,
			CCCallFunc::create(this, callfunc_selector(Zombie::removeBodyDead)), NULL);
	zombieDead->retain();
	//zombie isAttacked by gun
	CCArray* isAttackGunFrames = CCArray::createWithCapacity(3);
	for (int i = 1; i < 3; i++) {
		sprintf(str, "zombieAttacked%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		isAttackGunFrames->addObject(frame);
	}
	animisAttackedGun = CCAnimation::createWithSpriteFrames(isAttackGunFrames, 0.15f);
	animateisAttackedGun = CCAnimate::create(animisAttackedGun);
	animateisAttackedGun->retain();
	//zombie isAttacked by axe
	CCArray* isAttackAxeFrames = CCArray::createWithCapacity(3);
	for (int i = 1; i < 3; i++) {
		sprintf(str, "zombieAttacked%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		isAttackAxeFrames->addObject(frame);
	}
	animisAttackedAxe = CCAnimation::createWithSpriteFrames(isAttackAxeFrames, 0.15f);
	movRight = CCMoveBy::create(1.0f, ccp(100,0));
	movRight->retain();
	movLeft = CCMoveBy::create(1.0f, ccp(-100,0));
	movLeft->retain();
	animateisAttackedAxe = CCAnimate::create(animisAttackedAxe);
	animateisAttackedAxe->retain();

	//zombie hit
	CCArray* zombieHitFrames = CCArray::createWithCapacity(7);
	for (int i = 1; i < 7; i++) {
		if (i != 6) {
			sprintf(str, "zombieHit%02d.png", i);
			CCSpriteFrame *frame = cache->spriteFrameByName(str);
			zombieHitFrames->addObject(frame);
		} else {
			CCSpriteFrame *frame = cache->spriteFrameByName("zombieHit01.png");
			zombieHitFrames->addObject(frame);
		}

	}
	animzombieHit = CCAnimation::createWithSpriteFrames(zombieHitFrames, 0.05f);
	zombieHitAct = CCAnimate::create(animzombieHit);
	zombieHitAct->retain();

	//action Cut is Left
//	rotRightArmHitL1 = CCRotateBy::create(0.3f, 70);
//	rotRightArmHitBackL1 = rotRightArmHitL1->reverse();
	rotRightArmHitL = CCRotateBy::create(0.15f, 120);
	rotRightArmHitBackL = rotRightArmHitL->reverse();
	rotRightArmHitActL = CCSequence::create(rotRightArmHitL, rotRightArmHitBackL,
			CCCallFuncN::create(this, callfuncN_selector(Zombie::getDamageHit)), CCDelayTime::create(0.2f),
			CCCallFuncN::create(this, callfuncN_selector(Zombie::hold_hit)), NULL);
	rotRightArmHitActL->retain();
	//action Cut is Right
//	rotRightArmHit1 = CCRotateBy::create(0.3f, -70);
//	rotRightArmHitBack1 = rotRightArmHit1->reverse();
	rotRightArmHit = CCRotateBy::create(0.15f, -120);
	rotRightArmHitBack = rotRightArmHit->reverse();
	rotRightArmHitAct = CCSequence::create(rotRightArmHit, rotRightArmHitBack,
			CCCallFuncN::create(this, callfuncN_selector(Zombie::getDamageHit)), CCDelayTime::create(0.2f),
			CCCallFuncN::create(this, callfuncN_selector(Zombie::hold_hit)), NULL);
	rotRightArmHitAct->retain();

	//appear
	CCArray* appFrames = CCArray::createWithCapacity(14);
	for (int i = 1; i < 14; i++) {

		sprintf(str, "appear%02d.png", i);
		CCSpriteFrame *frame = cache->spriteFrameByName(str);
		appFrames->addObject(frame);

	}
	animApp = CCAnimation::createWithSpriteFrames(appFrames, 0.05f);
	animateApp = CCAnimate::create(animApp);
	appAct = CCSequence::create(animateApp, CCCallFuncN::create(this, callfuncN_selector(Zombie::after_appear)), NULL);
	appAct->retain();

}
void Zombie::removeBodyDead() {
	zombie->setVisible(false);
	leftArm->setVisible(false);
	rightArm->setVisible(false);
}
void Zombie::updateMove(float dt, int vCharacter) {

	if (isRight) {
		zombie->setFlipX(true);

		leftArm->setFlipX(true);
		leftArm->setAnchorPoint(ccp(0.5, 1));
		leftArm->setPosition(ccp(zombie->getPosition().x - zombie->getContentSize().width * zombie->getScaleX() * 0.22,
				zombie->getPosition().y + zombie->getContentSize().height* zombie->getScaleY() * 0.52));

		rightArm->setFlipX(true);
		rightArm->setAnchorPoint(ccp(0, 1));
		rightArm->setPosition(
				ccp(zombie->getPosition().x + zombie->getContentSize().width* zombie->getScaleX() / 2 * 0.1,
						zombie->getPosition().y + zombie->getContentSize().height* zombie->getScaleY()* 0.52));
		pan->setPosition(ccp(rightArm->getContentSize().width*0.6f, -rightArm->getContentSize().height*0.1f));
		pan->setAnchorPoint(ccp(0,0));
		pan->setFlipX(true);

		light->setAnchorPoint(ccp(0.5f,0.5f));
		light->setPosition(
				ccp(pan->getContentSize().width*pan->getScaleX()*0.9f,pan->getContentSize().height*pan->getScaleX()*0.7f));
		light->setFlipX(true);
	} else {
		zombie->setFlipX(false);
		leftArm->setFlipX(false);
		leftArm->setAnchorPoint(ccp(0.5, 1));
		leftArm->setPosition(ccp(zombie->getPosition().x + zombie->getContentSize().width* zombie->getScaleX() * 0.22,
				zombie->getPosition().y + zombie->getContentSize().height* zombie->getScaleY() * 0.52));
		rightArm->setFlipX(false);
		rightArm->setAnchorPoint(ccp(1, 1));
		rightArm->setPosition(
				ccp(zombie->getPosition().x - zombie->getContentSize().width* zombie->getScaleX() / 2 * 0.1,
						zombie->getPosition().y + zombie->getContentSize().height* zombie->getScaleY() * 0.52));
		pan->setPosition(ccp(rightArm->getContentSize().width*0.4f, -rightArm->getContentSize().height*0.1f));
		pan->setAnchorPoint(ccp(1,0));
		pan->setFlipX(false);
		light->setAnchorPoint(ccp(0.5f,0.5f));
		light->setPosition(
				ccp(pan->getContentSize().width*pan->getScaleX()*0.2f,pan->getContentSize().height*pan->getScaleX()*0.7f));
		light->setFlipX(false);

	}
	if (state == _MOVE) {
		if (isRight) {

			zombie->setPositionX(dt * (moveSpeed - vCharacter) + zombie->getPositionX());
			leftArm->setPositionX(dt * (moveSpeed - vCharacter) + leftArm->getPositionX());
			rightArm->setPositionX(dt * (moveSpeed - vCharacter) + rightArm->getPositionX());
		} else {

			zombie->setPositionX(zombie->getPositionX() - dt * (moveSpeed + vCharacter));
			leftArm->setPositionX(leftArm->getPositionX() - dt * (moveSpeed + vCharacter));
			rightArm->setPositionX(rightArm->getPositionX() - dt * (moveSpeed + vCharacter));
		}
	}
	if (!holdMove) {
		if (!isRight) {
			rightArm->setRotation(-20);
			leftArm->setRotation(20);

		} else {
			rightArm->setRotation(0);
			leftArm->setRotation(0);
		}
		zombie->stopAllActions();
		leftArm->stopAllActions();
		rightArm->stopAllActions();
		zombie->runAction(zombieWalk);
		leftArm->runAction(rotLeftArmZombie);
		rightArm->runAction(rotRightArmZombie);
		holdMove = true;

	}

}
void Zombie::updateState(float dt, int vCharater) {
	switch (state) {
	case _WAIT:
		zombie->setVisible(false);
		leftArm->setVisible(false);
		rightArm->setVisible(false);
//		zombie->setPosition(pos);
		break;
	case _APPEAR:
		zombie->setVisible(false);
		leftArm->setVisible(false);
		rightArm->setVisible(false);

		app->setVisible(true);
		updateMove(dt, vCharater);
		if (!holdAppear) {
			holdAppear = true;
			app->runAction(appAct);
		}

		break;
	case _MOVE:
		updateMove(dt, vCharater);
		zombie->setVisible(true);
		leftArm->setVisible(true);
		rightArm->setVisible(true);

		break;
	case _DEAD:
		dead();
		break;
	case _HIT:
		if (!holdHit) {
			zombie->stopAllActions();
			leftArm->setVisible(false);
			rightArm->stopAllActions();
			zombie->runAction(zombieHitAct);
			if (isRight) {
				rightArm->runAction(rotRightArmHitAct);
			} else {
				rightArm->runAction(rotRightArmHitActL);
			}
			holdHit = true;
		}
		break;
	case _BITE:
		break;
	case _isATTACKED:
		pan->setVisible(false);
		if (!holdMoveAttacked) {
			zombie->stopAllActions();
			zombie->runAction(animateisAttackedGun);
			if (mov) {
				if (!isRight) {
					zombie->runAction(movRight);
					leftArm->runAction((CCFiniteTimeAction*) (movRight->copy()->autorelease()));
					rightArm->runAction((CCFiniteTimeAction*) (movRight->copy()->autorelease()));
				} else {
					zombie->runAction(movLeft);
					leftArm->runAction((CCFiniteTimeAction*) (movLeft->copy()->autorelease()));
					rightArm->runAction((CCFiniteTimeAction*) (movLeft->copy()->autorelease()));
				}
				mov = false;
			}
			holdMoveAttacked = true;
			CCFiniteTimeAction* action = CCSequence::create(CCDelayTime::create(0.1),
					CCCallFuncN::create(this, callfuncN_selector(Zombie::remove_hold_move)), NULL);
			this->runAction(action);

//			//blood
//			blood1 = CCSprite::createWithSpriteFrameName("blood01.png");
////			blood1->runAction(
////					CCSequence::create(CCDelayTime::create(2.0f),
////							CCCallFuncN::create(this, callfuncN_selector(Zombie::remove_head)), NULL));
//			if (isRight) {
//				blood1->setFlipX(true);
//			} else {
//				blood1->setFlipX(false);
//			}
//			blood1->setPosition(
//					ccp(zombie->getPosition().x,zombie->getPosition().y + zombie->getContentSize().height * zombie->getScaleY()));
//			this->addChild(blood1, 10);
//
//			b2BodyDef ballBodyDef;
//			ballBodyDef.type = b2_dynamicBody;
//			ballBodyDef.position.Set(blood1->getPositionX() / PTM_RATIO, blood1->getPositionY() / PTM_RATIO);
//			ballBodyDef.userData = blood1;
//			b2Body* body = world->CreateBody(&ballBodyDef);
//			if (isRight) {
//				body->ApplyForce(b2Vec2(-200, 0), b2Vec2(0, 0));
//			} else {
//				body->ApplyForce(b2Vec2(200, 0), b2Vec2(0, 0));
//			}
//
//			b2PolygonShape box;
//			box.SetAsBox(blood1->getContentSize().width / 2.0f / PTM_RATIO, blood1->getContentSize().height / 2.0f / PTM_RATIO);
//			b2FixtureDef fixtureDef;
//			fixtureDef.shape = &box;
//			fixtureDef.density = 5.0f;
//			fixtureDef.friction = 0.4f;
//			fixtureDef.restitution = 0.1f;
//			body->CreateFixture(&fixtureDef);

//			CCParticleSystem* m_emitter;
//		    m_emitter = CCParticleSystemQuad::create("LavaFlow.plist");
//		    m_emitter->retain();
//			m_emitter->setPosition(zombie->getPosition());
//			this->addChild(m_emitter, 10);
//			m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("blood01.png"));

		}

		break;

	}

}

void Zombie::dead() {
	if (!holdDead) {
		SimpleAudioEngine::sharedEngine()->playEffect(
				CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/zombiedead.ogg"));
		zombie->stopAllActions();
		leftArm->stopAllActions();
		rightArm->stopAllActions();
		zombie->runAction(zombieDead);
		leftArm->setVisible(false);
		rightArm->setVisible(false);
		holdDead = true;
		hero_t->countGold = hero_t->countGold + gold;
		hero_t->countKill++;
		CCFiniteTimeAction* action = CCSequence::create(CCDelayTime::create(5.0f),
				CCCallFuncN::create(this, callfuncN_selector(Zombie::recall)), NULL);
		this->runAction(action);

		head = CCSprite::createWithSpriteFrameName("zombieHead.png");
		head->runAction(
				CCSequence::create(CCDelayTime::create(2.0f),
						CCCallFuncN::create(this, callfuncN_selector(Zombie::remove_head)), NULL));
		if (isRight) {
			head->setFlipX(true);
		} else {
			head->setFlipX(false);
		}
		head->setPosition(
				ccp(zombie->getPosition().x,zombie->getPosition().y + zombie->getContentSize().height * zombie->getScaleY()));
		this->addChild(head, 10);

		b2BodyDef ballBodyDef;
		ballBodyDef.type = b2_dynamicBody;
		ballBodyDef.position.Set(head->getPositionX() / PTM_RATIO, head->getPositionY() / PTM_RATIO);
		ballBodyDef.userData = head;
		b2Body* body = world->CreateBody(&ballBodyDef);
		if (isRight) {
			body->ApplyForce(b2Vec2(-200, 50), b2Vec2(100, 50));
		} else {
			body->ApplyForce(b2Vec2(200, 50), b2Vec2(50, 100));
		}

		b2PolygonShape box;
		box.SetAsBox(head->getContentSize().width / 2.0f / PTM_RATIO, head->getContentSize().height / 2.0f / PTM_RATIO);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = 5.0f;
		fixtureDef.friction = 0.4f;
		fixtureDef.restitution = 0.1f;
		body->CreateFixture(&fixtureDef);
	}

}
void Zombie::bite() {

}
float Zombie::getX() {
	return zombie->getPositionX();

}
void Zombie::onEnter() {
	CCLayer::onEnter();
//setFlipX();
//setDead();
//	state = _DEAD;
	scheduleUpdate();

}
void Zombie::onExit() {
	CCLayer::onExit();

}
void Zombie::update(float dt, Character* hero) {
	hero_t = hero;
	int vCharacter = hero->moveSpeed;
	int _damge = hero->damge;
	bool visibleC = hero->visible;
	float sizeoC = hero->body->getContentSize().width * hero->body->getScaleX();

	if (hp <= 0) {
		state = _DEAD;
	}
	if (hero->moveState == _STAND) {
		updateState(dt, 0);
	} else {
		updateState(dt, vCharacter);
	}

	if (state != _MOVE) {	//update after dead

		zombie->setPositionX(-dt * vCharacter + zombie->getPositionX());
		leftArm->setPositionX(-dt * vCharacter + leftArm->getPositionX());
		rightArm->setPositionX(-dt * vCharacter + rightArm->getPositionX());

	}
	app->setPositionX(-dt * vCharacter + app->getPositionX());
	if (appType == 1) {
		if (state == _WAIT
				&& (zombie->getPositionX() < screen.width / 2 + 300 * scale
						&& zombie->getPositionX() > screen.width / 2 - 300 * scale)) {
			state = _MOVE;
			holdHit = false;

		}
	} else if (appType == 2) {
		if (state == _WAIT
				&& (zombie->getPositionX() < screen.width / 2 + 200 * scale
						&& zombie->getPositionX() > screen.width / 2 - 10 * scale)) {
			state = _APPEAR;
			holdHit = false;

		}
	}
	if (state == _MOVE && !hero->isDead
			&& (zombie->getPositionX() < screen.width / 2.0 + sizeoC
					&& zombie->getPositionX() > screen.width / 2.0 - sizeoC && visibleC)) {
		state = _HIT;
		pan->setVisible(true);

	}
	if (state == _HIT
			&& (zombie->getPositionX() > screen.width / 2.0 + sizeoC
					|| zombie->getPositionX() < screen.width / 2.0 - sizeoC) && visibleC) {

		state = _MOVE;
		holdMove = false;
		holdHit = false;
		light->setVisible(false);
		pan->setVisible(false);
	}
	if (!hero->visible) {
		light->setVisible(false);
		pan->setVisible(false);
	}

	if (zombie->getPositionX() > screen.width / 2.0f && visibleC && !hero->isDead) {
		isRight = false;
	}
	if (zombie->getPositionX() < screen.width / 2.0f && visibleC && !hero->isDead) {
		isRight = true;
	}

	if ((isAttackedGun || isAttackedAxe) && state != _WAIT && state != _DEAD) {
		hp = hp - _damge;
		state = _isATTACKED;
		pan->setVisible(false);
		holdHit = false;
		if (isAttackedAxe) {
			mov = true;
		}
		isAttackedGun = false;
		isAttackedAxe = false;
	}
	if (hit) {
		if (!holdDamage) {
			hero->hp = hero->hp - damage;
			holdDamage = true;
			hero->isAttacked = true;
		}
	}
	for (b2Body *b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *data = (CCSprite *) b->GetUserData();
			if (!data->isVisible()) {
				world->DestroyBody(b);

			}

		}
	}

}
void Zombie::recall() {
	hp = _HP;
	state = _WAIT;
	holdMove = false;
	zombie->setPosition(pos);
	holdDead = false;
}

void Zombie::is_attacked(bool isGun) {

}
void Zombie::remove_hold_move() {

	holdMoveAttacked = false;
	state = _MOVE;
	holdMove = false;
}

void Zombie::getDamageHit() {
	light->setVisible(true);
	hit = true;
}

void Zombie::hold_hit() {
	state = _MOVE;
	holdHit = false;
	holdMove = false;
	light->setVisible(false);
	hit = false;
	holdDamage = false;
}

void Zombie::after_appear() {
	app->setVisible(false);
	zombie->setVisible(true);
	leftArm->setVisible(true);
	rightArm->setVisible(true);
	holdAppear = false;
	state = _MOVE;
}
void Zombie::remove_head() {
	head->setVisible(false);
}
