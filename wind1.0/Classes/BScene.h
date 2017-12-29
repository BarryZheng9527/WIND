#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Game4Key.h"
#include "cocos-ext.h"
#include "cocostudio/CCArmatureDefine.h"
#include "cocostudio/CCBone.h"
#include "cocostudio/CCBatchNode.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "cocostudio/CCSpriteFrameCacheHelper.h"
#include "cocostudio/CCArmatureDataManager.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

class BScene : public cocos2d::CCLayer
{
private:
	CCSize visibleSize;//��ʾ����
	CCPoint origin;//Դ��
public:
	CCFiniteTimeAction*startAction; //������������
	CCSprite*startAnimationCon; //������������

    virtual bool init();  

    static cocos2d::CCScene* scene();

	void loadstartAnimation(float dt);
	void startGame(CCNode *pSender);
    void toGameLayer(Ref* sender, Widget::TouchEventType type);
	void CloseCallback(Ref* sender, Widget::TouchEventType type);

    CREATE_FUNC(BScene);
};

#endif
