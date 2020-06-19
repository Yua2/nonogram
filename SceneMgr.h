#ifndef SCENEMGR_H_
#define SCENEMGR_H_

#include "InputNickName.h"
#include "Menu.h"
#include "Selection.h"
#include "Game.h"
#include "MakePuzzle.h"
#include "MouseState.h"

// シーン列挙体
typedef enum {
	InputNickNameScr,	// ニックネーム入力画面
	MenuScr,			// メニュー画面
	SelectionScr,		// 難易度・パズル選択画面
	GameScr,			// チュートリアル/ゲームプレイ画面
	MakePuzzleScr,			// パズル作成/投稿画面
	None				// 接続失敗
} EScene;

//　シーン管理構造体
typedef struct SCR_MGR {
	InputNickName_t mInputNickName;	// ニックネーム入力画面構造体
	Menu_t          mMenu;			// メニュー画面構造体
	Selection_t	    mSelection;		// 難易度・パズル選択画面構造体
	Game_t          mGame;			// チュートリアル/ゲームプレイ画面構造体
	MakePuzzle_t    mMakePuzzle;	// パズル作成/投稿画面構造体
} ScrMgr_t;							


// M1:シーン初期化処理
EScene InitializeScene(EScene nextScene, ScrMgr_t* scrMgr);

// M2:シーン更新処理
EScene UpdateScene(EScene scene, ScrMgr_t* scrMgr, Mouse_t mouse, int* key, Puzzle puzzle);

// M3:シーン終了処理
void FinalizeScene(EScene scene, ScrMgr_t* scrMgr);

#endif /* SCENEMGR_H_ */
