#ifndef GAME4KEY_H_
#define GAME4KEY_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "VisibleRect.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;
typedef struct
{
	int		time;
	int		type;
	int		longpressmark;
}KeyInfo;
typedef std::vector<KeyInfo> GameData;

class Game4Key: public cocos2d::CCLayer {
private:
	CCSize visibleSize;//显示区域
	CCPoint origin;//源点
	float perKeyLW;//点击区域和判定区域的两种宽度
	float perKeyMW;
	int speed;//下落速度
protected:
	int m_index; //读取配置下标
	float m_time;//音乐已播放时间
	//四个音符滑道绘制
	CCDrawNode *draw1;
	CCDrawNode *draw2;
	CCDrawNode *draw3;
	CCDrawNode *draw4;
	//按键触发显示
	CCSprite* bottomButton1_1;
	CCSprite* bottomButton1_2;
	CCSprite* bottomButton1_3;
	CCSprite* bottomButton1_4;
	GameData m_data;//生成时机
public:
	int maxCombo;//最大连击
	int curCombo;//当前连击
	int score;//分数
	int perNumCount;//音符计数，嘉仪专用
	int polyploid;//当前积分倍数
	//四个按键的按下状态
	bool perPressDown1;
	bool perPressDown2;
	bool perPressDown3;
	bool perPressDown4;
	//点击区域
	CCRect key1Rect;
	CCRect key2Rect;
	CCRect key3Rect;
	CCRect key4Rect;
	//音符初始坐标
	CCPoint bornNotePos1;
	CCPoint bornNotePos2;
	CCPoint bornNotePos3;
	CCPoint bornNotePos4;
	//两层场景
	CCSprite* pSprite;
	CCSprite* nightSprite;
	//四个滑道框
	CCPoint star1[4];
	CCPoint star2[4];
	CCPoint star3[4];
	CCPoint star4[4];
	int bornKey;//音符初始类型，1,2,3,4
	//判定监听范围
	CCRect perfectRect;
	CCRect greatRect;
	CCRect missRect;
	int perfectNum; //perfect数量
	int greatNum; //great数量
	int missNum; //miss数量
	//判定特效动作
	CCFiniteTimeAction* perAction1;
	CCFiniteTimeAction* perAction2;
	CCFiniteTimeAction* perAction3;
	CCFiniteTimeAction* perAction4;
	//判定特效容器
	CCSprite*perAnimationCon1;
	CCSprite*perAnimationCon2;
	CCSprite*perAnimationCon3;
	CCSprite*perAnimationCon4;
	CCFiniteTimeAction* sceneAction; //场景特效动作
	CCSprite*sceneAnimationCon; //场景特效容器
	CCFiniteTimeAction* nightFirAction; //黑夜特效第一步动作
	CCSprite*nightFirAnimationCon; //黑夜特效第一步容器
	CCFiniteTimeAction* nightSecAction; //黑夜特效第二步动作
	CCSprite*nightSecAnimationCon; //黑夜特效第二步容器
	CCFiniteTimeAction* thunderAction; //闪电特效动作
	CCSprite*thunderAnimationCon; //闪电特效容器
	CCSprite*doubleScore; //双倍积分提示
	CCSprite*tripleScore; //三倍积分提示
	CCSprite*quadrupleScore; //四倍积分提示

	virtual bool init();
	static cocos2d::CCScene* scene();

	void remove(CCNode *pSender);
	void animationRemove1(CCNode *pSender);
	void animationRemove2(CCNode *pSender);
	void animationRemove3(CCNode *pSender);
	void animationRemove4(CCNode *pSender);
	void removePerAnimation(int touchKey);
	void playSceneAnimation(float dt);
	void sceneCallback(CCNode *pSender);
	void playNightAnimation();
	void nightFirCallback(CCNode *pSender);
	void nightSecCallback(CCNode *pSender);
	void thunderCallback(CCNode *pSender);
	void showPolyploid(int type, bool bShow);

	int checkScore(CCNode *pNode, int touchKey);
	void clearCurComb(CCNode *pLable);
	void showCombo(int id);
	void born(float dt);
	int containsTouchLocation(CCTouch* touch);
	void timeslice(float dt);

	CREATE_FUNC(Game4Key);
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void toBSceneLayer(Ref* sender, Widget::TouchEventType type);
	bool loadDataFromFile(const char * fileName);
};

#endif
