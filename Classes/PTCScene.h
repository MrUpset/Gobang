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

//һ��ɱ��
//1. ���壺���ֱ�Ӿ�Ӯ�ˣ�ɱ����MAX
//2. ���ģ�ֻҪ�����ⷽ����ڣ���һ����ʤ��ɱ��������������
//   ˫�ģ�����������������ʲô��״������һ��ɱ������ͬ���ģ����������������һ��ɱ�Ĵ��ڣ��������ﶼ�ǳ���
//   �����������������ڻ��ı�ɱ�������������۵Ķ��ǳ��ġ����ʵ�����Ǻͻ���һ���ģ�����߳���������ô����Է���Ҫ���صĻ���Ҳ��ǣ�ű����ߣ���ͬ��һ��ɱ
//       �������ͣ�ֻ��¼�����ᵽ�ı�ɱ���ͣ��磺���ġ���������������˫����
//			#define SHAPE_4_4	10000
//			#define SHAPE_4_F3	10000
//			#define SHAPE_F3_F3 2000
//3. ˫������һ�����γ�����������ɱ�������ڻ���
//
//��������
//��ͬʱ�γɵ�����Խ��Խ�ã��������������ͱȳ��ĺã�������������һ���ʱ䣬���ۼӺ���
//�������۵��������͵�����
//1. ���ģ�������ɱ��������VCF������
//2. ������������ɱ����Ҳ�ǳ��ģ���ѷ�ڳ���
//3. ������һ��㣬�������ģ������������ʱ����𣬵�����������һ�������������ͬ��֮������û�������2�õ�
//3. ���������������������
//4. �߶���û�ط��¾������

//���ͣ������ֵ����÷֣�ע��5W��������ʤ��1W����һ��ɱ��2000��������ɱ
//�������ͣ�F�ǻL������J�ǳ����
#define SHAPE_FIVE		50000
#define SHAPE_FOUR_FL	10010
#define SHAPE_FOUR_FREE	10000
#define SHAPE_FOUR_JUMP 1000
#define SHAPE_F3_F3  	2000//˫����
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