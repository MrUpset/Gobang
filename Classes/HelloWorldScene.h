#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PTCScene.h"
#include "PTPScene.h"

class HelloWorld : public cocos2d::Layer
{
private:
	cocos2d::Node *LoginNode;
	cocos2d::ui::Button *startGame, *endGame, *back1, *ptp, *ptc;
	float duration;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void HelloWorld::startGameCallback(cocos2d::Ref* pSender);
	void HelloWorld::endGameCallback  (cocos2d::Ref* pSender);
	void HelloWorld::back1Callback    (cocos2d::Ref* pSender);
	void HelloWorld::ptcCallback      (cocos2d::Ref* pSender);
	void HelloWorld::ptpCallback      (cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
