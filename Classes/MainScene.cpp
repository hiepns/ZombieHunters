#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "Box2D/Box2D.h"
using namespace cocos2d;
using namespace CocosDenshion;
#define PTM_RATIO 32

MainScene::MainScene(bool _mute) {

	CCScene::init();
	screen = CCDirector::sharedDirector()->getWinSize();
	pc = screen.width / 2.0f;
	holdStand = false;
	holdRight = false;
	holdLeft = false;
	holdDead = false;
	gunType = 2;
	heroMoveSpeed = _characterMoveSpeed;

	mute = _mute;

	cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("controlbar.plist", "controlbar.png");

	//init physics world ******************************************************
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	world = new b2World(gravity);

	// Do we want to let bodies sleep?
	world->SetAllowSleeping(true);

	world->SetContinuousPhysics(true);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2EdgeShape groundBox;

	// bottom
	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO),
			b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO),
			b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

//	// left
//	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO),
//			b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO));
//	groundBody->CreateFixture(&groundBox, 0);
//
//	// right
//	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO),
//			b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
//	groundBody->CreateFixture(&groundBox, 0);
//end init physics world

	//World
	World = new WorldLayer();
	addChild(World, 1);
	scale = World->scale;
	//ControlBar
	Control = new ControlBar(scale);
	addChild(Control, 10);
	//hide place
