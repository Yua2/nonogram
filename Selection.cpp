/******************************************************
*** Designer      : 陳
*** Date          : 2020.7.5
*** Purpose       : W2 パズルサイズ・パズル選択画面に関する処理
*******************************************************/
#include "Selection.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include <stdio.h>
#include <stdlib.h>
#include "file.h"
/******************************************************
*** Function Name: updatemSelection
*** Designer: 陳
*** Date: 2020.7.5
*** Function：mSelection構造体を更新する．
*** Return: Escene
*******************************************************/
void updatemSelection(Selection_t* mSelection,const char filename[256],const char filename2[256]) {
	int error,i;												
	FILE* fp;													//ファイル構造体
	error = fopen_s(&fp, filename2, "r");
	fscanf_s(fp, "%d", &mSelection->num);
	fclose(fp);
	error = fopen_s(&fp, filename, "r");
	mSelection->Spz = (SimplePuzzle_t*)malloc(mSelection->num * sizeof(SimplePuzzle_t));
	for (i = 0; i < mSelection->num; i++) {                     //パズル一覧を読み出す
		fscanf_s(fp, "%d %s %s %s %d %d", &(mSelection->Spz[i].puzzleId), &(mSelection->Spz[i].puzzleTitle),256, &(mSelection->Spz[i].puzzleMakerName),256, &(mSelection->Spz[i].fastestId),256, &(mSelection->Spz[i].fastestTime),&(mSelection->Spz[i].flag));
	}
	fclose(fp);
}
/******************************************************
*** Function Name: InitializeSelection
*** Designer: 陳
*** Date: 2020.7.5
*** Function：パズル選択画面を初期化する．
*** Return: Escene
*******************************************************/
int InitializeSelection(Selection_t* mSelection) {
	mSelection->size = 1010;
	mSelection->Font1 = CreateFontToHandle("ＭＳ ゴシック", 40, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
	mSelection->Font2 = CreateFontToHandle("ＭＳ ゴシック", 20, 1, DX_FONTTYPE_NORMAL);

	mSelection->HandleS1 = LoadGraph("selectionScr/1010.png");
	mSelection->HandleS2 = LoadGraph("selectionScr/1510.png");
	mSelection->HandleS3 = LoadGraph("selectionScr/1515.png");
	mSelection->HandleS4 = LoadGraph("selectionScr/2015.png");
	mSelection->HandleS5 = LoadGraph("selectionScr/2020.png");
	mSelection->HandleR1 = LoadGraph("selectionScr/return1.png");
	mSelection->HandleR2 = LoadGraph("selectionScr/return2.png");
	mSelection->HandleP1 = LoadGraph("selectionScr/puzzlebox1.png");
	mSelection->HandleP2 = LoadGraph("selectionScr/puzzlebox2.png");
	mSelection->HandleUp = LoadGraph("selectionScr/scrollarrow_up.png");
	mSelection->HandleDown = LoadGraph("selectionScr/scrollarrow_down.png");

	mSelection->scroll = 0;
	mSelection->scrollbar_on = 0;
	updatemSelection(mSelection, "PuzzleInfo/1010/simpleInformation.csv", "PuzzleInfo/1010/puzzlenum.csv");
	ScreenFlip();
	return SelectionScr;
}
/******************************************************
*** Function Name: UpdateSelection
*** Designer: 陳
*** Date: 2020.7.5
*** Function：パズル選択画面を更新する．
*** Return: Escene
*******************************************************/
int UpdateSelection(Selection_t* mSelection, Puzzle_t* Puzzle, Mouse_t* mouse) {
	int i, scrollsize, scrollBarLength, scrolldy,k;//ループカウンタ，スクロールできる上限，スクロールバーの長さ，スクロールした量，マウスクリック位置
	char min[20],No[20],filename[64];//時間，パズル番号，ファイルパス
	k = 0;

	if (mSelection->scrollbar_on == 0) {
		if (mouse->mX > 140 && mouse->mX < 300 && mouse->mY > 80 && mouse->mY < 135 && mouse->mButton == left && mouse->waitRelease == 0) {
			k = 1;
			mSelection->size = 1010;
			mSelection->scroll = 0;
			updatemSelection(mSelection, "PuzzleInfo/1010/simpleInformation.csv", "PuzzleInfo/1010/puzzlenum.csv");
		}
		else if (mouse->mX > 300 && mouse->mX < 460 && mouse->mY > 80 && mouse->mY < 135 && mouse->mButton == left && mouse->waitRelease == 0) {
			k = 1;
			mSelection->size = 1510;
			mSelection->scroll = 0;
			updatemSelection(mSelection, "PuzzleInfo/1510/simpleInformation.csv", "PuzzleInfo/1510/puzzlenum.csv");
		}
		else if (mouse->mX > 460 && mouse->mX < 620 && mouse->mY > 80 && mouse->mY < 135 && mouse->mButton == left && mouse->waitRelease == 0) {
			k = 1;
			mSelection->size = 1515;
			mSelection->scroll = 0;
			updatemSelection(mSelection, "PuzzleInfo/1515/simpleInformation.csv", "PuzzleInfo/1515/puzzlenum.csv");
		}
		else if (mouse->mX > 620 && mouse->mX < 780 && mouse->mY > 80 && mouse->mY < 135 && mouse->mButton == left && mouse->waitRelease == 0) {
			k = 1;
			mSelection->size = 2015;
			mSelection->scroll = 0;
			updatemSelection(mSelection, "PuzzleInfo/2015/simpleInformation.csv", "PuzzleInfo/2015/puzzlenum.csv");
		}
		else if (mouse->mX > 780 && mouse->mX < 940 && mouse->mY > 80 && mouse->mY < 135 && mouse->mButton == left && mouse->waitRelease == 0) {
			k = 1;
			mSelection->size = 2020;
			mSelection->scroll = 0;
			updatemSelection(mSelection, "PuzzleInfo/2020/simpleInformation.csv", "PuzzleInfo/2020/puzzlenum.csv");
		}
	}
	switch (mSelection->size) {
		case(1010):	DrawGraph(140, 80, mSelection->HandleS1, TRUE);  break;
		case(1510): DrawGraph(140, 80, mSelection->HandleS2, TRUE);  break;
		case(1515):	DrawGraph(140, 80, mSelection->HandleS3, TRUE);  break;
		case(2015):	DrawGraph(140, 80, mSelection->HandleS4, TRUE);  break;
		case(2020): DrawGraph(140, 80, mSelection->HandleS5, TRUE);  break;
		default:	break;

	}
	
	DrawFormatString(200, 100, GetColor(0, 0, 0), "10*10");
	DrawFormatString(360, 100, GetColor(0, 0, 0), "15*10");
	DrawFormatString(520, 100, GetColor(0, 0, 0), "15*15");
	DrawFormatString(680, 100, GetColor(0, 0, 0), "20*15");
	DrawFormatString(840, 100, GetColor(0, 0, 0), "20*20");
	if (mouse->mX < 114 && mouse->mX < 114 >0 && mouse->mY>661 && mouse->mY <720) {
		DrawGraph(-1, 661, mSelection->HandleR2, TRUE);
		k = 1;
		if (mouse->mButton == left && mouse->waitRelease ==0)
			return MenuScr;
	}else
		DrawGraph(-1, 661, mSelection->HandleR1, TRUE);
	
	
	DrawFormatString(29, 681, GetColor(0, 0, 0), "return");
	
	if (mSelection->num > 3) {
		scrollsize = 430 - mSelection->num * 160;
		mSelection->scroll += mouse->mWheel * 20;											//マウスホイール制御
		if (mSelection->scroll > 0)
			mSelection->scroll = 0;
		if (mSelection->scroll < scrollsize)
			mSelection->scroll = scrollsize;
		DrawBox(920, 139, 940, 159, GetColor(0, 0, 0), FALSE);								//スクロールアロー制御
		if (mouse->mX > 920 && mouse->mX < 940 && mouse->mY >139 && mouse->mY < 159 && mouse->waitRelease == 0) {
			DrawBox(921, 140, 939, 158, GetColor(192, 192, 192), TRUE);
			if (mouse->mButton == left && mSelection->scroll < 0)
				mSelection->scroll += 20;
		}
		else
			DrawBox(921, 140, 939, 158, GetColor(255, 255, 255), TRUE);
		DrawBox(920, 617, 940, 637, GetColor(0, 0, 0), FALSE);
		if (mouse->mX > 920 && mouse->mX < 940 && mouse->mY >617 && mouse->mY < 637 && mouse->waitRelease == 0) {
			DrawBox(921, 618, 939, 636, GetColor(192, 192, 192), TRUE);
			if (mouse->mButton == left && mSelection->scroll > scrollsize)
				mSelection->scroll -= 20;
		}
		else
			DrawBox(921, 618, 939, 636, GetColor(255, 255, 255), TRUE);
		DrawGraph(920, 140, mSelection->HandleUp, TRUE);
		DrawGraph(920, 617, mSelection->HandleDown, TRUE);

		DrawLine(920, 140, 920, 618, GetColor(0, 0, 0), 1);									//スクロールバー制御
		scrollBarLength = 460 * 496 / (mSelection->num * 160 + 66);
		DrawBox(920, 158 + 460 * mSelection->scroll / (scrollsize - 496), 940, 158 + 460 * mSelection->scroll / (scrollsize - 496) + scrollBarLength, GetColor(0, 0, 0), FALSE);

		if (mouse->waitRelease == 0 &&mouse->mX > 920 && mouse->mX < 940 && mouse->mY >158 + 460 * mSelection->scroll / (scrollsize - 496) && mouse->mY < 158 + 460 * mSelection->scroll / (scrollsize - 496) + scrollBarLength || mSelection->scrollbar_on == 1) {
			if (mouse->mButton == left) {
				mSelection->scrollbar_on = 1;
				if (mSelection->scroll_mouse_y != NULL) {
					scrolldy = mSelection->scroll_mouse_y - mouse->mY;
					if (mSelection->scroll < 0 && scrolldy > 0 || mSelection->scroll > scrollsize && scrolldy < 0)
						mSelection->scroll += -(scrollsize - 396) * scrolldy / 460;
				}
				mSelection->scroll_mouse_y = mouse->mY;
			}
			else {
				mSelection->scroll_mouse_y = NULL;
				mSelection->scrollbar_on = 0;
			}
		}
	}
		SetDrawArea(143, 140, 941, 636);

		for (i = 0; i < mSelection->num; i++) {                                     //パズル一覧の描画
			if (mouse->mX > 200 && mouse->mX < 880 && mouse->mY >170 + mSelection->scroll + 160 * i && mouse->mY < 280 + mSelection->scroll + 160 * i && mouse->mY >140 && mouse->mY < 636) {
				DrawGraph(200, 170 + mSelection->scroll + 160 * i, mSelection->HandleP2, TRUE);
				k = 1;
				if (mouse->mButton == left && mouse->waitRelease != 1) {
					sprintf_s(filename, 64, "PuzzleInfo/%d/Puzzle%d.csv", mSelection->size, mSelection->Spz[i].puzzleId);
					readPuzzle(Puzzle, filename);
					mouse->waitRelease = 1;
					return GameScr;
				}
			}
			else
				DrawGraph(200, 170 + mSelection->scroll + 160 * i, mSelection->HandleP1, TRUE);
			sprintf_s(No, 20, "No.%d", mSelection->Spz[i].puzzleId);
			DrawStringToHandle(220, 205 + mSelection->scroll + 160 * i, No, GetColor(0, 0, 0), mSelection->Font1);
			DrawStringToHandle(360, 205 + mSelection->scroll + 160 * i, mSelection->Spz[i].puzzleTitle, GetColor(0, 0, 0), mSelection->Font1);
			DrawStringToHandle(460, 250 + mSelection->scroll + 160 * i, "by", GetColor(0, 0, 0), mSelection->Font2);
			DrawStringToHandle(490, 250 + mSelection->scroll + 160 * i, mSelection->Spz[i].puzzleMakerName, GetColor(0, 0, 0), mSelection->Font2);
			if (mSelection->Spz[i].flag == 1) {
				DrawStringToHandle(800, 250 + mSelection->scroll + 160 * i, mSelection->Spz[i].fastestId, GetColor(0, 0, 0), mSelection->Font2);
				DrawStringToHandle(600, 180 + mSelection->scroll + 160 * i, "BEST TIME", GetColor(0, 0, 0), mSelection->Font2);
				if (mSelection->Spz[i].fastestTime % 60 >= 10)
					sprintf_s(min, 20, "%d:%d", mSelection->Spz[i].fastestTime / 60, mSelection->Spz[i].fastestTime % 60);
				else
					sprintf_s(min, 20, "%d:0%d", mSelection->Spz[i].fastestTime / 60, mSelection->Spz[i].fastestTime % 60);
				DrawStringToHandle(680, 205 + mSelection->scroll + 160 * i, min, GetColor(0, 0, 0), mSelection->Font1);
			}
	}
		if (k == 0 && mouse->mButton == left)
			mouse->waitRelease = 1;
	ScreenFlip();
	return SelectionScr;
}
/******************************************************
*** Function Name: FinalizeSelection
*** Designer: 陳
*** Date: 2020.7.5
*** Function：パズル選択画面を終了する．
*** Return: Escene
*******************************************************/
void FinalizeSelection(Selection_t* mSelection) {
	DeleteGraph(mSelection->HandleP1);
	DeleteGraph(mSelection->HandleP2);
	DeleteGraph(mSelection->HandleR1);
	DeleteGraph(mSelection->HandleR2);
	DeleteGraph(mSelection->HandleS1);
	DeleteGraph(mSelection->HandleS2);
	DeleteGraph(mSelection->HandleS3);
	DeleteGraph(mSelection->HandleS4);
	DeleteGraph(mSelection->HandleS5);
	DeleteGraph(mSelection->HandleUp);
	DeleteGraph(mSelection->HandleDown);
	DeleteFontToHandle(mSelection->Font1);
	DeleteFontToHandle(mSelection->Font2);
	free(mSelection->Spz);
	InitGraph();
	InitFontToHandle();
}