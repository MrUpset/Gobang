#include "PTCScene.h"

USING_NS_CC;

Scene* PTCScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = PTCScene::create();
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

bool PTCScene::init(){
	if (!Layer::init()){
		return false;
	}

	PTCNode = CSLoader::createNode("MainScene.csb");
	this->addChild(PTCNode);
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
	backupPosition1.x= backupPosition1.y= 0;
	backupPosition2.x= backupPosition2.y= 0;
	end              = false;
	cancleOnce       = true;
	whichChess       = false;
	ok               = nullptr;
	back2            = dynamic_cast<ui::Button*>(PTCNode->getChildByName("back_to_login2"));
	cancle           = dynamic_cast<ui::Button*>(PTCNode->getChildByName("cancle"));
	restart          = dynamic_cast<ui::Button*>(PTCNode->getChildByName("restart"));
	point            = Sprite::create("point.png");
	auto listener1   = EventListenerKeyboard::create();

	back2		    ->addClickEventListener(CC_CALLBACK_1(PTCScene::back2Callback,   this));
	cancle          ->addClickEventListener(CC_CALLBACK_1(PTCScene::cancleCallback,  this));
	restart		    ->addClickEventListener(CC_CALLBACK_1(PTCScene::restartCallback, this));
	listener1		->onKeyReleased = CC_CALLBACK_2(PTCScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

	point->setPosition(Vec2(467, 319));
	this->addChild(point);

	return true;
}

void PTCScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
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
		//黑子落子
		state[y][x] = black;
		whichChess = blackChess;
		Sprite *blackChessSprite = Sprite::create("black.png");
		blackChessSprite->setPosition(point->getPosition());
		this->addChild(blackChessSprite);
		//备份以备悔棋
		backupSprite1 = blackChessSprite;
		backupPosition1.x = x;
		backupPosition1.y = y;
		//判断游戏是否结束
		if (PTCScene::checkGame(backupPosition1.x, backupPosition1.y, black) == true){
			end = true;
			auto blackWin = CSLoader::createNode("win.csb");
			this->addChild(blackWin);
			ok = dynamic_cast<ui::Button*>(blackWin->getChildByName("ok"));
			ok->addClickEventListener(CC_CALLBACK_1(PTCScene::okCallback, this));
			return;
		}
		//白子(电脑)落子
		whichChess = whiteChess;
		PTCScene::search();
		//设置成可以悔棋状态
		cancleOnce = false;
		if (end == true) break;//和棋
		//判断游戏是否结束
		if (PTCScene::checkGame(backupPosition2.x, backupPosition2.y, white) == true){
			end = true;
			auto whiteWin = CSLoader::createNode("lose.csb");
			this->addChild(whiteWin);
			ok = dynamic_cast<ui::Button*>(whiteWin->getChildByName("ok"));
			ok->addClickEventListener(CC_CALLBACK_1(PTCScene::okCallback, this));
			return;
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
			ok->addClickEventListener(CC_CALLBACK_1(PTCScene::okCallback, this));
			return;
		}
		break;
	}
}

void PTCScene::back2Callback(cocos2d::Ref* pSender){
	auto helloWorldScene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(helloWorldScene);
}

void PTCScene::cancleCallback(cocos2d::Ref* pSender){
	if (backupPosition1.x == 0 ||
		backupPosition1.y == 0 ||
		cancleOnce == true     ||
		whichChess == whiteChess) return;
	if (ok != nullptr) ok->getParent()->removeFromParent();
	backupSprite1->removeFromParent();
	backupSprite2->removeFromParent();
	state[backupPosition1.y][backupPosition1.x] = 0;
	state[backupPosition2.y][backupPosition2.x] = 0;
	cancleOnce = true;
	if (end == true) end = false;
	ok = nullptr;
}

void PTCScene::okCallback(cocos2d::Ref* pSender){
	ok->getParent()->removeFromParent();
	ok = nullptr;
}

void PTCScene::restartCallback(cocos2d::Ref* pSender){
	auto PTCScene = PTCScene::createScene();
	Director::getInstance()->replaceScene(PTCScene);
}

