/******************************************
*** Designer : 藤川
*** Date     : 2020.7.7
*** Purpose  : チュートリアル/ゲームプレイ画面処理
*******************************************/

#include "DxLib.h"
#include "Game.h"
#include "Controller.h"
#include "Information.h"
#include "MouseState.h"
#include "EScene.h"

/********************************************
*** Function Name : InitializeGame
*** Designer      : 藤川
*** Date          : 2020.7.7
*** Function      : 引数でとったチュートリアル/ゲームプレイ画面の
                    構造体(Game_t)のメンバ()
*/
int InitializeGame(Game_t* game, Puzzle_t* puzzle) {
	Mouse_t mouse;
	GetMouseState(&mouse);

	//パズルのサイズから，パズルの左端座標と右端座標を取得
	if (puzzle->x_size == puzzle->y_size) {	// 正方形の場合
		game->puzzleX = 520;	// パズルの表示のx座標
		game->puzzleY = 160;	// パズルの表示のy座標
	}
	else if (puzzle->x_size > puzzle->y_size) {	// 横長の場合
		game->puzzleX = 520;	// パズルの表示のx座標
		game->puzzleY = 210;	// パズルの表示のy座標
	}
	else {	// 縦長の場合
		game->puzzleX = 570;	// パズルの表示のx座標
		game->puzzleY = 160;	// パズルの表示のy座標
	}

	//パズルのサイズから，パズルのマスのサイズを取得
	if (puzzle->x_size == 20 || puzzle->y_size == 20) {	// 縦横どちらか一辺が20の時
		game->puzzleGridSize = 27;	// パズルのマスのサイズ(20×20)
	}
	else if (puzzle->x_size == 15 || puzzle->y_size == 15) {	// 縦横どちらか一辺が15の時
		game->puzzleGridSize = 40;	// パズルのマスのサイズ(15×15)
	}
	else {	// 両辺とも10の時
		game->puzzleGridSize = 53;	// パズルのマスのサイズ(10×10)
	}

	// 塗る場所を指示する上と右の数列を0に初期化
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			game->drawGrid_H[i][j] = 0;
			game->drawGrid_V[i][j] = 0;
		}
	}
	
	for (int i = 0; i < puzzle->y_size; i++) {
		int vIndex = 0;
		for (int j = 0; j < puzzle->x_size; j++) {
			if (puzzle->puzzleData[i][j] > 100) {
				game->drawGrid_V[i][vIndex]++;
			}

			if (game->drawGrid_V[i][vIndex] != 0 && puzzle->puzzleData[i][j] <= 100) {
				vIndex++;
			}
		}
	}

	for (int i = 0; i < puzzle->x_size; i++) {
		int hIndex = 0;
		for (int j = 0; j < puzzle->y_size; j++) {
			if (puzzle->puzzleData[j][i] > 100) {
				game->drawGrid_H[i][hIndex]++;
			}

			if (game->drawGrid_H[i][hIndex] != 0 && puzzle->puzzleData[j][i] <= 100) {
				hIndex++;
			}
		}
	}

	for (int i = 0; i < puzzle->y_size; i++) {
		int count = 0;
		for (int j = 0; j < 10; j++) {
			if (game->drawGrid_V[i][j] > 0) {
				count++;
			}
		}

		for (int j = 0; j < count; j++) {
			int tmp = game->drawGrid_V[i][j];
			game->drawGrid_V[i][j] = game->drawGrid_V[i][10 - count + j];
			game->drawGrid_V[i][10 - count + j] = tmp;
		}
	}

	for (int i = 0; i < puzzle->x_size; i++) {
		int count = 0;
		for (int j = 0; j < 10; j++) {
			if (game->drawGrid_H[i][j] > 0) {
				count++;
			}
		}

		for (int j = 0; j < count; j++) {
			int tmp = game->drawGrid_H[i][j];
			game->drawGrid_H[i][j] = game->drawGrid_H[i][10 - count + j];
			game->drawGrid_H[i][10 - count + j] = tmp;
		}
	}

	game->checkPuzzle = *puzzle;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			game->checkPuzzle.puzzleData[i][j] = -1;
		}
	}

	int backImageHandle = LoadGraph("graph/backButton.bmp");
	int hintImageHandle = LoadGraph("graph/hintButton.bmp");
	int resetImageHandle = LoadGraph("graph/resetButton.bmp");
	int penImageHandle = LoadGraph("graph/penButton.bmp");
	int eraserImageHandle = LoadGraph("graph/eraserButton.bmp");
	game->lifeImageHandle = LoadGraph("graph/life1.png");
	game->lifeFontHandle = CreateFontToHandle(NULL, 35, 4);
	game->puzzleTitleFontHandle = CreateFontToHandle(NULL, 30, 4);
	game->startTimeFontHandle = CreateFontToHandle(NULL, 40, 4);
	game->drawNumFontHandle = CreateFontToHandle(NULL, 8, 2);

	setButton(55, 435, 145, 525, backImageHandle, NULL, mouse, &(game->backButton));
	setButton(175, 435, 265, 525, hintImageHandle, NULL, mouse, &(game->hintButton));
	setButton(295, 435, 385, 525, resetImageHandle, NULL, mouse, &(game->resetButton));
	setButton(100, 315, 190, 405, penImageHandle, NULL, mouse, &(game->penButton));
	setButton(250, 315, 340, 405, eraserImageHandle, NULL, mouse, &(game->eraserButton));
	game->penButton.mState = true;

	for (int i = 0, k = game->puzzleY; i < puzzle->y_size; i++, k += game->puzzleGridSize -1) {
		for (int j = 0, l = game->puzzleX; j < puzzle->x_size; j++, l += game->puzzleGridSize -1) {
			setButton(l, k, l + game->puzzleGridSize, k + game->puzzleGridSize, NULL, GetColor(255, 255, 255), mouse, &(game->puzzleGrid[i][j]));
			game->puzzleState[i][j] = false;
		}
	}

	DrawGraph(game->backButton.mX1, game->backButton.mY1, game->backButton.mImageHandle, FALSE);
	DrawGraph(game->hintButton.mX1, game->hintButton.mY1, game->hintButton.mImageHandle, FALSE);
	DrawGraph(game->resetButton.mX1, game->resetButton.mY1, game->resetButton.mImageHandle, FALSE);
	DrawGraph(game->penButton.mX1, game->penButton.mY1, game->penButton.mImageHandle, FALSE);
	DrawGraph(game->eraserButton.mX1, game->eraserButton.mY1, game->eraserButton.mImageHandle, FALSE);
	DrawGraph(170, 200, game->lifeImageHandle, TRUE);
	DrawFormatStringToHandle(210, 200, GetColor(0, 0, 0), game->lifeFontHandle, "×%d", game->lifeCounter);
	DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->startTimeFontHandle, "Time : 00:00");
	int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
	DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

	for (int i = 0; i < puzzle->y_size; i++) {
		for (int j = 0; j < puzzle->x_size; j++) {
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0,0,0), FALSE);
		}
	}

	int index = 0;
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX) - 100, game->puzzleY, GetColor(0, 0, 0));
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX), game->puzzleY - 100, GetColor(0, 0, 0));
	for (int i = 1; i <= puzzle->y_size; i++) {
		for (int j = 1; j <= puzzle->x_size; j++) {
			DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0));
			DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0));
		}
	}

	for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, x = ((game->puzzleX) - 100); j < 10; j++, x += 10) {
			if ((game->drawGrid_V[i][j]) != 0) {
				int drawY = y + (game->puzzleGridSize / 2) - 4;
				DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_V[i][j]));
			}
		}
	}

	for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, y = ((game->puzzleY) - 100); j < 10; j++, y += 10) {
			if ((game->drawGrid_H[i][j]) != 0) {
				int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "&d", game->drawGrid_H[i][j]) / 2);
				DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_H[i][j]));
			}
		}
	}
	
	game->startTime = GetNowCount();

	return GameScr;
}

