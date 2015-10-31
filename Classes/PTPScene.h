#ifndef __PTP_SCENE_H__
#define __PTP_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"

#define up 28
#define down 29
#define left 26
#define right 27
#define enter 35

#define white 1
#define black 2
#define whiteChess true
#define blackChess false

class PTPScene : public cocos2d::Layer
{
private:
	cocos2d::Node *PTPNode;
	cocos2d::ui::Button *back2, *cancle, *ok, *restart;
	cocos2d::Sprite *chessBoard, *point, *backupSprite;
	cocos2d::ui::Text *text_blackChess, *text_whiteChess;
	struct posi{
		int x, y;
	};
	posi position, backupPosition;
	int state[17][17];
	bool whichChess, cancleOnce, end;
	float duration;
public:
	virtual bool init();
	bool         PTPScene::checkGame();
	static       cocos2d::Scene* createScene();
	void         PTPScene::okCallback(cocos2d::Ref* pSender);
	void         PTPScene::back2Callback(cocos2d::Ref* pSender);
	void         PTPScene::cancleCallback(cocos2d::Ref* pSender);
	void         PTPScene::restartCallback(cocos2d::Ref* pSender);
	void         PTPScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	CREATE_FUNC(PTPScene);
};

#endif