#include "AppDelegate.h"

#include <vector>
#include <string>

#include "BScene.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    Director* pDirector = Director::getInstance();
    GLView* pEGLView = pDirector->getOpenGLView();
	if(!pEGLView) {
		pEGLView = GLView::create("wind");
		pDirector->setOpenGLView(pEGLView);
	}
    pDirector->setOpenGLView(pEGLView);
    //ÉèÖÃ·Ö±æÂÊ
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
	CCSize frameSize = pEGLView->getFrameSize();

    pDirector->setDisplayStats(false);

    pDirector->setAnimationInterval(1.0 / 60);

    CCScene *pScene = BScene::scene();

    pDirector->runWithScene(pScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
	CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