bool PTCScene::checkGame(int x, int y, int color){
	int heng = 0, shu = 0, zuoxie = 0, youxie = 0;// "- | / \"
	int i;
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

nearCount PTCScene::getNearCount(int x, int y, int color){
	int heng = 0, shu = 0, zuoxie = 0, youxie = 0;// "- | / \"
	int i;
	nearCount a;

	for (i = 1; state[y][x + i] == color; i++, heng++);
		if (state[y][x + i] != color && state[y][x + i] != NULL) a.heng.state = mian;
		else a.heng.state = huo;
	for (i = 1; state[y][x - i] == color; i++, heng++);
		if (state[y][x - i] != color && state[y][x - i] != NULL){
			if (a.heng.state == mian) a.heng.state = die;
			else a.heng.state = mian;
		}
		else if (state[y][x - i] == NULL){
			if (a.heng.state == mian) a.heng.state = mian;
			else a.heng.state = huo;
		}
	a.heng.number = heng;

	for (i = 1; state[y + i][x] == color; i++, shu++);
		if (state[y + i][x] != color && state[y + i][x] != NULL) a.shu.state = mian;
		else a.shu.state = huo;
	for (i = 1; state[y - i][x] == color; i++, shu++);
		if (state[y - i][x] != color && state[y - i][x] != NULL){
			if (a.shu.state == mian) a.shu.state = die;
			else a.shu.state = mian;
		}
		else if (state[y - i][x] == NULL){
			if (a.shu.state == mian) a.shu.state = mian;
			else a.shu.state = huo;
		}
	a.shu.number = shu;

	for (i = 1; state[y + i][x - i] == color; i++, zuoxie++);
		if (state[y + i][x - i] != color && state[y + i][x - i] != NULL) a.zuoxie.state = mian;
		else a.zuoxie.state = huo;
	for (i = 1; state[y - i][x + i] == color; i++, zuoxie++);
		if (state[y - i][x + i] != color && state[y - i][x + i] != NULL){
			if (a.zuoxie.state == mian) a.zuoxie.state = die;
			else a.zuoxie.state = mian;
		}
		else if (state[y - i][x + i] == NULL){
			if (a.zuoxie.state == mian) a.zuoxie.state = mian;
			else a.zuoxie.state = huo;
		}
	a.zuoxie.number = zuoxie;

	for (i = 1; state[y + i][x + i] == color; i++, youxie++);
		if (state[y + i][x + i] != color && state[y + i][x + i] != NULL) a.youxie.state = mian;
		else a.youxie.state = huo;
	for (i = 1; state[y - i][x - i] == color; i++, youxie++);
		if (state[y - i][x - i] != color && state[y - i][x - i] != NULL){
			if (a.youxie.state == mian) a.youxie.state = die;
			else a.youxie.state = mian;
		}
		else if (state[y - i][x - i] == NULL){
			if (a.youxie.state == mian) a.youxie.state = mian;
			else a.youxie.state = huo;
		}
	a.youxie.number = youxie;

	return a;
}

int PTCScene::evaluate(nearCount dir){
	int temp = 0, score = 0;

	if (dir.heng.number   == 1 && dir.shu.number    == 1&&
		dir.zuoxie.number == 1 && dir.youxie.number == 1)
		return SHAPE_ONE_NEAR;//一点

	if (dir.heng.number   >= 5 || dir.shu.number    >= 5 || 
		dir.zuoxie.number >= 5 || dir.youxie.number >= 5)//五连
		return SHAPE_FIVE;

	if (dir.heng  .number == 4 && dir.heng  .state == huo) temp++;
	if (dir.shu   .number == 4 && dir.shu   .state == huo) temp++;
	if (dir.zuoxie.number == 4 && dir.zuoxie.state == huo) temp++;
	if (dir.youxie.number == 4 && dir.youxie.state == huo) temp++;
	if (temp >= 2) 
		score += SHAPE_FOUR_FL;//双活四
	else if (temp == 1)
		score += SHAPE_FOUR_FREE;//活四
	temp = 0;
	if ((dir.heng  .number == 4 && dir.heng  .state == mian)||//冲四
		(dir.shu   .number == 4 && dir.shu   .state == mian)||
		(dir.zuoxie.number == 4 && dir.zuoxie.state == mian)||
		(dir.youxie.number == 4 && dir.youxie.state == mian))
		score += SHAPE_FOUR_JUMP;

	if (dir.heng  .number == 3 && dir.heng  .state == huo) temp++;
	if (dir.shu   .number == 3 && dir.shu   .state == huo) temp++;
	if (dir.zuoxie.number == 3 && dir.zuoxie.state == huo) temp++;
	if (dir.youxie.number == 3 && dir.youxie.state == huo) temp++;
	if (temp >= 2) 
		score += SHAPE_F3_F3;//双活三
	else if (temp == 1)
		score += SHAPE_THREE_F;//活三
	temp = 0;
	if (dir.heng  .number == 3 && dir.heng  .state == mian) temp++;
	if (dir.shu   .number == 3 && dir.shu   .state == mian) temp++;
	if (dir.zuoxie.number == 3 && dir.zuoxie.state == mian) temp++;
	if (dir.youxie.number == 3 && dir.youxie.state == mian) temp++;
	if (temp >= 2)
		score += SHAPE_THREE_JL;//双眠三
	else if (temp == 1)
		score += SHAPE_THREE_J;//眠三
	temp = 0;

	if (dir.heng  .number == 2 && dir.heng  .state == huo) temp++;
	if (dir.shu   .number == 2 && dir.shu   .state == huo) temp++;
	if (dir.zuoxie.number == 2 && dir.zuoxie.state == huo) temp++;
	if (dir.youxie.number == 2 && dir.youxie.state == huo) temp++;
	if (temp >= 2)
		score += SHAPE_TWO_FL;//双活二
	else if (temp == 1)
		score += SHAPE_TWO_FREE;//活二
	temp = 0;
	if (dir.heng  .number == 2 && dir.heng  .state == mian) temp++;
	if (dir.shu   .number == 2 && dir.shu   .state == mian) temp++;
	if (dir.zuoxie.number == 2 && dir.zuoxie.state == mian) temp++;
	if (dir.youxie.number == 2 && dir.youxie.state == mian) temp++;
	if (temp >= 2)
		score += SHAPE_TWO_JL;//双眠二
	else if (temp == 1)
		score += SHAPE_TWO_JUMP;//眠二

	return score;
}

void PTCScene::search(){
	int i, j, w_x, w_y, w_max = 0, b_x, b_y, b_max = 0, x, y, max;
	nearCount a;
	for (i = 1; i <= 15; i++){
		for (j = 1; j <= 15; j++){
			if (state[i][j] == NULL){
				//考虑黑色的下一步
				a = getNearCount(j, i, black);
				a.heng  .number++;
				a.shu   .number++;
				a.zuoxie.number++;
				a.youxie.number++;
				eval_state_black[i][j] = evaluate(a);// +important_table[i][j];
				//考虑白色的当前步
				a = getNearCount(j, i, white);
				a.heng  .number++;
				a.shu   .number++;
				a.zuoxie.number++;
				a.youxie.number++;
				eval_state_white[i][j] = evaluate(a);// +important_table[i][j];
			}
			else{
				eval_state_black[i][j] = NULL;
				eval_state_white[i][j] = NULL;
			}
		}
	}
	for (i = 1; i <= 15; i++){
		for (j = 1; j <= 15; j++){
			if (w_max < eval_state_white[i][j]){
				w_max = eval_state_white[i][j];
				w_x = j;
				w_y = i;
			}
			else if (b_max < eval_state_black[i][j]){
				b_max = eval_state_black[i][j];
				b_x = j;
				b_y = i;
			}
		}
	}
	if (b_max < 0 && w_max < 0){//和棋
		end = true;
		auto noWin = CSLoader::createNode("noWin.csb");
		this->addChild(noWin);
		ok = dynamic_cast<ui::Button*>(noWin->getChildByName("ok"));
		ok->addClickEventListener(CC_CALLBACK_1(PTCScene::okCallback, this));
		return;
	}
	if (b_max > w_max){
		x = b_x;
		y = b_y;
		max = b_max;
	}
	else{
		x = w_x;
		y = w_y;
		max = w_max;
	}
	state[y][x] = white;
	Sprite *whiteChessSprite = Sprite::create("white.png");
	whiteChessSprite->setPosition(Vec2(195 + x * 34, 591 - y * 34));
	this->addChild(whiteChessSprite);
	point->runAction(MoveTo::create(duration, Vec2(195 + x * 34, 591 - y * 34))); 
	position.x = x;
	position.y = y;
	whichChess = blackChess;
	//备份以备悔棋
	backupSprite2 = whiteChessSprite;
	backupPosition2.x = x;
	backupPosition2.y = y;
}