// M17:チュートリアル画面更新
int UpdateTutorial(Game_t* game, Puzzle_t* puzzle) {
	Mouse_t mouse;
	GetMouseState(&mouse);

	bool penState = game->penButton.mState;
	bool eraserState = !penState;

	setButton(55, 435, 145, 525, game->backButton.mImageHandle, NULL, mouse, &(game->backButton));
	setButton(175, 435, 265, 525, game->hintButton.mImageHandle, NULL, mouse, &(game->hintButton));
	setButton(295, 435, 385, 525, game->resetButton.mImageHandle, NULL, mouse, &(game->resetButton));
	setButton(100, 315, 190, 405, game->penButton.mImageHandle, NULL, mouse, &(game->penButton));
	setButton(250, 315, 340, 405, game->eraserButton.mImageHandle, NULL, mouse, &(game->eraserButton));

	if (!game->penButton.mState) {
		if (!game->eraserButton.mState) {
			game->penButton.mState = penState;
			game->eraserButton.mState = eraserState;
		}
		else {
			game->eraserButton.mState = penState;
			game->penButton.mState = eraserState;
		}
	}

	if (game->backButton.mState) {
		return SelectionScr;
	}

	if (game->hintButton.mState) {

	}

	if (game->resetButton.mState) {
		for (int i = 0; i < puzzle->y_size; i++) {
			for (int j = 0; j < puzzle->x_size; j++) {
				game->puzzleState[i][j] = false;
				game->checkPuzzle.puzzleData[i][j] = 0;
				game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
			}
		}
	}

	int nowMiliSec = GetNowCount();
	int sec = ((nowMiliSec - (game->startTime)) % 60000) / 1000;
	int minute = (nowMiliSec - (game->startTime)) / 60000;
	int life = 0;
	DrawGraph(game->backButton.mX1, game->backButton.mY1, game->backButton.mImageHandle, FALSE);
	DrawGraph(game->hintButton.mX1, game->hintButton.mY1, game->hintButton.mImageHandle, FALSE);
	DrawGraph(game->resetButton.mX1, game->resetButton.mY1, game->resetButton.mImageHandle, FALSE);
	DrawGraph(game->penButton.mX1, game->penButton.mY1, game->penButton.mImageHandle, FALSE);
	DrawGraph(game->eraserButton.mX1, game->eraserButton.mY1, game->eraserButton.mImageHandle, FALSE);
	DrawGraph(170, 200, game->lifeImageHandle, TRUE);
	DrawFormatStringToHandle(210, 200, GetColor(0, 0, 0), game->lifeFontHandle, "×%d", game->lifeCounter);
	DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->startTimeFontHandle, "Time : %02d:%02d", minute, sec);
	int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
	DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

	for (int i = 0; i < puzzle->y_size; i++) {
		for (int j = 0; j < puzzle->x_size; j++) {
			bool preState = game->puzzleState[i][j];
			setButton(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, NULL, game->puzzleGrid[i][j].mColor, mouse, &(game->puzzleGrid[i][j]));
			if (game->puzzleGrid[i][j].mState && game->checkPuzzle.puzzleData[i][j] == -1) {
				game->puzzleState[i][j] = true;
				if (game->penButton.mState) {
					game->checkPuzzle.puzzleData[i][j] = 1;
				}
				else if (game->eraserButton.mState) {
					game->checkPuzzle.puzzleData[i][j] = 0;
				}

				if (puzzle->puzzleData[i][j] / 100 != game->checkPuzzle.puzzleData[i][j]) {
					life--;
					if (game->checkPuzzle.puzzleData[i][j] == 1) {
						game->checkPuzzle.puzzleData[i][j] = 0;
					}
					else if (game->checkPuzzle.puzzleData[i][j] == 0) {
						game->checkPuzzle.puzzleData[i][j] = 1;
					}
				}
			}

			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);

			if (game->puzzleState[i][j]) {
				if (game->checkPuzzle.puzzleData[i][j] == 1) {
					game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
				}
				else if (game->checkPuzzle.puzzleData[i][j] == 0) {
					DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), 1);
					DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 0), 1);
				}
			}
		}
	}

	int index = 0;
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX) - 100, game->puzzleY, GetColor(0, 0, 0));
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX), game->puzzleY - 100, GetColor(0, 0, 0));
	for (int i = 1; i <= puzzle->y_size; i++) {
		for (int j = 1; j <= puzzle->x_size; j++) {
			DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize-1)*j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize-1)*j, GetColor(0, 0, 0));
			DrawLine(game->puzzleX + (game->puzzleGridSize-1)*i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize-1)*i, (game->puzzleY) - 100, GetColor(0, 0, 0));
		}
	}

	int tmpLife = game->lifeCounter - 1;
	if (life < 0 && game->lifeCounter >= 1) {
		game->lifeCounter = tmpLife;
	}

	for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, x = ((game->puzzleX) - 100); j < 10; j++, x += 10) {
			if ((game->drawGrid_V[i][j]) != 0) {
				int drawY = y + (game->puzzleGridSize / 2) - 4;
				DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_V[i][j]));
			}
		}
	}

	for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, y = ((game->puzzleY) - 100); j < 10; j++, y += 10) {
			if ((game->drawGrid_H[i][j]) != 0) {
				int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "&d", game->drawGrid_H[i][j]) / 2);
				DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_H[i][j]));
			}
		}
	}

	return GameScr;
}

