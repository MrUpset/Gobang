#ifndef __PTC_SCENE_H__
#define __PTC_SCENE_H__

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

//一、杀招
//1. 连五：这个直接就赢了，杀伤力MAX
//2. 活四：只要有任意方向存在，下一步必胜，杀伤力仅次于连五
//   双四：无论者这两个四是什么形状，都是一步杀，威力同活四，不过由于上面活四一步杀的存在，所以这里都是冲四
//   四三（活三）：由于活四必杀，所以这里讨论的都是冲四。这个实际上是和活四一样的：如果走出四三，那么如果对方需要防守的话，也是牵着鼻子走，等同于一步杀
//       复合棋型，只记录上面提到的必杀棋型，如：四四、四三（活三）、双活三
//			#define SHAPE_4_4	10000
//			#define SHAPE_4_F3	10000
//			#define SHAPE_F3_F3 2000
//3. 双活三：一步后形成四三，两步杀，略弱于活四
//
//二、好招
//能同时形成的棋型越多越好，四三（眠三）就比冲四好，但不像上面是一个质变，就累加好了
//下面讨论单方向棋型的威力
//1. 冲四：不防即杀，且能做VCF，好棋
//2. 活三：不防即杀，防也是冲四，略逊于冲四
//3. 眠三：一般般，不防冲四，与活三相比有质变区别，但是两个合在一起与活三有异曲同工之妙，可以用活三除以2得到
//3. 活二：至少眠三，还不错
//4. 眠二：没地方下就这个吧

//棋型，定义的值是其得分，注意5W分是立刻胜，1W分是一步杀，2000分是两步杀
//基本棋型，F是活，L是连，J是冲或眠
#define SHAPE_FIVE		50000
#define SHAPE_FOUR_FL	10010
#define SHAPE_FOUR_FREE	10000
#define SHAPE_FOUR_JUMP 1000
#define SHAPE_F3_F3  	2000//双活三
#define SHAPE_THREE_F	410
#define SHAPE_THREE_JL	210
#define SHAPE_THREE_J	200
#define SHAPE_TWO_FL	110
#define SHAPE_TWO_FREE	100
#define SHAPE_TWO_JL	60
#define SHAPE_TWO_JUMP	50
#define SHAPE_ONE_NEAR	2

#define huo 1
#define mian 0
#define die -1

struct whichState{
	int number, state;
};

struct nearCount{
	whichState heng, shu, zuoxie, youxie;
};

struct posi{
	int x, y;
};

class PTCScene : public cocos2d::Layer
{
private:
	cocos2d::Node *PTCNode;
	cocos2d::Sprite *chessBoard, *point, *backupSprite1, *backupSprite2;
	cocos2d::ui::Button *back2, *cancle, *ok, *restart;
	posi position, backupPosition1, backupPosition2;
	int state[17][17], eval_state_black[17][17], eval_state_white[17][17];
	bool whichChess, cancleOnce, end;
	float duration;
public:
	virtual bool init();
	static cocos2d ::Scene* createScene();
	bool   PTCScene::checkGame(int x, int y, int color);
	int    PTCScene::evaluate(nearCount dir);
	void   PTCScene::search();
	void   PTCScene::okCallback(cocos2d::Ref* pSender);
	void   PTCScene::back2Callback(cocos2d::Ref* pSender);
	void   PTCScene::cancleCallback(cocos2d::Ref* pSender);
	void   PTCScene::restartCallback(cocos2d::Ref* pSender);
	void   PTCScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	nearCount PTCScene::getNearCount(int x, int y, int color);
	CREATE_FUNC(PTCScene);
};

//const static int important_table[17][17] =
//{
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
//	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
//	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
//	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
//	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
//	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
//};

#endif