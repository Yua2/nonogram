/*********************************************
*** Designer : 藤川
*** Date     : 2020.6.12
*** Purpose  : ボタン関数の実装
**********************************************/

#include "DxLib.h"
#include "SceneMgr.h"
#include "MouseState.h"
#include "Information.h"

/*********************************************************
*** Function Name : InitializeScene (M1)
*** Designer      : 藤川
*** Date          : 2020.7.7
*** Function      : シーン遷移するときのシーンごとの初期化処理呼び出し
*** Return        : scene (遷移したシーン先のシーン番号)
**********************************************************/
int InitializeScene(int nextScene, ScrMgr_t* scrMgr, Puzzle_t* puzzle,Mouse_t*mouse) {
	int scene = 0;	// 返り値のシーン番号

	// 引数のこれから遷移するシーン番号によって分岐
	switch (nextScene) {
	case InputNickNameScr:	// 起動シーン番号の時
		scene = InitializeInputNickName(&(scrMgr->mInputNickName));	// 起動シーン番号の初期化処理を実行し，返り値を設定
		break;

	case MenuScr:	// メニューシーン番号の時
		scene = InitializeMenu(&(scrMgr->mMenu),mouse);	// メニューシーン番号の初期化処理を実行し，返り値を設定
		break;

	case SelectionScr:	// 難易度・パズル選択シーン番号の時
		scene = InitializeSelection(&(scrMgr->mSelection));	// 難易度・パズル選択シーン番号の初期化処理を実行し，返り値を設定
		break;

	case GameScr:	// チュートリアル/ゲームプレイシーン番号の時
		scene = InitializeGame(&(scrMgr->mGame), puzzle);	// チュートリアル/ゲームプレイシーン番号の初期化処理を実行し，返り値を設定
		break;

	case MakePuzzleScr:	// パズル作成シーン番号の時
		scene = InitializeMakePuzzle(&(scrMgr->mMakePuzzle));	// パズル作成シーン番号の初期化処理を実行し，返り値を設定
		break;
	}

	return scene;	// 遷移したシーン先のシーン番号
}

/*********************************************************
*** Function Name : UpdateScene (M2)
*** Designer      : 藤川
*** Date          : 2020.7.17
*** Function      : 現在のシーンごとのの更新処理呼び出し
*** Return        : nextScene (これから遷移するシーンのシーン番号)
**********************************************************/
int UpdateScene(int scene, ScrMgr_t* scrMgr, Mouse_t* mouse, Puzzle_t* puzzle) {
	int nextScene = 0;	// 返り値のこれから遷移するシーン先のシーン番号

	// 引数の現在のシーン番号によって分岐
	switch (scene) {
	case InputNickNameScr:	// 起動シーン番号のとき
		nextScene = UpdateInputNickName(&(scrMgr->mInputNickName), mouse);	// 起動シーンの更新処理を呼び出し，返り値を設定
		break;

	case MenuScr:	// メニューシーン番号のとき
		nextScene = UpdateMenu(&(scrMgr->mMenu), mouse, puzzle);	// メニューシーンの更新処理を呼び出し，返り値を設定
		break;

	case SelectionScr:	// 難易度・パズル選択シーン番号のとき
		nextScene = UpdateSelection(&(scrMgr->mSelection), puzzle, mouse);	// 難易度・パズル選択シーンの更新処理を呼び出し，返り値を設定
		break;

	case GameScr:	// チュートリアル/ゲームプレイシーン番号のとき
		// パズルIDが0(チュートリアルパズル)の時
		if (puzzle->puzzleId == 0) {
			nextScene = UpdateTutorial(&(scrMgr->mGame), puzzle, mouse);	// チュートリアルシーンの更新処理を呼び出し，返り値を設定
		}
		// パズルIDが0でない(チュートリアルパズル以外のパズル)のとき
		else {
			nextScene = UpdateGame(&(scrMgr->mGame), puzzle, mouse);	// ゲームプレイシーンの更新処理を呼び出し，返り値を設定
		}
		break;

	case MakePuzzleScr:	// パズル作成シーン番号のとき
		nextScene = UpdateMakePuzzle(&(scrMgr->mMakePuzzle));	// パズル作成シーンの更新処理を呼び出し，返り値を設定
		break;
	}

	return nextScene;
}


/*********************************************************
*** Function Name : FinalizeScene (M3)
*** Designer      : 藤川
*** Date          : 2020.6.12
*** Function      : 現在のシーンごとのの終了処理呼び出し
*** Return        : void  --戻り値なし
**********************************************************/
void FinalizeScene(int scene, ScrMgr_t* scrMgr, Puzzle_t* puzzle) {
	// 引数の現在のシーン番号によって分岐
	switch (scene) {
	case InputNickNameScr:	// 起動シーン番号のとき
		FinalizeInputNickName(&(scrMgr->mInputNickName));	// 起動シーンの終了処理を呼び出す
		break;

	case MenuScr:	// メニューシーン番号のとき
		FinalizeMenu(&(scrMgr->mMenu));	// メニューシーンの終了処理を呼び出す
		break;

	case SelectionScr:	// 難易度・パズル選択シーン番号のとき
		FinalizeSelection(&(scrMgr->mSelection));	// 難易度・パズル選択シーンの終了処理を呼び出す
		break;

	case GameScr:	// チュートリアル/ゲームプレイシーン番号のとき
		FinalizeGame(&(scrMgr->mGame), puzzle);	// チュートリアル/ゲームプレイシーンの終了処理を呼び出す
		break;

	case MakePuzzleScr:	// パズル作成シーン番号のとき
		FinalizeMakePuzzle(&(scrMgr->mMakePuzzle));	// パズル作成シーンの終了処理を呼び出す
		break;

	case None:	// どのシーンでもないとき(メインループの最初のループのときに入る)何も実行しない
		break;
	}
}