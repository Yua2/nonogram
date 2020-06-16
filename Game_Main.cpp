#include "DxLib.h"
#include "SceneMgr.h"
#include "MouseState.h"
#include "KeyboardState.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	ScrMgr_t scrMgr;	// シーン管理構造体

	EScene scene = None;		// 現在シーン
	EScene nextScene = None;	// 次のシーン

	int key[256];		// キーボード状態格納変数
	Mouse_t mouse;		// マウス状態確認変数

	Pazzle pz = tutorialPz;		// 遊ぶパズル(初期状態はチュートリアルパズル)

	scene = CheckPlayerID("ファイルパス");		// 起動時処理
	
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && scene != None) {
		GetKeyBoardState(key);		// キーボード状態取得
		GetMouseState(&mouse);		// マウス状態取得

		// シーンが遷移するならば
		if (scene != nextScene) {	
			scene = InitializeScene(nextScene, &scrMgr);	// 遷移先のシーンの初期化を実行して，今のシーンから遷移
			FinalizeScene(scene, &scrMgr);					// 遷移前ののシーンを終了
		}

		nextScene = UpdateScene(scene, &scrMgr, mouse, key, &pz);	// 今のシーンの更新(描画も含む)して遷移先のシーンを決める
	}

	DxLib_End();		    // ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}