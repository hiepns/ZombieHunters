#ifndef __CHARATER__
#define __CHARACRER__
#include "cocos2d.h"
#include "ControlBar.h"
#include "Box2D/Box2D.h"
using namespace cocos2d;
class Character: public CCLayer {
public:
	~Character();
	Character(float _scale, float _pos, bool _mute, b2World* _world);
	void update(float dt, int _moveState, int _moveSpeed, int _attackState, int _gunType);
	virtual void onEnter();
	virtual void onExit();
	void stand_state();
	void move_state();
	void attack_state(int _attackType);
	void updateMoveState(float dt, int _moveState);
	void updateAttackState(float dt, int _attackState);
	void dirBody(bool _isRight); //direction of Body

	void hold_fire();
	void hold_cut();
	void getDamageCut();
	void getDamageShoot();
	void hideAction();
	void call_update();
	void initAction();
	void call_item();
	void hold_attacked();
	void remove_cartridge();

	CCSprite* body;
	CCSprite* head;
	CCSprite* leftArm;
	CCSprite* rightArm;
	CCSprite* gun;
	CCSprite* gun2;
	CCSprite* axe;
	CCSprite* light;
	CCSprite* light2;
	CCSprite* hp1;
	CCSprite* hp2;

	CCSpriteFrameCache* cache;

	CCFiniteTimeAction* rotRightArmShootLAct;
	CCActionInterval* rotRightArmShootL;
	CCActionInterval* rotRightArmShootBackL;

	CCRepeatForever* rotGun2Act;
	CCActionInterval* rotGun2;
	CCActionInterval* rotGun2Back;

	CCRepeatForever* rotGun2ActL;
	CCActionInterval* rotGun2L;
	CCActionInterval* rotGun2BackL;

	CCRepeatForever* rotGun2ActS;
	CCActionInterval* rotGun2S;
	CCActionInterval* rotGun2BackS;

	CCRepeatForever* rotGun2ActSL;
	CCActionInterval* rotGun2SL;
	CCActionInterval* rotGun2BackSL;

	CCFiniteTimeAction* rotGun2ShootAct;
	CCActionInterval* rotGun2Shoot;
	CCActionInterval* rotGun2ShootBack;

	CCFiniteTimeAction* rotGun2ShootActL;
	CCActionInterval* rotGun2ShootL;
	CCActionInterval* rotGun2ShootBackL;

	CCFiniteTimeAction* rotRightArmShootAct;
	CCActionInterval* rotRightArmShoot;
	CCActionInterval* rotRightArmShootBack;

	CCFiniteTimeAction* rotRightArmCutActL;
	CCActionInterval* rotRightArmCutL;
	CCActionInterval* rotRightArmCutBackL;
	CCActionInterval* rotRightArmCutL1;
	CCActionInterval* rotRightArmCutBackL1;

	CCFiniteTimeAction* rotRightArmCutAct;
	CCActionInterval* rotRightArmCut;
	CCActionInterval* rotRightArmCutBack;
	CCActionInterval* rotRightArmCut1;
	CCActionInterval* rotRightArmCutBack1;

	CCAnimate* animbodyShootAct;
	CCAnimation* animbodyShoot;

	CCAnimate* deadAct;
	CCAnimation* animDead;

	CCAnimate* animbodyCutAct;
	CCAnimation* animbodyCut;

	CCAnimate* animAttackedAct;
	CCAnimation* animAttacked;

	CCRepeatForever* animlightAct;
	CCAnimation* animlight;

	CCRepeatForever* animlightAct2;
	CCAnimation* animlight2;

	CCRepeatForever* movLeftArmStandAct;
	CCActionInterval* movLeftArmStand;
	CCActionInterval* movLeftArmStandBack;

	CCRepeatForever* rotLeftArmMoveAct;
	CCActionInterval* rotLeftArmMove;
	CCActionInterval* rotLeftArmMoveBack;

	CCRepeatForever* rotRightArmMoveAct;
	CCActionInterval* rotRightArmMove;
	CCActionInterval* rotRightArmMoveBack;

	CCRepeatForever* animbodyStandAct;
	CCAnimation* animbodyStand;
	CCRepeatForever* movHeadStandAct;
	CCActionInterval* movHeadStand;
	CCActionInterval* movHeadStandBack;

	CCRepeatForever* movRightArmStandAct;
	CCActionInterval* movRightArmStand;
	CCActionInterval* movRightArmStandBack;

	CCRepeatForever* animBodyMoveAct;
	CCAnimation *animBodyMove;

	CCRepeatForever* animHeadMoveAct;
	CCAnimation *animHeadMove;

	CCDelayTime* delay_func;

	CCActionInterval* zoomin;
	CCActionInterval* movbody;
	CCActionInterval* movbodyBack;
	CCFiniteTimeAction* movbodyAct;

	float scale;
	float pos;
	bool isRight;
//	bool shooting;
//	bool holdStand;
//	bool holdLeft;
//	bool holdRight;
	bool holdShoot;
	bool holdCut;
	bool holdMoveState;
	bool holdAttackState;
	bool holdAttacked;
	bool isDead;
	bool holdMoveHide;
	bool holdMoveApp;
	bool holdMoveApp2;
	bool holdDead;

	bool visible;
	bool isAttacked;
	bool isGun2;
	bool callMove;

	bool addItem;
	bool updateRA;
	bool mute;

	//bool attackStateChange;
	int moveState;
	int lastMoveState;
	int tempMoveState;

	int attackState;
	int lastAttackState;
	int tempAttackState;

//	int state;
	int damge;
	int attackSpeed;
	float delay;
	float moveSpeed;
	int gunType;
	int hp;
	int countCart1;
	int countCart2;
	int countGold;
	int countKill;
	int level;
	bool isGun; //check current state
	b2World* world;
	CCArray* ar_cartridge;

};

static const char s_headxy[] = "images/character/headxy.png";
static const char s_torsoxy[] = "images/character/torsoxy.png";
static const char s_leftArmxy[] = "images/character/leftArmxy.png";
static const char s_rightArmxy[] = "images/character/rightArmxy.png";
static const char s_leftFootxy[] = "images/character/leftFootxy.png";
static const char s_rightFootxy[] = "images/character/rightFootxy.png";
static const char s_walk[] = "images/character/walk.png";
//static const int tagBody = 3;
//static const int tagLeftArm = 1;
//static const int tagHead = 2;
enum {
	tagLeftArm, tagHead, tagBody
};

static const float _characterMoveSpeed = 170;
static const float _HPC = 1000;
#endif
