#include "Selection.h"
#include "SceneMgr.h"
#include "DxLib.h"
int InitializeSelection(Selection_t* mSelection) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "selection");
	ScreenFlip();
	return SelectionScr;
}
int UpdateSelection(Selection_t* mSelection, Puzzle_t* Puzzle) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "selection");
	ScreenFlip();
	WaitKey();
	return MenuScr;
}
void FinalizeSelection(Selection_t* mSelection) {

}