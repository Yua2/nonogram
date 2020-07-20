/******************************************
*** Designer : 藤川
*** Date     : 2020.6.12
*** Purpose  : メイン処理
*******************************************/

#include "DxLib.h"
#include "SceneMgr.h"
#include "MouseState.h"
#include "Information.h"

/*****************************************************
*** Function Name : WinMain
*** Designer      : 藤川
*** Date          : 2020.7.20
*** Function      : メイン関数
*** Return        : 0 (プログラムの正常終了)
******************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetGraphMode(1080, 720, 16);	// ウィンドウサイズを1080×720に設定
	ChangeWindowMode(TRUE);	// ウィンドウモードに設定
	DxLib_Init();	// DxLib初期化
	SetDrawScreen(DX_SCREEN_BACK);	// 裏画面に設定
	SetBackgroundColor(222, 184, 135);	// 背景色を(R, G, B) = (222, 184, 135)に設定
	
	ScrMgr_t scrMgr;	// 表示するシーンを持つシーン構造体の変数

	int scene = None;	// 現在のシーン番号を格納する変数
	int nextScene = InputNickNameScr;	// 次の遷移先のシーンを格納する変数

	Mouse_t mouse;	// 使われているマウスに対応するマウス変数

	Puzzle_t pz;	// 遊ぶパズルのパズル変数

	// メインループ
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		GetMouseState(&mouse, TRUE);	// マウスの状態更新(C1:M4)

		// マウスのボタンが押されていなければ
		if (mouse.mButton == none) {
			mouse.waitRelease = 0;	// 押しっぱなし状態から変更
		}

		SetDrawScreen(DX_SCREEN_BACK);	// 裏画面に設定

		// 現在のシーンと次のシーンが異なっていたら
		if (scene != nextScene) {
			FinalizeScene(scene, &scrMgr, &pz);	// 現在のシーンに対して終了処理(C1:M3)
			scene = InitializeScene(nextScene, &scrMgr, &pz);// 次のシーンを初期化(C1:M1)して現在のシーンにセット
		}

		nextScene = UpdateScene(scene, &scrMgr, &mouse, &pz);	// 現在のシーンを更新(C1:M2)して次のシーンにセット
	}

	DxLib_End();	// DxLib狩猟
	return 0;	// 正常終了
}