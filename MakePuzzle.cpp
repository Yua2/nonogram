#include "MakePuzzle.h"
#include "DxLib.h"
#include "SceneMgr.h"
int InitializeMakePuzzle(MakePuzzle_t* mMakePuzzle) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "makepuzzle");
	ScreenFlip();
	return MakePuzzleScr;
}
int UpdateMakePuzzle(MakePuzzle_t* mMakePuzzle) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "makepuzzle");
	ScreenFlip();
	WaitKey();
	return MenuScr;
}
void FinalizeMakePuzzle(MakePuzzle_t* mMakePuzzle) {

}