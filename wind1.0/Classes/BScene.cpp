#include "BScene.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;
using namespace cocos2d::extension;

CCScene* BScene::scene() {
	CCScene *scene = CCScene::create();
	BScene *layer = BScene::create();
	scene->addChild(layer);
	return scene;
}

bool BScene::init() {
	if (!CCLayer::init()) {
		return false;
	}
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//背景
	CCSprite* pSprite = CCSprite::create("bg.jpg");
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(pSprite, 0);
	//进入按钮
	Button* key4SpriteButton = Button::create("start1.png","start2.png","start2.png",Widget::TextureResType::LOCAL);
	key4SpriteButton->setPosition(Vec2(origin.x + visibleSize.width*23/96,origin.y + visibleSize.height*15/32));
	key4SpriteButton->addTouchEventListener(CC_CALLBACK_2(BScene::toGameLayer, this));
	this->addChild(key4SpriteButton, 1);
	//关闭按钮
	Button* pCloseButton = Button::create("exit1.png","exit2.png","exit2.png",Widget::TextureResType::LOCAL);
	pCloseButton->setPosition(Vec2(origin.x + visibleSize.width*7/32,origin.y + visibleSize.height*9/32));
	pCloseButton->addTouchEventListener(CC_CALLBACK_2(BScene::CloseCallback, this));
	this->addChild(pCloseButton, 1);
	//播放主界面背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/FrenzySpeed.mp3", true);
	//加载开场动画
	scheduleOnce(schedule_selector(BScene::loadstartAnimation), 0.5);
	return true;
}
//点击进入按钮回调
void BScene::toGameLayer(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED)
	{
		SimpleAudioEngine::getInstance()->playEffect("music/selectstart.mp3");
		SimpleAudioEngine::getInstance()->playEffect("music/cheer.mp3");
		startAnimationCon = CCSprite::create();
		startAnimationCon->setPosition(ccp(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
		this->addChild(startAnimationCon, 2);
		startAnimationCon->runAction(startAction);
	}
}
//预加载部分动画
void BScene::loadstartAnimation(float dt) {
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/perAnimation.plist","armature/perAnimation.png");

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation0.plist","armature/startAnimation0.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation1.plist","armature/startAnimation1.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation2.plist","armature/startAnimation2.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation3.plist","armature/startAnimation3.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation4.plist","armature/startAnimation4.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation5.plist","armature/startAnimation5.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation6.plist","armature/startAnimation6.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation7.plist","armature/startAnimation7.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/startAnimation8.plist","armature/startAnimation8.png");

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/sceneAnimation.plist","armature/sceneAnimation.png");

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightFirAnimation0.plist","armature/nightFirAnimation0.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightFirAnimation1.plist","armature/nightFirAnimation1.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightFirAnimation2.plist","armature/nightFirAnimation2.png");

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightSecAnimation0.plist","armature/nightSecAnimation0.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightSecAnimation1.plist","armature/nightSecAnimation1.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightSecAnimation2.plist","armature/nightSecAnimation2.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightSecAnimation3.plist","armature/nightSecAnimation3.png");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("armature/nightSecAnimation4.plist","armature/nightSecAnimation4.png");

	Vector<SpriteFrame*> startFrames;
	for (int i = 0; i < 99; i++) {
		char pngName[20];
		sprintf(pngName, "start%d.png",i+1);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pngName);
		startFrames.pushBack(frame);
	}
	CCAnimation*startAnimation = CCAnimation::createWithSpriteFrames(startFrames,0.03,1);
	CCAnimate*startAnimate = CCAnimate::create(startAnimation);
	CCCallFuncN*startCallback = CCCallFuncN::create(this,callfuncN_selector(BScene::startGame));
	startAction = (CCFiniteTimeAction*) CCSequence::create(startAnimate,startCallback,NULL);
	startAction->retain();
	startAnimationCon = NULL;
}
//移除开场动画
void BScene::startGame(CCNode *pSender) {
	if(startAnimationCon)
	{
		startAnimationCon->removeFromParent();
		startAnimationCon = NULL;
	}
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene *pScene = Game4Key::scene();
	pDirector->replaceScene(pScene);
}
//点击关闭按钮回调
void BScene::CloseCallback(Ref* sender, Widget::TouchEventType type) {
	CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
