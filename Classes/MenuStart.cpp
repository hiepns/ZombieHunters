#include "MenuStart.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "Shop.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MenuStart::scene() {
	CCScene* scene = CCScene::create();
	scene->retain();
	MenuStart *layer = MenuStart::create();
//	layer->autorelease();
	scene->addChild(layer);
	return scene;
}

bool MenuStart::init() {
	if (!CCLayer::init()) {
		return false;
	}
	CCSize screen = CCDirector::sharedDirector()->getWinSize();
	setTouchEnabled(true);

	mute = false;

	bg = CCSprite::create("menustart/menubg.png");
	scaleY = screen.height / bg->getContentSize().height;
	scaleX = screen.width / bg->getContentSize().width;

	bg->setAnchorPoint(ccp(0,0));
	bg->setPosition(ccp(0,0));
	bg->setScaleY(scaleY);
	bg->setScaleX(scaleX);
	this->addChild(bg, 1);

	CCMenuItemImage* play = CCMenuItemImage::create("menustart/play1.png", "menustart/play2.png", this,
			menu_selector(MenuStart::play));
	play->setScale(scaleY);

	CCMenuItemImage* resume = CCMenuItemImage::create("menustart/exit1.png", "menustart/exit2.png", this,
			menu_selector(MenuStart::menuCloseCallback));
	resume->setScale(scaleY);

	CCMenuItemImage* shop = CCMenuItemImage::create("menustart/shop1.png", "menustart/shop2.png", this,
			menu_selector(MenuStart::shop));
	shop->setScale(scaleY);

	CCMenu* pMenu = CCMenu::create(play, shop, resume, NULL);

	pMenu->alignItemsVertically();
	pMenu->setPosition(ccp(screen.width*0.6f,screen.height*0.4f));
	this->addChild(pMenu, 1);

	CCMenuItemToggle *sound = CCMenuItemToggle::createWithTarget(this, menu_selector(MenuStart::sound_controler),
			CCMenuItemImage::create("menustart/sound_on.png", "menustart/sound_on2.png", this,
					menu_selector(MenuStart::sound_controler)),
			CCMenuItemImage::create("menustart/sound_off.png", "menustart/sound_off2.png", this,
					menu_selector(MenuStart::sound_controler)), NULL);
	sound->setScale(scaleY);

	CCMenuItemImage* info = CCMenuItemImage::create("menustart/info.png", "menustart/info2.png", this,
			menu_selector(MenuStart::sound_controler));
	info->setScale(scaleY);

	CCMenuItemImage* download = CCMenuItemImage::create("menustart/download.png", "menustart/download2.png", this,
			menu_selector(MenuStart::sound_controler));
	download->setScale(scaleY);
	CCMenuItemImage* contact = CCMenuItemImage::create("menustart/contact.png", "menustart/contact2.png", this,
			menu_selector(MenuStart::sound_controler));
	contact->setScale(scaleY);

	CCMenu* pMenu2 = CCMenu::create(sound, info, download, contact, NULL);

	pMenu2->alignItemsVerticallyWithPadding(0.0f);
	pMenu2->setPosition(ccp(screen.width*0.95f,screen.height*0.25f));
	this->addChild(pMenu2, 1);

	preload_music();

	return true;
}
void MenuStart::onEnter() {
	CCLayer::onEnter();
	if (!mute) {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
				CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/menubg.ogg"), true);
	}
}
void MenuStart::onExit() {
	CCLayer::onExit();

}
void MenuStart::menuCloseCallback(CCObject* pSender) {
	CCDirector::sharedDirector()->end();

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
}

void MenuStart::play() {
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/click.ogg"), false);
	MainScene* main = new MainScene(mute);
	CCDirector::sharedDirector()->replaceScene(main);

}
void MenuStart::shop() {
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/click.ogg"), false);
	CCScene* shop = Shop::scene();
	CCDirector::sharedDirector()->replaceScene(shop);
}

void MenuStart::preload_music() {
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/menubg.ogg"));

	SimpleAudioEngine::sharedEngine()->preloadEffect(
			CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("sound/click.ogg"));
}
void MenuStart::sound_controler() {

}
