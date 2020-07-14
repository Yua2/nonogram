#ifndef GAME_SCR_H_
#define GAME_SCR_H_

#include "MouseState.h"
#include "Controller.h"
#include "Information.h"

// ゲーム構造体
struct Game_t {
	int startTime;	// ゲームプレイを始めた時間(PCが起動してからのミリ秒単位で経過した時間)
	int finishTime;	// ゲームクリア時の時間(PCが起動してからのミリ秒単位で経過した時間)
	int timeFontHandle;	// ゲームプレイ時間の表示文字のフォントハンドル
	int puzzleTitleFontHandle;	// プレイするパズルのパズルタイトルの表示文字のフォントハンドル
	Button_t penButton;	// 塗るボタンの表示画像のフォントハンドル
	int pressedPenButtonImageHandle;
	int releasePenButtonImageHandle;
	Button_t eraserButton;	// ×印ボタンの表示画像のフォントハンドル
	int pressedEraserButtonImageHandle;
	int releaseEraserButtonImageHandle;
	Button_t hintButton;	// ヒントボタンの表示画像のフォントハンドル
	Button_t resetButton;	// 
	Button_t backButton;
	int	puzzleX;
	int puzzleY;
	int puzzleGridSize;
	int drawGrid_V[20][10];
	int drawGrid_H[20][10];
	int drawNumFontHandle;
	Button_t puzzleGrid[20][20];
	bool puzzleState[20][20];
	bool toWhiteChange;
	bool fromWhiteChange;
	Puzzle_t checkPuzzle;
	bool clearFlag;
	int inputNicknameDisplayFlag;
	int rankingTitleFontHnadle;
	int rankingFontHandle;
	int playerRankingFontHandle;
	int keyHandle;
	int tutorialProcess;
	Button_t nextButton;
	int hintcounter;
};

// M16:チュートリアル/ゲームプレイ初期化
int InitializeGame(Game_t* game, Puzzle_t* puzzle);

// M17:チュートリアル画面更新
int UpdateTutorial(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse, int* key);

// M18:ゲームプレイ更新
int UpdateGame(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse, int* key);

// M19:チュートリアル/ゲームプレイ終了処理
void FinalizeGame(Game_t* game, Puzzle_t* puzzle);

#endif /* GAME_SCR_H_ */