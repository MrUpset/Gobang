#include "PTPScene.h"

USING_NS_CC;

Scene* PTPScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PTPScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool PTPScene::init(){
	if (!Layer::init()){
		return false;
	}

	PTPNode = CSLoader::createNode("MainScene.csb");
	this->addChild(PTPNode);
	for (int i = 1; i <= 15; i++)
		for (int j = 1; j <= 15; j++)
			state[i][j] = 0;
	for (int i = 0; i < 17; i++){
		state[0][i]  = -1;
		state[16][i] = -1;
		state[i][0]  = -1;
		state[i][16] = -1;
	}
	duration         = 0.05f;
	position.x       = position.y       = 8;
	backupPosition.x = backupPosition.y = 0;
	end              = false;
	cancleOnce       = true;
	whichChess       = false;
	ok               = nullptr;
	back2            = dynamic_cast<ui::Button*>(PTPNode->getChildByName("back_to_login2"));
	cancle           = dynamic_cast<ui::Button*>(PTPNode->getChildByName("cancle"));
	restart          = dynamic_cast<ui::Button*>(PTPNode->getChildByName("restart"));
	point            = Sprite::create("point.png");
	auto listener1   = EventListenerKeyboard::create();

	back2		    ->addClickEventListener(CC_CALLBACK_1(PTPScene::back2Callback,   this));
	cancle          ->addClickEventListener(CC_CALLBACK_1(PTPScene::cancleCallback,  this));
	restart		    ->addClickEventListener(CC_CALLBACK_1(PTPScene::restartCallback, this));
	listener1		->onKeyReleased = CC_CALLBACK_2(PTPScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

	point->setPosition(Vec2(467, 319));
	this->addChild(point);

	text_blackChess = dynamic_cast<ui::Text*>(PTPNode->getChildByName("Text_BlackChess"));
	text_whiteChess = dynamic_cast<ui::Text*>(PTPNode->getChildByName("Text_WhiteChess"));
	text_blackChess->setVisible(true);

	return true;
}

void PTPScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	int x = position.x, y = position.y;
	if (end == true) return;
	switch ((int)keyCode)
	{
	case(up) :
		if (y <= 1)  break;
		position.y--;
		point->runAction(MoveBy::create(duration, Vec2(0, 34))); break;
	case(down) :
		if (y >= 15) break;
		position.y++;
		point->runAction(MoveBy::create(duration, Vec2(0, -34))); break;
	case(left) :
		if (x <= 1)  break;
		position.x--;
		point->runAction(MoveBy::create(duration, Vec2(-34, 0))); break;
	case(right) :
		if (x >= 15) break;
		position.x++;
		point->runAction(MoveBy::create(duration, Vec2(34, 0))); break;
	case(enter) :
		//不重复在同一点下棋
		if (state[y][x] != 0) break;
		//判断黑白棋并落子
		if (whichChess == whiteChess){
			state[y][x] = white;
			Sprite *whiteChessSprite = Sprite::create("white.png");
			whiteChessSprite->setPosition(point->getPosition());
			text_whiteChess->setVisible(false);
			text_blackChess->setVisible(true);
			this->addChild(whiteChessSprite);
			whichChess = blackChess;
			//备份以备悔棋
			backupSprite = whiteChessSprite;
		}
		else{
			state[y][x] = black;
			Sprite *blackChessSprite = Sprite::create("black.png");
			blackChessSprite->setPosition(point->getPosition());
			text_blackChess->setVisible(false);
			text_whiteChess->setVisible(true);
			this->addChild(blackChessSprite);
			whichChess = whiteChess;
			//备份以备悔棋
			backupSprite = blackChessSprite;
		}
		backupPosition.x = x;
		backupPosition.y = y;
		cancleOnce = false;
		//判断游戏是否结束
		if (PTPScene::checkGame() == true){
			end = true;
			if (whichChess == blackChess){
				auto whiteWin = CSLoader::createNode("whiteWin.csb");
				this->addChild(whiteWin);
				ok = dynamic_cast<ui::Button*>(whiteWin->getChildByName("ok"));
				ok ->addClickEventListener(CC_CALLBACK_1(PTPScene::okCallback, this));
			}
			else if (whichChess == whiteChess){
				auto blackWin = CSLoader::createNode("blackWin.csb");
				this->addChild(blackWin);
				ok = dynamic_cast<ui::Button*>(blackWin->getChildByName("ok"));
				ok->addClickEventListener(CC_CALLBACK_1(PTPScene::okCallback, this));
			}
		}
		//判断是否和棋(棋盘全满)
		int i, j;
		for (i = 1; i <= 15; i++)
			for (j = 1; j <= 15; j++)
				if (state[i][j] == NULL) break;
		if (i == 16 && j == 16 && state[15][15] != NULL){
			end = true;
			auto noWin = CSLoader::createNode("noWin.csb");
			this->addChild(noWin);
			ok = dynamic_cast<ui::Button*>(noWin->getChildByName("ok"));
			ok->addClickEventListener(CC_CALLBACK_1(PTPScene::okCallback, this));
			return;
		}
		break;
	}
}


void PTPScene::back2Callback(cocos2d::Ref* pSender){
	auto helloWorldScene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(helloWorldScene);
}

void PTPScene::cancleCallback(cocos2d::Ref* pSender){
	if (backupPosition.x == 0 || backupPosition.y == 0 || cancleOnce == true) return;
	if (ok != nullptr) ok->getParent()->removeFromParent();
	backupSprite->removeFromParent();
	state[backupPosition.y][backupPosition.x] = 0;
	whichChess = !whichChess;
	cancleOnce = true;
	if (end == true) end = false;
	if (whichChess == blackChess){
		text_whiteChess->setVisible(false);
		text_blackChess->setVisible(true);
	}
	else{
		text_blackChess->setVisible(false);
		text_whiteChess->setVisible(true);
	}
}

void PTPScene::okCallback(cocos2d::Ref* pSender){
	ok->getParent()->removeFromParent();
	ok = nullptr;
}

void PTPScene::restartCallback(cocos2d::Ref* pSender){
	auto PTPScene = PTPScene::createScene();
	Director::getInstance()->replaceScene(PTPScene);
}

bool PTPScene::checkGame(){
	int x = position.x, y = position.y;
	int heng = 0, shu = 0, zuoxie = 0, youxie = 0;// "- | / \"
	int i, color = state[y][x];
	for (i = 0; state[y][x + i] == color; i++, heng++);
	for (i = 0; state[y][x - i] == color; i++, heng++);
	--heng;
	for (i = 0; state[y + i][x] == color; i++, shu++);
	for (i = 0; state[y - i][x] == color; i++, shu++);
	--shu;
	for (i = 0; state[y + i][x - i] == color; i++, zuoxie++);
	for (i = 0; state[y - i][x + i] == color; i++, zuoxie++);
	--zuoxie;
	for (i = 0; state[y + i][x + i] == color; i++, youxie++);
	for (i = 0; state[y - i][x - i] == color; i++, youxie++);
	--youxie;
	if (heng >= 5 || shu >= 5 || zuoxie >= 5 || youxie >= 5)
		return true;
	return false;
}