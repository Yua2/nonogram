#include "DxLib.h"
#include "SceneMgr.h"
#include "MouseState.h"
#include "Information.h"

// M1:シーン初期化処理
EScene InitializeScene(EScene nextScene, ScrMgr_t* scrMgr) {
	EScene scene;

	switch (nextScene) {
	case(InputNickNameScr) :
		scene = InitializeInputNickName(&(scrMgr->mInputNickName));
		break;
	case(MenuScr):
		scene = InitializeMenu(&(scrMgr->mMenu));	
		break;
	case(SelectionScr):
		scene = InitializeSelection(&(scrMgr->mSelection));
		break;
	case(GameScr):
		scene = InitializeGame(&(scrMgr->mGame));
		break;
	case(MakePuzzleScr):
		scene = InitializeMakePuzzle(&(scrMgr->mMakePuzzle));
		break;
	}

	return scene;
}

// M2:シーン更新処理
EScene UpdateScene(EScene scene, ScrMgr_t* scrMgr, Mouse_t mouse, int* key, Puzzle *puzzle) {
	EScene nextScene;

	switch (scene) {
	case(InputNickNameScr):
		nextScene = UpdateInputNickName(&(scrMgr->mInputNickName));
		break;
	case(MenuScr):
		nextScene = UpdateMenu(&(scrMgr->mMenu));
		break;
	case(SelectionScr):
		nextScene = UpdateSelection(&(scrMgr->mSelection), &puzzle);
		break;
	case(GameScr):
		nextScene = UpdateGame(&(scrMgr->mGame), &puzzle);
		break;
	case(MakePuzzleScr):
		nextScene = UpdateMakePuzzle(&(scrMgr->mMakePuzzle));
		break;
	}

	return nextScene;
}


// M3:シーン終了処理
void FinalizeScene(EScene scene, ScrMgr_t* scrMgr) {
	switch (scene) {
	case(InputNickNameScr):
		FinalizeInputNickName(&(scrMgr->mInputNickName));
		break;
	case(MenuScr):
		FinalizeMenu(&(scrMgr->mMenu));
		break;
	case(SelectionScr):
		FinalizeSelection(&(scrMgr->mSelection));
		break;
	case(GameScr):
		FinalizeGame(&(scrMgr->mGame));
		break;
	case(MakePuzzleScr):
		FinalizeMakePuzzle(&(scrMgr->mMakePuzzle));
		break;
	}
}
