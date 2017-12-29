#include "Game4Key.h"
#include "SimpleAudioEngine.h"
#include "BScene.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::extension;

CCScene* Game4Key::scene() {
	CCScene *scene = CCScene::create();
	Game4Key *layer = Game4Key::create();
	scene->addChild(layer);
	return scene;
}

bool Game4Key::init() {
	if (!CCLayer::init()) {
		return false;
	}
	m_index = 0;
	loadDataFromFile("rhythm.dat");//读取配置
	m_time = 0.0f;//初始化播放时间
	setTouchEnabled(true);
	setKeyboardEnabled(true);
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//点击区域和判定区域的两种宽度
	perKeyLW = visibleSize.width*55/192;
	perKeyMW = visibleSize.width*41/192;
	speed = 1;//音符下落的速度
	this->maxCombo = 0;//最大连击
	this->curCombo = 0;//当前连击
	this->score = 0;//分数
	this->perNumCount = 0;//音符计数，嘉仪专用
	//点击区域
	key1Rect = CCRectMake(origin.x,origin.y,perKeyLW,visibleSize.height/4);
	key2Rect = CCRectMake(origin.x+perKeyLW,origin.y,perKeyMW,visibleSize.height/4);
	key3Rect = CCRectMake(origin.x+visibleSize.width/2,origin.y,perKeyMW,visibleSize.height/4);
	key4Rect = CCRectMake(origin.x+visibleSize.width/2+perKeyMW,origin.y,perKeyLW,visibleSize.height/4);
	//场景
	pSprite = CCSprite::create("gamebg.jpg");
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(pSprite, 0);
	//黑夜模式场景
	nightSprite = CCSprite::create("gamenightbg.jpg");
	nightSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(nightSprite, -1);
	//滑道
	CCSprite* perbgSprite = CCSprite::create("perbg.png");
	perbgSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height*543/1280 + origin.y));
	this->addChild(perbgSprite, 3);
	//边线
	CCSprite* sidelineSprite = CCSprite::create("sideline.png");
	sidelineSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(sidelineSprite, 5);
	//关闭按钮
	Button* pReturnButton = Button::create("return.png","return.png","return.png",Widget::TextureResType::LOCAL);
	pReturnButton->setPosition(Vec2(origin.x + visibleSize.width-pReturnButton->getContentSize().width/2,origin.y + visibleSize.height-pReturnButton->getContentSize().height/2));
	pReturnButton->addTouchEventListener(CC_CALLBACK_2(Game4Key::toBSceneLayer, this));
	this->addChild(pReturnButton, 10);
	//顶点出生音符宽度
	float bornPerW = visibleSize.width/6;
	float leftX = visibleSize.width/2 - bornPerW/2;
	float bornPerH = visibleSize.height*101/128;
	//音符初始坐标
	bornNotePos1 = CCPointMake(origin.x + leftX + bornPerW/8,origin.y+bornPerH);
	bornNotePos2 = CCPointMake(origin.x + leftX + bornPerW*3/8,origin.y+bornPerH);
	bornNotePos3 = CCPointMake(origin.x + leftX + bornPerW*5/8,origin.y+bornPerH);
	bornNotePos4 = CCPointMake(origin.x + leftX + bornPerW*7/8,origin.y+bornPerH);
	//四个音符滑道绘制
	draw1 = CCDrawNode::create();
	draw2 = CCDrawNode::create();
	draw3 = CCDrawNode::create();
	draw4 = CCDrawNode::create();
	addChild(draw1, -4);
	addChild(draw2, -4);
	addChild(draw3, -4);
	addChild(draw4, -4);
	//四个滑道框
	star1[0] = ccp(origin.x+leftX,origin.y+bornPerH);
	star1[1] = ccp(origin.x+visibleSize.width*17/192,origin.y+visibleSize.height*25/128);
	star1[2] = ccp(origin.x+visibleSize.width*55/192,origin.y+visibleSize.height*25/128);
	star1[3] = ccp(origin.x+leftX+bornPerW*11/24,origin.y+bornPerH);
	star2[0] = ccp(origin.x+leftX+bornPerW*11/24,origin.y+bornPerH);
	star2[1] = ccp(origin.x+visibleSize.width*55/192,origin.y+visibleSize.height*25/128);
	star2[2] = ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*25/128);
	star2[3] = ccp(origin.x+visibleSize.width/2,origin.y+bornPerH);
	star3[0] = ccp(origin.x+visibleSize.width/2,origin.y+bornPerH);
	star3[1] = ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*25/128);
	star3[2] = ccp(origin.x+visibleSize.width*137/192,origin.y+visibleSize.height*25/128);
	star3[3] = ccp(origin.x+visibleSize.width/2+bornPerW/4,origin.y+bornPerH);
	star4[0] = ccp(origin.x+visibleSize.width/2+bornPerW/4,origin.y+bornPerH);
	star4[1] = ccp(origin.x+visibleSize.width*137/192,origin.y+visibleSize.height*25/128);
	star4[2] = ccp(origin.x+visibleSize.width*175/192,origin.y+visibleSize.height*25/128);
	star4[3] = ccp(origin.x+leftX+bornPerW,origin.y+bornPerH);
	//边框闪动
  	CCBlink* sidelineActin = CCBlink::create(1, 1);
  	sidelineSprite->runAction(CCRepeatForever::create(CCSequence::create(sidelineActin, NULL)));
	//按键触发
	bottomButton1_1 = CCSprite::create("key_2_1.png");
	bottomButton1_2 = CCSprite::create("key_2_2.png");
	bottomButton1_3 = CCSprite::create("key_2_3.png");
	bottomButton1_4 = CCSprite::create("key_2_4.png");
	bottomButton1_1->setPosition(ccp(origin.x+visibleSize.width*29/192,origin.y+visibleSize.height*9/64));
	this->addChild(bottomButton1_1, -6);
	bottomButton1_2->setPosition(ccp(origin.x+visibleSize.width*73/192,origin.y+visibleSize.height*9/64));
	this->addChild(bottomButton1_2, -6);
	bottomButton1_3->setPosition(ccp(origin.x+visibleSize.width*119/192,origin.y+visibleSize.height*9/64));
	this->addChild(bottomButton1_3, -6);
	bottomButton1_4->setPosition(ccp(origin.x+visibleSize.width*163/192,origin.y+visibleSize.height*9/64));
	this->addChild(bottomButton1_4, -6);
	//连击
	CCLabelAtlas *comble = CCLabelAtlas::create("", "curcomb.png", 29, 40, 48);
	comble->setAnchorPoint(Vec2(0.5, 0.5));
	comble->setPosition(ccp(origin.x+visibleSize.width/2, origin.y+visibleSize.height*171/256));
	addChild(comble, 10, 10);
	//分数
	CCLabelAtlas *scoreLable = CCLabelAtlas::create("","score.png", 21, 30, 48);
	scoreLable->setAnchorPoint(Vec2(0.5, 0.5));
	scoreLable->setPosition(ccp(origin.x+visibleSize.width*165/192, origin.y+visibleSize.height*123/128));
	addChild(scoreLable, 10, 11);
	//音符计数