//	hideplace = new HidePlace(3, scale, World->sizeWay / 3.0f);
//	addChild(hideplace, 2);
//	hideplace->setPosition(ccp(0,0));
//	hideplace->setAnchorPoint(ccp(0,0));
	//Character
	hero = new Character(scale, World->sizeWay / 3, mute, world);
	addChild(hero, 3);
	velocity = _characterMoveSpeed / 32.0f;
	levelGenerator(1);



	scheduleUpdate();

}
void MainScene::onEnter() {
	CCScene::onEnter();

	preload_music();
	if (!mute) {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
				CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/playbg.ogg"), true);
	}

}
void MainScene::onExit() {
	CCScene::onExit();

}
void MainScene::gameLoop(float dt) {

	if (hero->visible && hero->attackState == _SHOOT && !hero->holdAttackState && !hero->isDead) {
		hero->attackState = _NOATTACK;
		int nearest = getNearestZombie(hero->isRight);
		if (nearest != -1) {
			Zombie* zom1 = (Zombie*) ar_zombie->objectAtIndex(nearest);
			zom1->isAttackedGun = true;
		}
//		CCLog("%d", nearest);

	}

//cut*************************

	if (hero->attackState == _CUT && !hero->holdAttackState && hero->visible && !hero->isDead) {
		hero->attackState = _NOATTACK;
		for (int i = 0; i < ar_zombie->capacity(); i++) {
			Zombie* zom = (Zombie*) ar_zombie->objectAtIndex(i);
			if (!hero->isRight
					&& zom->getX() + zom->getContentSize().width * 0.5f * zom->zombie->getScaleX()
							< hero->rightArm->getPositionX()
									- hero->rightArm->getContentSize().width * hero->rightArm->getScaleX()
					&& zom->getX() + zom->getContentSize().width * 0.5f * zom->zombie->getScaleX()
							> hero->rightArm->getPositionX()
									- hero->axe->getContentSize().width * hero->axe->getScaleX()
									- hero->rightArm->getContentSize().width * hero->rightArm->getScaleX()) {
				zom->isAttackedAxe = true;

			}
			if (hero->isRight
					&& zom->getX() - zom->getContentSize().width * 0.5f * zom->zombie->getScaleX()
							> hero->rightArm->getPositionX()
									+ hero->rightArm->getContentSize().width * hero->rightArm->getScaleX()
					&& zom->getX() - zom->getContentSize().width * 0.5f * zom->zombie->getScaleX()
							< hero->rightArm->getPositionX()
									+ hero->axe->getContentSize().width * hero->axe->getScaleX()
									+ hero->rightArm->getContentSize().width * hero->rightArm->getScaleX()) {
				zom->isAttackedAxe = true;
			}
		}
	}
	//**********************************

}
void MainScene::update(float dt) {
	if (!hero->isDead) {
		World->update(dt, hero->moveState, velocity);
	}
	gameLoop(dt);
	hero->update(dt, Control->moveState, _characterMoveSpeed, Control->attackState, Control->gunType);
	for (int i = 0; i < ar_zombie->capacity(); i++) {
		Zombie* zom = (Zombie*) ar_zombie->objectAtIndex(i);
		zom->update(dt, hero);
	}

	world->Step(dt, 10, 10);
	for (b2Body *b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *ballData = (CCSprite *) b->GetUserData();
			ballData->setPosition(ccp((b->GetPosition().x )* PTM_RATIO,
					b->GetPosition().y * PTM_RATIO));

			ballData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

	//update hide place

	CCObject* obj;
	CCARRAY_FOREACH(ar_hide,obj) {
		HidePlace* hide = (HidePlace*) obj;
		hide->update(dt, hero->moveSpeed, hero->isDead, hero->body->getContentSize().width * hero->body->getScaleX());
		if (!hide->isEmpty) {
			if (hide->callMove) {
//				hero->setVisible(false);
				hero->callMove = false;
				hero->holdMoveApp = false;
				hero->visible = false;

				if (hero->addItem) {
					if (hide->item == 1) {
						hero->countCart1 = hero->countCart1 + 20;
						hide->item = 0;
					} else if (hide->item == 2) {
						hero->countCart2 = hero->countCart2 + 20;
						hide->item = 0;
					} else if (hide->item == 3) {
						hero->countGold = hero->countGold + 20;
						hide->item = 0;
					} else if (hide->item == 4) {

					}
					hero->addItem = false;
				}
			}

		} else if (hide->isEmpty && !hide->holdAct && hide->isVisible) {
			if (!hide->callMove) {
//				hero->setVisible(true);
				hero->callMove = true;
				hero->holdMoveHide = false;
				hero->visible = true;
			}

		}

	}

	Control->update(dt, hero->countCart1, hero->countCart2, hero->countGold, hero->countKill, hero->level);

	if (hero->isDead && !holdDead) {
		CCLayer* deadLayer = CCLayerColor::create(ccc4(0, 0, 0, 120));
		CCSprite* dead = CCSprite::createWithSpriteFrameName("gameover.png");
		dead->setScale(scale);
		dead->setOpacity(0);
		dead->runAction(CCFadeIn::create(5.0f));
		deadLayer->addChild(dead, 10);
		dead->setPosition(ccp(screen.width*0.5f,screen.height*0.5f));
		this->addChild(deadLayer, 10);
		Control->setVisible(false);
		Control->active = false;
		holdDead = true;

	}

}
void MainScene::levelGenerator(int _level) {
	if (_level == 1) {
		//Zombie
		//	CCArray* ar_Pos = CCArray::createWithCapacity(3);
		ar_zombie = CCArray::createWithCapacity(10);
		ar_zombie->retain();
		for (int i = 0; i < ar_zombie->capacity(); i++) {
			Zombie* zombie = new Zombie(scale, ccp(800*i, World->sizeWay / 3),2,world);
			addChild(zombie, 4);
			ar_zombie->addObject(zombie);
		}
		Zombie* zom = (Zombie*) ar_zombie->objectAtIndex(3);
		zom->isRight = false;
//		ar_hide = CCArray::createWithCapacity(2);
//		ar_hide->retain();
		ar_hide = CCArray::createWithCapacity(20);
		ar_hide->retain();
		for (int i = 0; i < ar_hide->capacity(); i++) {
			int j = rand() % 3 + 1;
			HidePlace* hideplace = new HidePlace(i % 3 + 1, scale, ccp(1000*i*scale,World->sizeWay / 2.0f),j);
			addChild(hideplace, 2);
			ar_hide->addObject(hideplace);
		}

	}
}

int MainScene::getNearestZombie(bool _isRight) {
	int nearest = -1;

//	CCArray* ar = CCArray::createWithCapacity(ar_zombie->capacity());
	CCObject* obj;
	float minposLeft = -1000;
	float minposRight = 1000;
	int i = 0;
	CCARRAY_FOREACH(ar_zombie,obj) {
		Zombie* zom = (Zombie*) obj;
		if (_isRight) {

			if (zom->getX() > pc) {
				if (zom->getX() < minposRight && zom->state != _DEAD && zom->state != _WAIT) {
					minposRight = zom->getX();
					nearest = i;
				}
			}
		} else {
			if (zom->getX() < pc) {
				if (zom->getX() > minposLeft && zom->state != _DEAD && zom->state != _WAIT) {
					minposLeft = zom->getX();
					nearest = i;
				}
			}
		}
		i++;
	}
	return nearest;
}

void MainScene::preload_music() {
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/playbg.ogg"));

	SimpleAudioEngine::sharedEngine()->preloadEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/gun1.ogg"));
	SimpleAudioEngine::sharedEngine()->preloadEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/gun2.ogg"));
	SimpleAudioEngine::sharedEngine()->preloadEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/axe.ogg"));
	SimpleAudioEngine::sharedEngine()->preloadEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/dead.ogg"));
	SimpleAudioEngine::sharedEngine()->preloadEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/zombiedead.ogg"));
	SimpleAudioEngine::sharedEngine()->preloadEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/opendoor.ogg"));
}
//void MainScene::get_cartridge() {
//	CCSprite* head = CCSprite::create("cartridge2.png");
//	head->setPosition(ccp(300,300));
//	this->addChild(head, 10);
//
//	b2BodyDef ballBodyDef;
//	ballBodyDef.type = b2_dynamicBody;
//	ballBodyDef.position.Set(head->getPositionX() / PTM_RATIO, head->getPositionY() / PTM_RATIO);
//	ballBodyDef.userData = head;
//	b2Body* body = world->CreateBody(&ballBodyDef);
//	body->ApplyForce(b2Vec2(-10, -10), b2Vec2(10, 100));
//
//	b2PolygonShape box;
//	box.SetAsBox(head->getContentSize().width / 2.0f / PTM_RATIO, head->getContentSize().height / 2.0f / PTM_RATIO);
//	b2FixtureDef fixtureDef;
//	fixtureDef.shape = &box;
//	fixtureDef.density = 1.0f;
//	fixtureDef.friction = 0.4f;
//	fixtureDef.restitution = 0.1f;
//	body->CreateFixture(&fixtureDef);
//}
