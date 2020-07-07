#ifndef GAME_SCR_H_
#define GAME_SCR_H_

#include "MouseState.h"
#include "Controller.h"
#include "Information.h"
#include "EScene.h"

// ゲーム構造体
struct Game_t {
	int lifeCounter = 5;	// ライフの数(初期数=5)
	int lifeImageHandle;	// ライフの表示画像のイメージハンドル
	int lifeFontHandle;	// ライフの数の表示文字のフォントハンドル
	int startTime;	// ゲームプレイを始めた時間(PCが起動してからのミリ秒単位で経過した時間)
	int startTimeFontHandle;	// ゲームプレイ時間の表示文字のフォントハンドル
	int puzzleTitleFontHandle;	// プレイするパズルのパズルタイトルの表示文字のフォントハンドル
	Button_t penButton;	// 塗るボタンの表示画像のフォントハンドル
	Button_t eraserButton;	// ×印ボタンの表示画像のフォントハンドル
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
	Puzzle_t checkPuzzle;
};

// M16:チュートリアル/ゲームプレイ初期化
EScene InitializeGame(Game_t* game, Puzzle_t* puzzle);

// M17:チュートリアル画面更新
EScene UpdateTutorial(Game_t* game, Puzzle_t* puzzle);

// M18:ゲームプレイ更新
EScene UpdateGame(Game_t* game, Puzzle_t* puzzle);

// M19:チュートリアル/ゲームプレイ終了処理
void FinalizeGame(Game_t* game, Puzzle_t* puzzle);

#endif /* GAME_SCR_H_ */
