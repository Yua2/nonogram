#include"Game.h"
#include"DxLib.h"
#include"SceneMgr.h"
int InitializeGame(Game_t* mGame) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "game");
	ScreenFlip();
	return GameScr;
}
int UpdateGame(Game_t* mGame, Puzzle_t* Puzzle) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "game");
	ScreenFlip();
	WaitKey();
	return MenuScr;
}
void FinalizeGame(Game_t* mGame) {

}