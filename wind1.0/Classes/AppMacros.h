#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

#define DESIGN_RESOLUTION_480X320    0
#define DESIGN_RESOLUTION_960X640    1
#define DESIGN_RESOLUTION_1024X768   2
#define DESIGN_RESOLUTION_2048X1536  3

#define TARGET_DESIGN_RESOLUTION_SIZE  DESIGN_RESOLUTION_960X640

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(480, 320),   "iphone" };
static Resource computerResource  =  { cocos2d::CCSizeMake(960, 640),   "computer" };
static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "ipad"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "ipadhd" };

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_480X320)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_960X640)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(960, 640);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_1024X768)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(1024, 768);
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_2048X1536)
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(2048, 1536);
#else
#error unknown target design resolution!
#endif

#define TITLE_FONT_SIZE  (cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)

#endif
