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
int InitializeMenu(Menu_t *mMenu) {
	int i, j;
	*mMenu = { 520,100,"MENU",{{ 100,300,400,500,165,440,"TUTORIAL"},                               //メニュー画面初期化
						{ 440,640,400,500,520,440,"START" },
						{ 780,980,400,500,830,440,"MAKE PUZZLE" } } };
	DrawFormatString(mMenu->x, mMenu->y, GetColor(0,0,0), mMenu->name);                     //タイトル描画
	for (i = 0; i < 3; i++) {                                                                       //ボタンの描画
		for (j = mMenu->b[i].leftx; j < mMenu->b[i].rightx; j++) {
			DrawPixel(j, mMenu->b[i].upy, GetColor(0, 0, 0));
			DrawPixel(j, mMenu->b[i].downy, GetColor(0, 0, 0));
		}
		for (j = mMenu->b[i].upy; j < mMenu->b[i].downy; j++) {
			DrawPixel(mMenu->b[i].leftx,j, GetColor(0, 0, 0));
			DrawPixel(mMenu->b[i].rightx,j, GetColor(0, 0, 0));
		}
		DrawFormatString(mMenu->b[i].charx, mMenu->b[i].chary, GetColor(0,0,0), mMenu->b[i].name);
	}
	ScreenFlip();
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
	int i,j,color[3];
	char name[64]="puzzleInfo/Puzzle0.csv";
	for (i = 0; i < 3; i++) {                                                                       //マウスが置かれているボタンを判定
		if (mouse->mX > mMenu->b[i].leftx && mouse->mX<mMenu->b[i].rightx && mouse->mY>mMenu->b[i].upy && mouse->mY < mMenu->b[i].downy) {
			color[i] = GetColor(120, 120, 120);
			if (mouse->mButton == left && mouse->waitRelease != 1) {
				mouse->waitRelease = 1;
				switch (i) {
				case(0): readPuzzle(puzzle,name); return GameScr;
				case(1):return SelectionScr;
				case(2):return MakePuzzleScr;
				default: break;
				}
			}
		}	
		else
			color[i] = GetColor(0,0,0);
	}

	DrawFormatString(mMenu->x, mMenu->y, GetColor(0,0,0), mMenu->name);
	for (i = 0; i < 3; i++) {
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

}