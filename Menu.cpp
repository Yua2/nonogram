/******************************************************
*** Designer: 陳
*** Date: 2020.6.23
*** Purpose: メニュー画面に関する処理
*******************************************************/
#include "DxLib.h"
#include "SceneMgr.h"
#include "MouseState.h"
#include "file.h"
#include "Information.h"

/******************************************************
*** Function Name: InitializeMenu
*** Designer: 陳
*** Date: 2020.6.23
*** Function:メニュー画面を初期化する．
*** Return: Escene
*******************************************************/
static int first = 0;																							//初めてメニュー画面に来るかどうか
int InitializeMenu(Menu_t *mMenu,Mouse_t *mouse) {
	
	*mMenu = {LoadGraph("graph/menu.png"),{{ 100,300,400,500,165,440,"TUTORIAL"},                               //メニュー画面初期化
						{ 440,640,400,500,520,440,"START" },
						{ 780,980,400,500,830,440,"MAKE PUZZLE" } } };
	DrawGraph(0, 0, mMenu->handle, TRUE);
	if (first == 0) {																							//初めてメニュー画面に来るのでなければ表示しない
		DrawFormatString(480, 400, GetColor(255, 255, 255), "PRESS ANY KEY");
		ScreenFlip();
		WaitKey();
		first = 1;
		mouse->waitRelease = 1;
	}
	return MenuScr;
}
/******************************************************
*** Function Name: UpedateMenu
*** Designer: 陳
*** Date: 2020.6.23
*** Function:メニュー画面を更新する．
*** Return: Escene
*******************************************************/
int UpdateMenu(Menu_t* mMenu,Mouse_t* mouse,Puzzle_t* puzzle) {
	int i,j,k,color[3];												//i:ループカウンタ，j:ループカウンタ，k:マウスの位置を記録，color:ボタンの色
	char name[64]="puzzleInfo/Puzzle0.csv";														  //チュートリアルパズルのファイルのパス
	DrawGraph(0, 0, mMenu->handle, TRUE);
	k = 0;
	for (i = 0; i < 3; i++) {                                                                       //マウスが置かれているボタンを判定
		if (mouse->mX > mMenu->b[i].leftx && mouse->mX<mMenu->b[i].rightx && mouse->mY>mMenu->b[i].upy && mouse->mY < mMenu->b[i].downy) {		//三つのボタンの座標
			color[i] = GetColor(120, 120, 120);
			if (mouse->mButton == left && mouse->waitRelease != 1) {																			//ボタンが押されたかどうか
				mouse->waitRelease = 1;
				switch (i) {																													//選択された画面へ遷移
				case(0): readPuzzle(puzzle,name); return GameScr;
				case(1):return SelectionScr;
				case(2):return MakePuzzleScr;
				fault: break;
				}
			}
			k = 1;
		}	
		else
			color[i] = GetColor(0,0,0);	
	}

	if (k == 0 && mouse->mButton == left)																										//マウスはボタンじゃない場所で押されたら，離すまで動作しない．
		mouse->waitRelease = 1;

	for (i = 0; i < 3; i++) {																		//マウスが置かれているボタンの色を変える
		for (j = mMenu->b[i].leftx; j < mMenu->b[i].rightx; j++) {
			DrawPixel(j, mMenu->b[i].upy, GetColor(0,0,0));
			DrawPixel(j, mMenu->b[i].downy, GetColor(0,0,0));
		}
		for (j = mMenu->b[i].upy; j < mMenu->b[i].downy; j++) {
			DrawPixel(mMenu->b[i].leftx, j, GetColor(0,0,0));
			DrawPixel(mMenu->b[i].rightx, j, GetColor(0,0,0));
		}
		DrawFormatString(mMenu->b[i].charx, mMenu->b[i].chary,color[i], mMenu->b[i].name);
	}
	ScreenFlip();
	return MenuScr;
}
void FinalizeMenu(Menu_t* mMenu) {
	InitGraph();
	InitFontToHandle();
}