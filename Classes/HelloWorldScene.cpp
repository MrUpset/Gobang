#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	duration = 0.5f;
	LoginNode = CSLoader::createNode("LoginScene.csb");
	this->addChild(LoginNode);
	startGame = dynamic_cast<ui::Button*>(LoginNode->getChildByName("start_game_button"        ));
	endGame   = dynamic_cast<ui::Button*>(LoginNode->getChildByName("end_game_button"          ));
	back1     = dynamic_cast<ui::Button*>(LoginNode->getChildByName("back_to_login1"           ));
	ptc       = dynamic_cast<ui::Button*>(LoginNode->getChildByName("people_to_computer_button"));
	ptp       = dynamic_cast<ui::Button*>(LoginNode->getChildByName("people_to_people_button"  ));
	startGame-> addClickEventListener(CC_CALLBACK_1(HelloWorld::startGameCallback, this));
	endGame  -> addClickEventListener(CC_CALLBACK_1(HelloWorld::endGameCallback  , this));
	back1    -> addClickEventListener(CC_CALLBACK_1(HelloWorld::back1Callback    , this));
	ptc      -> addClickEventListener(CC_CALLBACK_1(HelloWorld::ptcCallback      , this));
	ptp      -> addClickEventListener(CC_CALLBACK_1(HelloWorld::ptpCallback      , this));

    return true;
}

void HelloWorld::startGameCallback(cocos2d::Ref* pSender){
	startGame->runAction(
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(-753.03, 0))));
	endGame  ->runAction(
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(-753.03, 0))));
	back1->runAction(Sequence::createWithTwoActions(
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(-821, 0))),
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(67.97, 0)))));
	ptc      ->runAction(Sequence::createWithTwoActions(
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(-821, 0))),
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(67.97, 0)))));
	ptp      ->runAction(Sequence::createWithTwoActions(
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(-821, 0))),
		EaseQuinticActionOut::create(MoveBy::create(duration, Vec2(67.97, 0)))));
}

void HelloWorld::endGameCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::back1Callback(cocos2d::Ref* pSender){
	startGame->runAction(MoveBy::create(duration, Vec2(753.03, 0)));
	endGame  ->runAction(MoveBy::create(duration, Vec2(753.03, 0)));
	back1    ->runAction(MoveBy::create(duration, Vec2(753.03, 0)));
	ptc      ->runAction(MoveBy::create(duration, Vec2(753.03, 0)));
	ptp      ->runAction(MoveBy::create(duration, Vec2(753.03, 0)));
}

void HelloWorld::ptcCallback(cocos2d::Ref* pSender){
	auto ptcScene = PTCScene::createScene();
	Director::getInstance()->replaceScene(ptcScene);
}

void HelloWorld::ptpCallback(cocos2d::Ref* pSender){
	auto ptpScene = PTPScene::createScene();
	Director::getInstance()->replaceScene(ptpScene);
}