// 	CCLabelAtlas *perLable = CCLabelAtlas::create("","score.png", 21, 30, 48);
// 	perLable->setPosition(ccp(origin.x+visibleSize.width*3/4, origin.y+visibleSize.height-scoreLable->getContentSize().height*4));
// 	addChild(perLable, 10, 12);
	//判定区域
	perfectRect = CCRectMake(origin.x,origin.y+visibleSize.height/16,visibleSize.width,visibleSize.height*5/32);
	greatRect = CCRectMake(origin.x,origin.y,visibleSize.width,visibleSize.height*5/16);
	missRect = CCRectMake(origin.x,origin.y,visibleSize.width,visibleSize.height*5/16);
	//背景音乐
	SimpleAudioEngine::getInstance()->playEffect("music/ready.wav");
 	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/gamebg.mp3");
	//判定特效
	Vector<SpriteFrame*> perFrames;
	for (int i = 0; i < 5; i++) {
		char pngName[20];
		sprintf(pngName, "per%d.png",i+1);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pngName);
		perFrames.pushBack(frame);
	}
	CCAnimation*perAnimation = CCAnimation::createWithSpriteFrames(perFrames,0.01,1);
	CCAnimate*perAnimate = CCAnimate::create(perAnimation);
	CCCallFuncN*perCallback = CCCallFuncN::create(this,callfuncN_selector(Game4Key::animationRemove));
	perAction = (CCFiniteTimeAction*) CCSequence::create(perAnimate,perCallback,NULL);
	perAction->retain();
	perAnimationCon1 = NULL;
	perAnimationCon2 = NULL;
	perAnimationCon3 = NULL;
	perAnimationCon4 = NULL;
	//场景特效
	Vector<SpriteFrame*> sceneFrames;
	for (int i = 0; i < 11; i++) {
		char pngName[20];
		sprintf(pngName, "scene%d.png",i+1);
		CCSpriteFrame *sceneframe = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pngName);
		sceneFrames.pushBack(sceneframe);
	}
	CCAnimation*sceneAnimation = CCAnimation::createWithSpriteFrames(sceneFrames,0.042,-1);
	CCAnimate*sceneSecAnimate = CCAnimate::create(sceneAnimation);
	CCCallFuncN*sceneCallback = CCCallFuncN::create(this,callfuncN_selector(Game4Key::sceneCallback));
	sceneAction = (CCFiniteTimeAction*) CCSequence::create(sceneSecAnimate,sceneCallback,NULL);
	sceneAction->retain();
	sceneAnimationCon = NULL;
	//延迟播放场景特效
	scheduleOnce(schedule_selector(Game4Key::playSceneAnimation), 3);
	//黑夜特效第一步
	Vector<SpriteFrame*> nightFirFrames;
	for (int i = 0; i < 35; i++) {
		char pngName[20];
		sprintf(pngName, "nightFir%d.png",i+1);
		CCSpriteFrame *nightfirframe = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pngName);
		nightFirFrames.pushBack(nightfirframe);
	}
	CCAnimation*nightFirAnimation = CCAnimation::createWithSpriteFrames(nightFirFrames,0.04,1);
	CCAnimate*nightFirAnimate = CCAnimate::create(nightFirAnimation);
	CCCallFuncN*nightFirCallback = CCCallFuncN::create(this,callfuncN_selector(Game4Key::nightFirCallback));
	nightFirAction = (CCFiniteTimeAction*) CCSequence::create(nightFirAnimate,nightFirCallback,NULL);
	nightFirAction->retain();
	nightFirAnimationCon = NULL;
	//黑夜特效第二步
	Vector<SpriteFrame*> nightSecFrames;
	for (int i = 0; i < 60; i++) {
		char pngName[20];
		sprintf(pngName, "nightSec%d.png",i+1);
		CCSpriteFrame *nightsecframe = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pngName);
		nightSecFrames.pushBack(nightsecframe);
	}
	CCAnimation*nightSecAnimation = CCAnimation::createWithSpriteFrames(nightSecFrames,0.05,4);
	CCAnimate*nightSecAnimate = CCAnimate::create(nightSecAnimation);
	CCCallFuncN*nightSecCallback = CCCallFuncN::create(this,callfuncN_selector(Game4Key::nightSecCallback));
	nightSecAction = (CCFiniteTimeAction*) CCSequence::create(nightSecAnimate,nightSecCallback,NULL);
	nightSecAction->retain();
	nightSecAnimationCon = NULL;
	schedule(schedule_selector(Game4Key::born), 0.01);
	return true;
}
//动作执行完后移除
void Game4Key::remove(CCNode *pSender) {
	if (pSender == NULL) {
		return;
	}
	CCSprite *sprite = (CCSprite *) pSender;
	sprite->removeFromParent();
	if (sprite->getTag() == 1 || sprite->getTag() == 2 || sprite->getTag() == 3 || sprite->getTag() == 4)
	{
		showCombo(3);
	}
}
//判定特效播放完后移除
void Game4Key::animationRemove(CCNode *pSender) {
	removePerAnimation(1);
	removePerAnimation(2);
	removePerAnimation(3);
	removePerAnimation(4);
}
//移除某个判定特效
void Game4Key::removePerAnimation(int touchKey) {
	switch (touchKey) {
	case 1:
		if(perAnimationCon1)
		{
			perAnimationCon1->removeFromParent();
			perAnimationCon1 = NULL;
		}
		break;
	case 2:
		if(perAnimationCon2)
		{
			perAnimationCon2->removeFromParent();
			perAnimationCon2 = NULL;
		}
		break;
	case 3:
		if(perAnimationCon3)
		{
			perAnimationCon3->removeFromParent();
			perAnimationCon3 = NULL;
		}
		break;
	case 4:
		if(perAnimationCon4)
		{
			perAnimationCon4->removeFromParent();
			perAnimationCon4 = NULL;
		}
		break;
	default:
		return;
	}
}
//播放场景特效
void Game4Key::playSceneAnimation(float dt) {
	sceneAnimationCon = CCSprite::create();
	sceneAnimationCon->setPosition(ccp(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(sceneAnimationCon, 1, 25);
	sceneAnimationCon->runAction(sceneAction);
}
//场景特效完成回调
void Game4Key::sceneCallback(CCNode *pSender) {
	if(sceneAnimationCon)
	{
		sceneAnimationCon->removeFromParent();
		sceneAnimationCon = NULL;
	}
}
//改变黑夜模式特效状态，播放或者停止
void Game4Key::playNightAnimation() {
	if(sceneAnimationCon)
	{
		sceneAnimationCon->removeFromParent();
		sceneAnimationCon = NULL;
	}
	reorderChild(pSprite, -1);
	reorderChild(nightSprite, 0);
	nightFirAnimationCon = CCSprite::create();
	nightFirAnimationCon->setPosition(ccp(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(nightFirAnimationCon, 7, 26);
	nightFirAnimationCon->runAction(nightFirAction);
}
//黑夜特效第一步完成回调
void Game4Key::nightFirCallback(CCNode *pSender) {
	if(nightFirAnimationCon)
	{
		nightFirAnimationCon->removeFromParent();
		nightFirAnimationCon = NULL;
	}
	nightSecAnimationCon = CCSprite::create();
	nightSecAnimationCon->setPosition(ccp(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(nightSecAnimationCon, 1, 27);
	nightSecAnimationCon->runAction(nightSecAction);
}
//黑夜特效第二步完成回调
void Game4Key::nightSecCallback(CCNode *pSender) {
	if(nightSecAnimationCon)
	{
		nightSecAnimationCon->removeFromParent();
		nightSecAnimationCon = NULL;
	}
	reorderChild(pSprite, 0);
	reorderChild(nightSprite, -1);
	playSceneAnimation(0);
}

//获取判定类型
int Game4Key::checkScore(CCNode *pNode,int touchKey) {
	int checkid = 0;
	if (pNode == NULL) {
		return checkid;
	}
	CCSprite *sprite = (CCSprite *) pNode;
	float s_y = sprite->getPosition().y;
	float s_perfect_maxy = perfectRect.getMaxY();
	float s_perfect_miny = perfectRect.getMinY();
	float s_great_maxy = greatRect.getMaxY();
	float s_great_miny = greatRect.getMinY();
	float s_miss_maxy = missRect.getMaxY();
	float s_miss_miny = missRect.getMinY();
	//perfect
	if (s_y >= s_perfect_miny && s_y <= s_perfect_maxy) {
		checkid = 1;
	}
	//great
	else if (s_y >= s_great_miny && s_y <= s_great_maxy) {
		checkid = 2;
	}
	//miss
	else if (s_y >= s_miss_miny && s_y <= s_miss_maxy) {
		checkid = 3;
	}
	if (checkid > 0)
	{
		sprite->removeFromParent();
		//判定特效
		if (touchKey > 0 && touchKey < 5)
		{
			removePerAnimation(touchKey);
			if (touchKey == 1)
			{
				perAnimationCon1 = CCSprite::create();
				perAnimationCon1->setPosition(ccp(origin.x+visibleSize.width*29/192, origin.y+visibleSize.height*9/64));
				this->addChild(perAnimationCon1, 9, 21);
				perAnimationCon1->runAction(perAction);
			}
			else if (touchKey == 2)
			{
				perAnimationCon2 = CCSprite::create();
				perAnimationCon2->setPosition(ccp(origin.x+visibleSize.width*73/192, origin.y+visibleSize.height*9/64));
				this->addChild(perAnimationCon2, 9, 22);
				perAnimationCon2->runAction(perAction);
			}
			else if (touchKey == 3)
			{
				perAnimationCon3 = CCSprite::create();
				perAnimationCon3->setPosition(ccp(origin.x+visibleSize.width*119/192, origin.y+visibleSize.height*9/64));
				this->addChild(perAnimationCon3, 9, 23);
				perAnimationCon3->runAction(perAction);
			}
			else if (touchKey == 4)
			{
				perAnimationCon4 = CCSprite::create();
				perAnimationCon4->setPosition(ccp(origin.x+visibleSize.width*163/192, origin.y+visibleSize.height*9/64));
				this->addChild(perAnimationCon4, 9, 24);
				perAnimationCon4->runAction(perAction);
			}
		}
	}
	return checkid;
}
//清空连击
void Game4Key::clearCurComb(CCNode *pLable) {
	if (pLable == NULL) {
		return;
	}
	CCLabelBMFont *lable = (CCLabelBMFont*) pLable;
	lable->setString("");
}
//根据判定类型更新连击分数
void Game4Key::showCombo(int id) {
	if (id == 0) {
		return;
	}
	CCSprite* s_p;
	if (id == 1) {
		s_p = CCSprite::create("perfect.png");
		score += 200;
	}
	if (id == 2) {
		s_p = CCSprite::create("great.png");
		score += 80;
	}
	curCombo ++;
	if (id == 3) {
		s_p = CCSprite::create("miss.png");
		this->curCombo = 0;
	}
	if (maxCombo < curCombo) {
		maxCombo++;
	}
	perNumCount ++;

	s_p->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+visibleSize.height*97/128));
	s_p->setScale(1);
	this->addChild(s_p, 10);
	CCCallFuncN* s_pCallBack = CCCallFuncN::create(this,callfuncN_selector(Game4Key::remove));
	CCAction* s_pAction = CCSequence::create(CCScaleTo::create(0.2, 0.1),s_pCallBack, NULL);
	s_p->runAction(s_pAction);
	CCLabelAtlas* curCombLable = (CCLabelAtlas*) getChildByTag(10);
	CCLabelAtlas* scoreLable = (CCLabelAtlas*) getChildByTag(11);
	char string[12] = { 0 };
	char strscore[12] = { 0 };
	if (curCombo > 0)
	{
		sprintf(string, "%d", curCombo);
		curCombLable->setString(string);
	}
	else
	{
		curCombLable->setString("");
	}
	sprintf(strscore, "%d", score);
	scoreLable->setString(strscore);

	if (perNumCount == 93 || perNumCount == 229)
	{
		playNightAnimation();
	}

// 	CCLabelAtlas* perLable = (CCLabelAtlas*) getChildByTag(12);
// 	char strper[12] = { 0 };
// 	sprintf(strper, "%d", perNumCount);
// 	perLable->setString(strper);
}
//音符生成
void Game4Key::born(float dt) {
	if(m_data.empty() || m_index >= m_data.size()){
		return ;
	}
	m_time += dt;
	int nowtime = int(m_time * 1000) - (int(m_time * 1000) % 10);
	while(m_index < m_data.size() && nowtime >= m_data[m_index].time)
	{
		bornKey = m_data[m_index].type;
		m_index++;
		CCCallFuncN* pCallback = CCCallFuncN::create(this,callfuncN_selector(Game4Key::remove));
		if (bornKey == 1) {
			CCSprite* musicNote_4_1 = CCSprite::create("key_1_1.png");
			musicNote_4_1->setPosition(bornNotePos1);
			musicNote_4_1->setScale(0.01f);
			this->addChild(musicNote_4_1, 8, 1);
			CCFiniteTimeAction* noteAction1 = (CCFiniteTimeAction*) CCSequence::create(CCSpawn::create(CCScaleTo::create(speed, 1),CCEaseSineIn::create(CCMoveTo::create(speed,ccp(origin.x+visibleSize.width*17/192,origin.y))),NULL),pCallback,NULL);
			musicNote_4_1->runAction(noteAction1);
		}
		if (bornKey == 2) {
			CCSprite* musicNote_4_2 = CCSprite::create("key_1_2.png");
			musicNote_4_2->setPosition(bornNotePos2);
			musicNote_4_2->setScale(0.01f);
			this->addChild(musicNote_4_2, 8, 2);
			CCFiniteTimeAction* noteAction2 = (CCFiniteTimeAction*) CCSequence::create(CCSpawn::create(CCScaleTo::create(speed, 1),CCEaseSineIn::create(CCMoveTo::create(speed,ccp(origin.x+visibleSize.width*23/64,origin.y))),NULL),pCallback,NULL);
			musicNote_4_2->runAction(noteAction2);
		}
		if (bornKey == 3) {
			CCSprite* musicNote_4_3 = CCSprite::create("key_1_3.png");
			musicNote_4_3->setPosition(bornNotePos3);
			musicNote_4_3->setScale(0.01f);
			this->addChild(musicNote_4_3, 8, 3);
			CCFiniteTimeAction* noteAction3 = (CCFiniteTimeAction*) CCSequence::create(CCSpawn::create(CCScaleTo::create(speed, 1),CCEaseSineIn::create(CCMoveTo::create(speed,ccp(origin.x+visibleSize.width*41/64,origin.y))),NULL),pCallback,NULL);
			musicNote_4_3->runAction(noteAction3);
		}
		if (bornKey == 4) {
			CCSprite* musicNote_4_4 = CCSprite::create("key_1_4.png");
			musicNote_4_4->setPosition(bornNotePos4);
			musicNote_4_4->setScale(0.01f);
			this->addChild(musicNote_4_4, 8, 4);
			CCFiniteTimeAction* noteAction4 = (CCFiniteTimeAction*) CCSequence::create(CCSpawn::create(CCScaleTo::create(speed, 1),CCEaseSineIn::create(CCMoveTo::create(speed,ccp(origin.x+visibleSize.width*175/192,origin.y))), NULL),pCallback,NULL);
			musicNote_4_4->runAction(noteAction4);
		}
	}
}
//获取点击区域
int Game4Key::containsTouchLocation(CCTouch* touch) {
	if (key1Rect.containsPoint(convertTouchToNodeSpace(touch))) {
		return 1;
	}
	if (key2Rect.containsPoint(convertTouchToNodeSpace(touch))) {
		return 2;
	}
	if (key3Rect.containsPoint(convertTouchToNodeSpace(touch))) {
		return 3;
	}
	if (key4Rect.containsPoint(convertTouchToNodeSpace(touch))) {
		return 4;
	}
	return 0;
}

//点击事件开始
void Game4Key::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event) {
	vector<Touch*>::const_iterator iter;
	Touch *touch;

	for (iter = touches.begin(); iter != touches.end(); ++iter) {
		touch = (Touch *) (*iter);
		int touchKey = Game4Key::containsTouchLocation(touch);
		if (touchKey == 0) {
			return;
		}
		switch (touchKey) {
		case 1:
			//整个滑道框点亮
			draw1->drawPolygon(star1, sizeof(star1) / sizeof(star1[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
			reorderChild(bottomButton1_1, 6);
			showCombo(checkScore(getChildByTag(1),touchKey));
			break;
		case 2:
			draw2->drawPolygon(star2, sizeof(star2) / sizeof(star2[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
			reorderChild(bottomButton1_2, 6);
			showCombo(checkScore(getChildByTag(2),touchKey));
			break;
		case 3:
			draw3->drawPolygon(star3, sizeof(star3) / sizeof(star3[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
			reorderChild(bottomButton1_3, 6);
			showCombo(checkScore(getChildByTag(3),touchKey));
			break;
		case 4:
			draw4->drawPolygon(star4, sizeof(star4) / sizeof(star4[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
			reorderChild(bottomButton1_4, 6);
			showCombo(checkScore(getChildByTag(4),touchKey));
			break;
		default:
			return;
		}
	}
}
//按键事件开始
void Game4Key::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_D:
		//整个滑道框点亮
		draw1->drawPolygon(star1, sizeof(star1) / sizeof(star1[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
		reorderChild(bottomButton1_1, 6);
		showCombo(checkScore(getChildByTag(1),1));
		break;
	case EventKeyboard::KeyCode::KEY_F:
		draw2->drawPolygon(star2, sizeof(star2) / sizeof(star2[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
		reorderChild(bottomButton1_2, 6);
		showCombo(checkScore(getChildByTag(2),2));
		break;
	case EventKeyboard::KeyCode::KEY_J:
		draw3->drawPolygon(star3, sizeof(star3) / sizeof(star3[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
		reorderChild(bottomButton1_3, 6);
		showCombo(checkScore(getChildByTag(3),3));
		break;
	case EventKeyboard::KeyCode::KEY_K:
		draw4->drawPolygon(star4, sizeof(star4) / sizeof(star4[0]),ccc4f(0.2, 0.8, 1, 0.5), 1, ccc4f(0, 0, 1, 255));
		reorderChild(bottomButton1_4, 6);
		showCombo(checkScore(getChildByTag(4),4));
		break;
	default:
		return;
	}
}
//点击事件结束
void Game4Key::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event) {
	vector<Touch*>::const_iterator iter;
	Touch *touch;

	for (iter = touches.begin(); iter != touches.end(); ++iter) {
		touch = (CCTouch *) (*iter);
		int touchKey = Game4Key::containsTouchLocation(touch);
		if (touchKey == 0) {
			return;
		}
		switch (touchKey) {
		case 1:
			draw1->clear();
			reorderChild(bottomButton1_1, -6);
			return;
		case 2:
			draw2->clear();
			reorderChild(bottomButton1_2, -6);
			return;
		case 3:
			draw3->clear();
			reorderChild(bottomButton1_3, -6);
			return;
		case 4:
			draw4->clear();
			reorderChild(bottomButton1_4, -6);
			return;
		default:
			return;
		}
	}
}
//按键事件结束
void Game4Key::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_D:
		draw1->clear();
		reorderChild(bottomButton1_1, -6);
		break;
	case EventKeyboard::KeyCode::KEY_F:
		draw2->clear();
		reorderChild(bottomButton1_2, -6);
		break;
	case EventKeyboard::KeyCode::KEY_J:
		draw3->clear();
		reorderChild(bottomButton1_3, -6);
		break;
	case EventKeyboard::KeyCode::KEY_K:
		draw4->clear();
		reorderChild(bottomButton1_4, -6);
		break;
	default:
		return;
	}
}
//移动事件
void Game4Key::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event) {
	draw1->clear();
	reorderChild(bottomButton1_1, -6);
	draw2->clear();
	reorderChild(bottomButton1_2, -6);
	draw3->clear();
	reorderChild(bottomButton1_3, -6);
	draw4->clear();
	reorderChild(bottomButton1_4, -6);
}

//返回主界面回调
void Game4Key::toBSceneLayer(Ref* sender, Widget::TouchEventType type) {
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCScene *pScene = BScene::scene();
	CCTransitionScene * scene = CCTransitionFade::create(1, pScene);
	pDirector->replaceScene(scene);
}
//读取配置
bool Game4Key::loadDataFromFile(const char * fileName)
{
	ifstream dataFile;
	char num[32];
	m_data.clear();
	dataFile.open(fileName);
	if(!dataFile.is_open())
		return false;
	KeyInfo keyinfo;
	std::string strInfo;
	int index = 0;
	while (!dataFile.eof())
	{
		dataFile.getline(num,16,'\n');
		strInfo = std::string(num);
		index = strInfo.find_first_of(',');
		keyinfo.time = atoi(strInfo.substr(0,index).c_str());
		keyinfo.type = atoi(strInfo.substr(index+1).c_str());
		m_data.push_back(keyinfo);		
	}
	dataFile.close();
	return true;
}