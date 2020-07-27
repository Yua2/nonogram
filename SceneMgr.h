/******************************************
*** Designer : 藤川
*** Date     : 2020.6.12
*** Purpose  : チュートリアル/ゲームプレイ画面の
			　　の構造体Game_tのメンバと各関数の
			   プロトタイプ宣言(M12~M15)
*******************************************/

#ifndef SCENEMGR_H_
#define SCENEMGR_H_

#include "InputNickName.h"
#include "Menu.h"
#include "Selection.h"
#include "Game.h"
#include "MakePuzzle.h"
#include "MouseState.h"
#include "Information.h"

// シーン番号
typedef enum {
	InputNickNameScr,	//　起動時シーン番号
	MenuScr,	//　メニューシーン番号
	SelectionScr,	//　難易度・パズル選択シーン番号
	GameScr,	//　チュートリアル/ゲームプレイシーン番号
	MakePuzzleScr,	// パズル作成シーン番号
	None	// 未起動シーン番号
}EScene;

// シーン構造体
typedef struct SCR_MGR {
	InputNickName_t mInputNickName;	// 起動時画面構造体
	Menu_t          mMenu;	// メニュー画面構造体
	Selection_t	    mSelection;	// 難易度・パズル選択画面構造体
	Game_t          mGame;	// チュートリアル/ゲームプレイ画面構造体
	MakePuzzle_t    mMakePuzzle;	// パズル作成画面構造体
} ScrMgr_t;

// M1:シーン初期化処理
int InitializeScene(int, ScrMgr_t*, Puzzle_t*,Mouse_t*);

// M2:シーン更新処理
int UpdateScene(int, ScrMgr_t*, Mouse_t*, Puzzle_t*);

// M3:シーン終了処理
void FinalizeScene(int, ScrMgr_t*, Puzzle_t*);

#endif /* SCENEMGR_H_ */