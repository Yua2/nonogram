/******************************************************
*** Designer      : 陳
*** Date          : 2020.6.12
*** Purpose       : W2 パズルサイズ・パズル選択画面に関する情報
*******************************************************/
#pragma once
#include "Information.h"
#include "MouseState.h"
typedef struct {
	int puzzleId;                       //パズルの番号
	char puzzleTitle[256];				//パズルのタイトル
	char puzzleMakerName[256];			//パズル作成者の名前
	char fastestId[256];				//ランキング最上位の人の名前
	int fastestTime;					//ランキング最上位のクリア時間
	int flag;							//ランキングが有効かどうかを示すフラグ
}SimplePuzzle_t;
typedef struct {
	int size;							//パズルサイズ
	int Font1;							//フォントハンドル
	int Font2;							//フォントハンドル
	int scroll;							//スクロール量
	int num;							//パズルの数
	int HandleP1;						//画像ハンドル
	int HandleP2;						//画像ハンドル
	int HandleS1;						//画像ハンドル
	int HandleS2;						//画像ハンドル
	int HandleS3;						//画像ハンドル
	int HandleS4;						//画像ハンドル
	int HandleS5;						//画像ハンドル
	int HandleR1;						//画像ハンドル
	int HandleR2;						//画像ハンドル
	int HandleUp;						//画像ハンドル
	int HandleDown;						//画像ハンドル
	int scroll_mouse_y;					//スクロール中マウスの変位
	int scrollbar_on;					//スクロールバーが動作中かどうか
	SimplePuzzle_t *Spz;				//パズル一覧
}Selection_t;

int InitializeSelection(Selection_t*);
int UpdateSelection(Selection_t*, Puzzle_t*,Mouse_t*);
void FinalizeSelection(Selection_t*);