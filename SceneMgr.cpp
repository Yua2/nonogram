#include "DxLib.h"
#include "SceneMgr.h"
#include "MouseState.h"
#include "Information.h"

int InitializeScene(int nextScene, ScrMgr_t* scrMgr,Puzzle_t *puzzle) {
	int scene = 1;

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
		scene = InitializeGame(&(scrMgr->mGame),puzzle);
		break;
	case(MakePuzzleScr):
		scene = InitializeMakePuzzle(&(scrMgr->mMakePuzzle));
		break;
	}

	return scene;
}

// M2:シーン更新処理
int UpdateScene(int scene, ScrMgr_t* scrMgr, Mouse_t* mouse, int* key, Puzzle_t *puzzle) {
	int nextScene=1;

	switch (scene) {
	case(InputNickNameScr):
		nextScene = UpdateInputNickName(&(scrMgr->mInputNickName),mouse);
		break;
	case(MenuScr):
		nextScene = UpdateMenu(&(scrMgr->mMenu),mouse,puzzle);
		break;
	case(SelectionScr):
		nextScene = UpdateSelection(&(scrMgr->mSelection), puzzle,mouse);
		break;
	case(GameScr):
		nextScene = UpdateGame(&(scrMgr->mGame), puzzle,mouse,key);
		break;
	case(MakePuzzleScr):
		nextScene = UpdateMakePuzzle(&(scrMgr->mMakePuzzle));
		break;
	}

	return nextScene;
}


// M3:シーン終了処理
void FinalizeScene(int scene, ScrMgr_t* scrMgr,Puzzle_t *puzzle) {
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
		FinalizeGame(&(scrMgr->mGame),puzzle);
		break;
	case(MakePuzzleScr):
		FinalizeMakePuzzle(&(scrMgr->mMakePuzzle));
		break;
	}
}
