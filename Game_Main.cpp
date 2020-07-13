#include "DxLib.h"
#include "SceneMgr.h"
#include "MouseState.h"
#include "KeyboardState.h"
#include "Information.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	SetGraphMode(1080, 720, 16);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK),SetBackgroundColor(222, 184, 135);
	ScrMgr_t scrMgr;

	int scene=none;
	int nextScene = InputNickNameScr;

	int key[256];
	Mouse_t mouse;
	Puzzle_t pz;

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && scene != None) {
		GetKeyBoardState(key);
		GetMouseState(&mouse);

		if (mouse.mButton == none) {
			mouse.waitRelease = 0;
		}
		SetDrawScreen(DX_SCREEN_BACK);
		if (scene != nextScene) {	
			FinalizeScene(scene, &scrMgr,&pz);
			scene = InitializeScene(nextScene, &scrMgr,&pz);
		}

		nextScene = UpdateScene(scene, &scrMgr, &mouse, key, &pz);
	}

	DxLib_End();
	return 0;	
}