// M18:ゲームプレイ更新
int UpdateGame(Game_t* game, Puzzle_t* puzzle) {
	Mouse_t mouse;
	GetMouseState(&mouse);

	bool penState = game->penButton.mState;
	bool eraserState = !penState;

	setButton(55, 435, 145, 525, game->backButton.mImageHandle, NULL, mouse, &(game->backButton));
	setButton(175, 435, 265, 525, game->hintButton.mImageHandle, NULL, mouse, &(game->hintButton));
	setButton(295, 435, 385, 525, game->resetButton.mImageHandle, NULL, mouse, &(game->resetButton));
	setButton(100, 315, 190, 405, game->penButton.mImageHandle, NULL, mouse, &(game->penButton));
	setButton(250, 315, 340, 405, game->eraserButton.mImageHandle, NULL, mouse, &(game->eraserButton));

	if (!game->penButton.mState) {
		if (!game->eraserButton.mState) {
			game->penButton.mState = penState;
			game->eraserButton.mState = eraserState;
		}
		else {
			game->eraserButton.mState = penState;
			game->penButton.mState = eraserState;
		}
	}

	if (game->backButton.mState) {
		return SelectionScr;
	}

	if (game->hintButton.mState) {

	}

	if (game->resetButton.mState) {
		for (int i = 0; i < puzzle->y_size; i++) {
			for (int j = 0; j < puzzle->x_size; j++) {
				game->puzzleState[i][j] = false;
				game->checkPuzzle.puzzleData[i][j] = 0;
				game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
			}
		}
	}

	int nowMiliSec = GetNowCount();
	int msec = (((nowMiliSec - (game->startTime)) / 10) * 10 - ((nowMiliSec - (game->startTime)) / 1000) * 1000) / 10;
	int sec = ((nowMiliSec - (game->startTime)) % 60000) / 1000;
	int minute = (nowMiliSec - (game->startTime)) / 60000;

	DrawGraph(game->backButton.mX1, game->backButton.mY1, game->backButton.mImageHandle, FALSE);
	DrawGraph(game->hintButton.mX1, game->hintButton.mY1, game->hintButton.mImageHandle, FALSE);
	DrawGraph(game->resetButton.mX1, game->resetButton.mY1, game->resetButton.mImageHandle, FALSE);
	DrawGraph(game->penButton.mX1, game->penButton.mY1, game->penButton.mImageHandle, FALSE);
	DrawGraph(game->eraserButton.mX1, game->eraserButton.mY1, game->eraserButton.mImageHandle, FALSE);
	DrawGraph(170, 200, game->lifeImageHandle, TRUE);
	DrawFormatStringToHandle(210, 200, GetColor(0, 0, 0), game->lifeFontHandle, "×%d", game->lifeCounter);
	DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->startTimeFontHandle, "Time : %02d:%02d:%02d", minute, sec, msec);
	int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
	DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

	for (int i = 0; i < puzzle->y_size; i++) {
		for (int j = 0; j < puzzle->x_size; j++) {
			setButton(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, NULL, game->puzzleGrid[i][j].mColor, mouse, &(game->puzzleGrid[i][j]));
			if (game->puzzleGrid[i][j].mState) {
				game->puzzleState[i][j] = true;
				if (game->penButton.mState) {
					game->checkPuzzle.puzzleData[i][j] = 1;
				}
			}

			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);

			if (game->puzzleState[i][j]) {
				if (game->checkPuzzle.puzzleData[i][j] == 1) {
					game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
				}
				else {
					DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), 5);
					DrawLine(game->puzzleGrid[i][j + 1].mX1, game->puzzleGrid[i][j + 1].mY1, game->puzzleGrid[i + 1][j].mX1, game->puzzleGrid[i + 1][j].mY1, GetColor(0, 0, 0), 5);
				}
			}
		}
	}

	int index = 0;
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX) - 100, game->puzzleY, GetColor(0, 0, 0));
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX), game->puzzleY - 100, GetColor(0, 0, 0));
	for (int i = 0; i < puzzle->y_size; i++) {
		for (int j = 0; j < puzzle->x_size; j++) {
			DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize), (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize), GetColor(0, 0, 0));
			DrawLine(game->puzzleX + (game->puzzleGridSize), game->puzzleY, (game->puzzleX) + (game->puzzleGridSize), (game->puzzleY) - 100, GetColor(0, 0, 0));
		}
	}

	return GameScr;
}


// M19:チュートリアル/ゲームプレイ終了処理
void FinalizeGame(Game_t* game, Puzzle_t* puzzle) {
	InitGraph();
	InitFontToHandle();
}