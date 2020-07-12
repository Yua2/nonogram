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
#include "SceneMgr.h"
#include "MakePuzzle.h"

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

	//パズルのサイズから，パズルのマスのサイズを取得
	if (puzzle->x_size == 20 || puzzle->y_size == 20) {	// 縦横どちらか一辺が20の時
		game->puzzleGridSize = 27;	// パズルのマスのサイズ(20×20)
	}
	else if (puzzle->x_size == 15 || puzzle->y_size == 15) {	// 縦横どちらか一辺が15の時
		game->puzzleGridSize = 35;	// パズルのマスのサイズ(15×15)
	}
	else {	// 両辺とも10の時
		game->puzzleGridSize = 54;	// パズルのマスのサイズ(10×10)
	}

	// 塗る場所を指示する上と右の数列を0に初期化
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 20; i++) {
			game->drawGrid_H[i][j] = 0;
			game->drawGrid_V[i][j] = 0;
		}
	}

	for (int j = 0; j < puzzle->y_size; j++) {
		int vIndex = 0;
		for (int i = 0; i < puzzle->x_size; i++) {
			if (puzzle->puzzleData[i][j] > 100) {
				game->drawGrid_V[j][vIndex]++;
			}
			else if (game->drawGrid_V[j][vIndex] > 0) {
				vIndex++;
			}
		}
	}

	for (int j = 0; j < puzzle->x_size; j++) {
		int hIndex = 0;
		for (int i = 0; i < puzzle->y_size; i++) {
			if (puzzle->puzzleData[j][i] > 100) {
				game->drawGrid_H[j][hIndex]++;
			}
			else if (game->drawGrid_H[j][hIndex] > 0) {
				hIndex++;
			}
		}
	}

	for (int i = 0; i < puzzle->y_size; i++) {
		int tmp_V[10];
		int tmp_H[10];
		for (int j = 0; j < 10; j++) {
			tmp_V[j] = game->drawGrid_V[i][9 - j];
			tmp_H[j] = game->drawGrid_H[i][9 - j];
		}

		int count_V = 0;
		int count_H = 0;
 		for (int j = 0; j < 10; j++) {
			if (tmp_V[j] > 0) {
				game->drawGrid_V[i][count_V] = tmp_V[j];
				count_V++;
			}
			else {
				game->drawGrid_V[i][9 - j] = tmp_V[j];
			}

			if (tmp_H[j] > 0) {
				game->drawGrid_H[i][count_H] = tmp_H[j];
				count_H++;
			}
			else {
				game->drawGrid_H[i][9 - j] = tmp_H[j];
			}
		}
	}

	game->checkPuzzle = *puzzle;
	for (int j = 0; j < 20; j++) {
		for (int i = 0; i < 20; i++) {
			game->checkPuzzle.puzzleData[i][j] = -1;
		}
	}

	int backImageHandle = LoadGraph("graph/backButton.bmp");
	int hintImageHandle = LoadGraph("graph/hintButton.bmp");
	int resetImageHandle = LoadGraph("graph/resetButton.bmp");
	int penImageHandle = LoadGraph("graph/penButton.bmp");
	int eraserImageHandle = LoadGraph("graph/eraserButton.bmp");
	game->pressedPenButtonImageHandle = LoadGraph("graph/penbutton_pressed.bmp");
	game->releasePenButtonImageHandle = LoadGraph("graph/penbutton.bmp");
	game->pressedEraserButtonImageHandle = LoadGraph("graph/eraserbutton_pressed.bmp");
	game->releaseEraserButtonImageHandle = LoadGraph("graph/eraserbutton.bmp");
	game->puzzleTitleFontHandle = CreateFontToHandle(NULL, 30, 4);
	game->timeFontHandle = CreateFontToHandle(NULL, 40, 4);
	game->drawNumFontHandle = CreateFontToHandle(NULL, 8, 2);
	game->rankingTitleFontHnadle = CreateFontToHandle(NULL, 34, 3);
	game->rankingFontHandle = CreateFontToHandle(NULL, 50, 3);
	game->playerRankingFontHandle = CreateFontToHandle(NULL, 30, 3);
	game->keyHandle = MakeKeyInput(32, FALSE, FALSE, FALSE);

	setButton(55, 435, 145, 525, backImageHandle, NULL, mouse, &(game->backButton));
	setButton(175, 435, 265, 525, hintImageHandle, NULL, mouse, &(game->hintButton));
	setButton(295, 435, 385, 525, resetImageHandle, NULL, mouse, &(game->resetButton));
	setButton(100, 315, 190, 405, game->pressedPenButtonImageHandle, NULL, mouse, &(game->penButton));
	setButton(250, 315, 340, 405, game->releaseEraserButtonImageHandle, NULL, mouse, &(game->eraserButton));
	game->penButton.mState = true;
	game->clearFlag = true;
	game->inputNicknameDisplayFlag = -1;

	for (int j = 0, k = game->puzzleY; j < puzzle->y_size; j++, k += game->puzzleGridSize - 1) {
		for (int i = 0, l = game->puzzleX; i < puzzle->x_size; i++, l += game->puzzleGridSize - 1) {
			setButton(l, k, l + game->puzzleGridSize, k + game->puzzleGridSize, NULL, GetColor(255, 255, 255), mouse, &(game->puzzleGrid[i][j]));
			game->puzzleState[i][j] = false;
		}
	}

	DrawGraph(game->backButton.mX1, game->backButton.mY1, game->backButton.mImageHandle, FALSE);
	DrawGraph(game->hintButton.mX1, game->hintButton.mY1, game->hintButton.mImageHandle, FALSE);
	DrawGraph(game->resetButton.mX1, game->resetButton.mY1, game->resetButton.mImageHandle, FALSE);
	DrawGraph(game->penButton.mX1, game->penButton.mY1, game->penButton.mImageHandle, FALSE);
	DrawGraph(game->eraserButton.mX1, game->eraserButton.mY1, game->eraserButton.mImageHandle, FALSE);
	DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->timeFontHandle, "Time : 00:00");
	int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
	DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

	for (int j = 0; j < puzzle->y_size; j++) {
		for (int i = 0; i < puzzle->x_size; i++) {
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
		}
	}

	int index = 0;
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX) - 100, game->puzzleY, GetColor(0, 0, 0));
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX), game->puzzleY - 100, GetColor(0, 0, 0));
	for (int j = 1; j <= puzzle->y_size; j++) {
		for (int i = 1; i <= puzzle->x_size; i++) {
			DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0));
			DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0));
		}
	}

	for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, x = ((game->puzzleX) - 10); j < 10; j++, x -= 10) {
			if ((game->drawGrid_V[i][j]) != 0) {
				int drawY = y + (game->puzzleGridSize / 2) - 4;
				DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_V[i][j]));
			}
		}
	}

	for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, y = ((game->puzzleY) - 10); j < 10; j++, y -= 10) {
			if ((game->drawGrid_H[i][j]) != 0) {
				int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "%d", game->drawGrid_H[i][j]) / 2);
				DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_H[i][j]));
			}
		}
	}

	game->startTime = GetNowCount();

	return GameScr;
}

// M17:チュートリアル画面更新
int UpdateTutorial(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse, int* key) {
	GetMouseState(mouse);

	if (game->clearFlag) {
		bool penState = game->penButton.mState;
		bool eraserState = !penState;

		bool hintState = game->hintButton.mState;

		setButton(55, 435, 145, 525, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton));
		setButton(175, 435, 265, 525, game->hintButton.mImageHandle, NULL, *mouse, &(game->hintButton));
		setButton(295, 435, 385, 525, game->resetButton.mImageHandle, NULL, *mouse, &(game->resetButton));
		setButton(100, 315, 190, 405, game->penButton.mImageHandle, NULL, *mouse, &(game->penButton));
		setButton(250, 315, 340, 405, game->eraserButton.mImageHandle, NULL, *mouse, &(game->eraserButton));

		if (penState) {
			if (game->eraserButton.mState) {
				game->eraserButton.mState = true;
				game->penButton.mState = false;
			}
			else {
				game->penButton.mState = true;
				game->eraserButton.mState = false;
			}
		}
		else {
			if (game->penButton.mState) {
				game->penButton.mState = true;
				game->eraserButton.mState = false;
			}
			else {
				game->eraserButton.mState = true;
				game->penButton.mState = false;
			}
		}

		if (game->penButton.mState) {
			game->penButton.mImageHandle = game->pressedPenButtonImageHandle;
			game->eraserButton.mImageHandle = game->releaseEraserButtonImageHandle;
		}
		else if (game->eraserButton.mState) {
			game->eraserButton.mImageHandle = game->pressedEraserButtonImageHandle;
			game->penButton.mImageHandle = game->releasePenButtonImageHandle;
		}

		if (game->backButton.mState) {
			mouse->waitRelease = 1;
			return MenuScr;
		}

		if (game->hintButton.mState) {
			if (!hintState) {
				int i = GetRand(puzzle->y_size - 1);
				int j = GetRand(puzzle->x_size - 1);
				while (game->puzzleState[i][j]) {
					i = GetRand(puzzle->y_size - 1);
					j = GetRand(puzzle->x_size - 1);
				}
				game->puzzleState[i][j] = true;
				if (puzzle->puzzleData[i][j] > 100) {
					game->checkPuzzle.puzzleData[i][j] = 1;
				}
				else if (puzzle->puzzleData[i][j] < 100) {
					game->checkPuzzle.puzzleData[i][j] = 0;
				}
			}
		}

		if (game->resetButton.mState) {
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					game->puzzleState[i][j] = false;
					game->checkPuzzle.puzzleData[i][j] = -1;
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
		DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->timeFontHandle, "Time : %02d:%02d", minute, sec);
		int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
		DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				Button_t preState = game->puzzleGrid[i][j];
				setButton(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, NULL, game->puzzleGrid[i][j].mColor, *mouse, &(game->puzzleGrid[i][j]));

				if (mouse->mX > game->puzzleGrid[i][j].mX1 && mouse->mX < game->puzzleGrid[i][j].mX2 && mouse->mY > game->puzzleGrid[i][j].mY1 && mouse->mY < game->puzzleGrid[i][j].mY2 && mouse->mButton == right) {
					game->puzzleGrid[i][j].mState = true;
				}

				if (preState.mState != game->puzzleGrid[i][j].mState) {
					if (game->puzzleGrid[i][j].mState && game->checkPuzzle.puzzleData[i][j] == -1 && mouse->mButton != right) {
						game->puzzleState[i][j] = true;
						if (game->penButton.mState) {
							game->checkPuzzle.puzzleData[i][j] = 1;
						}
						else if (game->eraserButton.mState) {
							game->checkPuzzle.puzzleData[i][j] = 0;
						}
					}
					else if (game->puzzleGrid[i][j].mState && game->checkPuzzle.puzzleData[i][j] != -1 && mouse->mButton != right) {
						game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
						game->puzzleState[i][j] = false;
						game->checkPuzzle.puzzleData[i][j] = -1;
					}
					else if (mouse->mButton == right) {
						game->puzzleState[i][j] = true;
						game->checkPuzzle.puzzleData[i][j] = 0;
					}
				}

				if (game->puzzleState[i][j]) {
					if (game->checkPuzzle.puzzleData[i][j] == 1) {
						game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
					}
					else if (game->checkPuzzle.puzzleData[i][j] == 0) {
						game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), 1);
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 0), 1);
					}
				}
				else {
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
				}
			}
		}

		int index = 0;
		DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX) - 100, game->puzzleY, GetColor(0, 0, 0));
		DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX), game->puzzleY - 100, GetColor(0, 0, 0));
		for (int j = 1; j <= puzzle->y_size; j++) {
			for (int i = 1; i <= puzzle->x_size; i++) {
				DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0));
				DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0));
			}
		}

		for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, x = ((game->puzzleX) - 10); j < 10; j++, x -= 10) {
				if ((game->drawGrid_V[i][j]) != 0) {
					int drawY = y + (game->puzzleGridSize / 2) - 4;
					DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_V[i][j]));
				}
			}
		}

		for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, y = ((game->puzzleY) - 10); j < 10; j++, y -= 10) {
				if ((game->drawGrid_H[i][j]) != 0) {
					int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "%d", game->drawGrid_H[i][j]) / 2);
					DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_H[i][j]));
				}
			}
		}

		int diff = 0;
		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				if (puzzle->puzzleData[i][j] > 100) {
					if (game->checkPuzzle.puzzleData[i][j] != 1) {
						diff++;
					}
				}
				else if (puzzle->puzzleData[i][j] < 100) {
					if (game->checkPuzzle.puzzleData[i][j] == 1) {
						diff++;
					}
				}
			}
		}

		

		if (diff == 0) {
			game->clearFlag = false;
			game->finishTime = GetNowCount();
		}
	}
	else {
		MakePuzzle_t color;
		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				if (puzzle->puzzleData[i][j] == 0) {
					game->puzzleGrid[i][j].mColor = color.white;
				}
				else if (puzzle->puzzleData[i][j] == 1) {
					game->puzzleGrid[i][j].mColor = color.gainsboro;
				}
				else if (puzzle->puzzleData[i][j] == 2) {
					game->puzzleGrid[i][j].mColor = color.aqua;
				}
				else if (puzzle->puzzleData[i][j] == 3) {
					game->puzzleGrid[i][j].mColor = color.springgreen;
				}
				else if (puzzle->puzzleData[i][j] == 4) {
					game->puzzleGrid[i][j].mColor = color.red;
				}
				else if (puzzle->puzzleData[i][j] == 5) {
					game->puzzleGrid[i][j].mColor = color.pink;
				}
				else if (puzzle->puzzleData[i][j] == 6) {
					game->puzzleGrid[i][j].mColor = color.orange;
				}
				else if (puzzle->puzzleData[i][j] == 7) {
					game->puzzleGrid[i][j].mColor = color.lemonchiffon;
				}
				else if (puzzle->puzzleData[i][j] == 8) {
					game->puzzleGrid[i][j].mColor = color.mediumpurple;
				}
				else if (puzzle->puzzleData[i][j] == 101) {
					game->puzzleGrid[i][j].mColor = color.black;
				}
				else if (puzzle->puzzleData[i][j] == 102) {
					game->puzzleGrid[i][j].mColor = color.blue;
				}
				else if (puzzle->puzzleData[i][j] == 103) {
					game->puzzleGrid[i][j].mColor = color.green;
				}
				else if (puzzle->puzzleData[i][j] == 104) {
					game->puzzleGrid[i][j].mColor = color.maroon;
				}
				else if (puzzle->puzzleData[i][j] == 105) {
					game->puzzleGrid[i][j].mColor = color.deeppink;
				}
				else if (puzzle->puzzleData[i][j] == 106) {
					game->puzzleGrid[i][j].mColor = color.saddlebrown;
				}
				else if (puzzle->puzzleData[i][j] == 107) {
					game->puzzleGrid[i][j].mColor = color.gold;
				}
				else if (puzzle->puzzleData[i][j] == 108) {
					game->puzzleGrid[i][j].mColor = color.indigo;
				}

				DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
				DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
			}
		}

		int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
		DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

		DrawBox(20, 20, 500, 590, GetColor(255, 255, 255), TRUE);
		DrawBox(20, 20, 500, 590, GetColor(0, 0, 0), FALSE);
		for (int i = 0; i < 10; i++) {
			DrawLine(20, 50 + (54 * i), 500, 50 + (54 * i), GetColor(0, 0, 0), 1);
		}
		int drawRankingLeftLineX = 24 + GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%2d", 10);
		int drawRankingRightLineX = 496 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "9999秒");
		DrawLine(drawRankingLeftLineX, 50, drawRankingLeftLineX, 590, GetColor(0, 0, 0), 1);
		DrawLine(drawRankingRightLineX, 50, drawRankingRightLineX, 590, GetColor(0, 0, 0), 1);

		int drawTitleX = 270 - (GetDrawFormatStringWidthToHandle(game->rankingTitleFontHnadle, "ランキング") / 2);
		DrawFormatStringToHandle(drawTitleX, 21, GetColor(0, 0, 0), game->puzzleTitleFontHandle, "ランキング");
		for (int i = 0; i < 10; i++) {
			int drawRankingNumX = drawRankingLeftLineX - 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%2d", 10) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%d", i + 1) / 2;
			DrawFormatStringToHandle(drawRankingNumX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, "%d", i + 1);
			int drawRankingNameX = drawRankingRightLineX - (drawRankingRightLineX - drawRankingLeftLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, puzzle->ranking[i].nickName) / 2;
			DrawFormatStringToHandle(drawRankingNameX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, puzzle->ranking[i].nickName);
			int drawRankingTimeX = drawRankingRightLineX + 2 + (500 - drawRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime) / 2;
			DrawFormatStringToHandle(drawRankingTimeX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime);
		}

		DrawGraph(215, 610, game->backButton.mImageHandle, FALSE);
		setButton(215, 610, 305, 700, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton));

		if (game->backButton.mState) {
			mouse->waitRelease = 1;
			if (game->inputNicknameDisplayFlag == 1) {
				return MenuScr;
			}
		}


		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(255, 255, 255), TRUE);
		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(0, 0, 0), FALSE);
		int drawPlayerRankingLeftLineX = game->puzzleX + 10 + GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "ランク外");
		int drawPlayerRankingRightLineX = game->puzzleX + 400 - 10 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "9999秒");
		DrawLine(drawPlayerRankingLeftLineX, game->puzzleY - 100, drawPlayerRankingLeftLineX, game->puzzleY - 20, GetColor(0, 0, 0), 1);
		DrawLine(drawPlayerRankingRightLineX, game->puzzleY - 100, drawPlayerRankingRightLineX, game->puzzleY - 20, GetColor(0, 0, 0), 1);
		int time = (game->finishTime / 1000) - (game->startTime / 1000);
		int rank = 0;
		for (int i = 0; i < 10; i++) {
			if (puzzle->ranking[i].cleartime > time) {
				rank = i + 1;
				if (game->inputNicknameDisplayFlag == -1) {
					game->inputNicknameDisplayFlag = 0;
				}
				SetActiveKeyInput(game->keyHandle);
				break;
			}
		}
		int drawPlayerRankingNumX = drawPlayerRankingLeftLineX - (drawPlayerRankingLeftLineX - game->puzzleX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "%d", rank) / 2;
		int drawPlayerRankingNameX = drawPlayerRankingRightLineX - (drawPlayerRankingRightLineX - drawPlayerRankingLeftLineX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "あなた") / 2;
		int drawPlayerRankingTimeX = drawPlayerRankingRightLineX + (game->puzzleX + 400 - drawPlayerRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "%d秒", time) / 2;
		if (rank > 0) {
			DrawFormatStringToHandle(drawPlayerRankingNumX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "%d", rank);
		}
		else {
			DrawFormatStringToHandle(game->puzzleX + 5, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "ランク外");
		}
		DrawFormatStringToHandle(drawPlayerRankingNameX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "あなた");
		DrawFormatStringToHandle(drawPlayerRankingTimeX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "%d秒", time);

		if (game->inputNicknameDisplayFlag == 0) {
			DrawBox(350, 200, 800, 500, GetColor(200, 200, 200), TRUE);
			DrawBox(350, 200, 800, 500, GetColor(0, 0, 0), FALSE);
			DrawBox(370, 450, 440, 490, GetColor(255, 255, 255), TRUE);
			DrawBox(370, 450, 440, 490, GetColor(0, 0, 0), FALSE);
			DrawFormatString(395, 465, GetColor(0, 0, 0), "OK");

			int str1X = 350 + (800 - 350) / 2 - GetDrawFormatStringWidth("ランキングTOP10入りおめでとうございます!!") / 2;
			int str2X = 350 + (800 - 350) / 2 - GetDrawFormatStringWidth("ランキングに乗せるニックネームを入力してください") / 2;
			DrawFormatString(str1X, 210, GetColor(0, 0, 0), "ランキングTOP10入りおめでとうございます!!");
			DrawFormatString(str2X, 230, GetColor(0, 0, 0), "ランキングに乗せるニックネームを入力してください");

			DrawBox(460, 340, 700, 370, GetColor(0, 0, 0), FALSE);

			Button_t tmpButton;
			setButton(370, 450, 440, 490, NULL, NULL, *mouse, &tmpButton);
			if (tmpButton.mState) {
				game->inputNicknameDisplayFlag = 1;
				DeleteKeyInput(game->keyHandle);
			}
			else {
				int black = GetColor(255, 255, 255);
				SetKeyInputStringColor(black, black, black, black, black, black, black);
				DrawKeyInputString(462, 344, game->keyHandle);
			}
		}
	}

	mouse->waitRelease = 1;
	return GameScr;
}

// M18:ゲームプレイ更新
int UpdateGame(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse, int* key) {
	GetMouseState(mouse);

	if (game->clearFlag) {
		bool penState = game->penButton.mState;
		bool eraserState = !penState;

		bool hintState = game->hintButton.mState;

		setButton(55, 435, 145, 525, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton));
		setButton(175, 435, 265, 525, game->hintButton.mImageHandle, NULL, *mouse, &(game->hintButton));
		setButton(295, 435, 385, 525, game->resetButton.mImageHandle, NULL, *mouse, &(game->resetButton));
		setButton(100, 315, 190, 405, game->penButton.mImageHandle, NULL, *mouse, &(game->penButton));
		setButton(250, 315, 340, 405, game->eraserButton.mImageHandle, NULL, *mouse, &(game->eraserButton));

		if (penState) {
			if (game->eraserButton.mState) {
				game->eraserButton.mState = true;
				game->penButton.mState = false;
			}
			else {
				game->penButton.mState = true;
				game->eraserButton.mState = false;
			}
		}
		else {
			if (game->penButton.mState) {
				game->penButton.mState = true;
				game->eraserButton.mState = false;
			}
			else {
				game->eraserButton.mState = true;
				game->penButton.mState = false;
			}
		}

		if (game->penButton.mState) {
			game->penButton.mImageHandle = game->pressedPenButtonImageHandle;
			game->eraserButton.mImageHandle = game->releaseEraserButtonImageHandle;
		}
		else if (game->eraserButton.mState) {
			game->eraserButton.mImageHandle = game->pressedEraserButtonImageHandle;
			game->penButton.mImageHandle = game->releasePenButtonImageHandle;
		}

		if (game->backButton.mState) {
			mouse->waitRelease = 1;
			return SelectionScr;
		}

		if (game->hintButton.mState) {
			if (!hintState) {
				int i = GetRand(puzzle->y_size - 1);
				int j = GetRand(puzzle->x_size - 1);
				while (game->puzzleState[i][j]) {
					i = GetRand(puzzle->y_size - 1);
					j = GetRand(puzzle->x_size - 1);
				}
				game->puzzleState[i][j] = true;
				if (puzzle->puzzleData[i][j] > 100) {
					game->checkPuzzle.puzzleData[i][j] = 1;
				}
				else if (puzzle->puzzleData[i][j] < 100) {
					game->checkPuzzle.puzzleData[i][j] = 0;
				}
			}
		}

		if (game->resetButton.mState) {
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					game->puzzleState[i][j] = false;
					game->checkPuzzle.puzzleData[i][j] = -1;
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
		DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->timeFontHandle, "Time : %02d:%02d", minute, sec);
		int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
		DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				Button_t preState = game->puzzleGrid[i][j];
				setButton(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, NULL, game->puzzleGrid[i][j].mColor, *mouse, &(game->puzzleGrid[i][j]));

				if (mouse->mX > game->puzzleGrid[i][j].mX1 && mouse->mX < game->puzzleGrid[i][j].mX2 && mouse->mY > game->puzzleGrid[i][j].mY1 && mouse->mY < game->puzzleGrid[i][j].mY2 && mouse->mButton == right) {
					game->puzzleGrid[i][j].mState = true;
				}

				if (preState.mState != game->puzzleGrid[i][j].mState) {
					if (game->puzzleGrid[i][j].mState && game->checkPuzzle.puzzleData[i][j] == -1 && mouse->mButton != right) {
						game->puzzleState[i][j] = true;
						if (game->penButton.mState) {
							game->checkPuzzle.puzzleData[i][j] = 1;
						}
						else if (game->eraserButton.mState) {
							game->checkPuzzle.puzzleData[i][j] = 0;
						}
					}
					else if (game->puzzleGrid[i][j].mState && game->checkPuzzle.puzzleData[i][j] != -1 && mouse->mButton != right) {
						game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
						game->puzzleState[i][j] = false;
						game->checkPuzzle.puzzleData[i][j] = -1;
					}
					else if (mouse->mButton == right) {
						game->puzzleState[i][j] = true;
						game->checkPuzzle.puzzleData[i][j] = 0;
					}
				}

				if (game->puzzleState[i][j]) {
					if (game->checkPuzzle.puzzleData[i][j] == 1) {
						game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
					}
					else if (game->checkPuzzle.puzzleData[i][j] == 0) {
						game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
						DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), 1);
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 0), 1);
					}
				}
				else {
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
				}
			}
		}

		int index = 0;
		DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX) - 100, game->puzzleY, GetColor(0, 0, 0));
		DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX), game->puzzleY - 100, GetColor(0, 0, 0));
		for (int j = 1; j <= puzzle->y_size; j++) {
			for (int i = 1; i <= puzzle->x_size; i++) {
				DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0));
				DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0));
			}
		}

		for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, x = ((game->puzzleX) - 10); j < 10; j++, x -= 10) {
				if ((game->drawGrid_V[i][j]) != 0) {
					int drawY = y + (game->puzzleGridSize / 2) - 4;
					DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_V[i][j]));
				}
			}
		}

		for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, y = ((game->puzzleY) - 10); j < 10; j++, y -= 10) {
				if ((game->drawGrid_H[i][j]) != 0) {
					int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "%d", game->drawGrid_H[i][j]) / 2);
					DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_H[i][j]));
				}
			}
		}

		int diff = 0;
		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				if (puzzle->puzzleData[i][j] > 100) {
					if (game->checkPuzzle.puzzleData[i][j] != 1) {
						diff++;
					}
				}
				else if (puzzle->puzzleData[i][j] < 100) {
					if (game->checkPuzzle.puzzleData[i][j] == 1) {
						diff++;
					}
				}
			}
		}

		DrawFormatString(0, 0, GetColor(0, 0, 0), "%d", diff);

		if (diff == 0) {
			game->clearFlag = false;
			game->finishTime = GetNowCount();
		}
	}
	else {
		MakePuzzle_t color;
		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				if (puzzle->puzzleData[i][j] == 0) {
					game->puzzleGrid[i][j].mColor = color.white;
				}
				else if (puzzle->puzzleData[i][j] == 1) {
					game->puzzleGrid[i][j].mColor = color.gainsboro;
				}
				else if (puzzle->puzzleData[i][j] == 2) {
					game->puzzleGrid[i][j].mColor = color.aqua;
				}
				else if (puzzle->puzzleData[i][j] == 3) {
					game->puzzleGrid[i][j].mColor = color.springgreen;
				}
				else if (puzzle->puzzleData[i][j] == 4) {
					game->puzzleGrid[i][j].mColor = color.red;
				}
				else if (puzzle->puzzleData[i][j] == 5) {
					game->puzzleGrid[i][j].mColor = color.pink;
				}
				else if (puzzle->puzzleData[i][j] == 6) {
					game->puzzleGrid[i][j].mColor = color.orange;
				}
				else if (puzzle->puzzleData[i][j] == 7) {
					game->puzzleGrid[i][j].mColor = color.lemonchiffon;
				}
				else if (puzzle->puzzleData[i][j] == 8) {
					game->puzzleGrid[i][j].mColor = color.mediumpurple;
				}
				else if (puzzle->puzzleData[i][j] == 101) {
					game->puzzleGrid[i][j].mColor = color.black;
				}
				else if (puzzle->puzzleData[i][j] == 102) {
					game->puzzleGrid[i][j].mColor = color.blue;
				}
				else if (puzzle->puzzleData[i][j] == 103) {
					game->puzzleGrid[i][j].mColor = color.green;
				}
				else if (puzzle->puzzleData[i][j] == 104) {
					game->puzzleGrid[i][j].mColor = color.maroon;
				}
				else if (puzzle->puzzleData[i][j] == 105) {
					game->puzzleGrid[i][j].mColor = color.deeppink;
				}
				else if (puzzle->puzzleData[i][j] == 106) {
					game->puzzleGrid[i][j].mColor = color.saddlebrown;
				}
				else if (puzzle->puzzleData[i][j] == 107) {
					game->puzzleGrid[i][j].mColor = color.gold;
				}
				else if (puzzle->puzzleData[i][j] == 108) {
					game->puzzleGrid[i][j].mColor = color.indigo;
				}

				DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
				DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
			}
		}

		int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);
		DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);

		DrawBox(20, 20, 500, 590, GetColor(255, 255, 255), TRUE);
		DrawBox(20, 20, 500, 590, GetColor(0, 0, 0), FALSE);
		for (int i = 0; i < 10; i++) {
			DrawLine(20, 50 + (54 * i), 500, 50 + (54 * i), GetColor(0, 0, 0), 1);
		}
		int drawRankingLeftLineX = 24 + GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%2d", 10);
		int drawRankingRightLineX = 496 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "9999秒");
		DrawLine(drawRankingLeftLineX, 50, drawRankingLeftLineX, 590, GetColor(0, 0, 0), 1);
		DrawLine(drawRankingRightLineX, 50, drawRankingRightLineX, 590, GetColor(0, 0, 0), 1);

		int drawTitleX = 270 - (GetDrawFormatStringWidthToHandle(game->rankingTitleFontHnadle, "ランキング") / 2);
		DrawFormatStringToHandle(drawTitleX, 21, GetColor(0, 0, 0), game->puzzleTitleFontHandle, "ランキング");
		for (int i = 0; i < 10; i++) {
			int drawRankingNumX = drawRankingLeftLineX - 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%2d", 10) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%d", i+1) / 2;
			DrawFormatStringToHandle(drawRankingNumX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, "%d", i+1);
			int drawRankingNameX = drawRankingRightLineX - (drawRankingRightLineX - drawRankingLeftLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, puzzle->ranking[i].nickName) / 2;
			DrawFormatStringToHandle(drawRankingNameX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, puzzle->ranking[i].nickName);
			int drawRankingTimeX = drawRankingRightLineX + 2 + (500 - drawRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime) / 2;
			DrawFormatStringToHandle(drawRankingTimeX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime);
		}

		DrawGraph(215, 610, game->backButton.mImageHandle, FALSE);
		setButton(215, 610, 305, 700, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton));
	
		if (game->backButton.mState) {
			mouse->waitRelease = 1;
			if (game->inputNicknameDisplayFlag == 1) {
				return MenuScr;
			}
		}


		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(255, 255, 255), TRUE);
		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(0, 0, 0), FALSE);
		int drawPlayerRankingLeftLineX = game->puzzleX + 10 + GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "ランク外");
		int drawPlayerRankingRightLineX = game->puzzleX + 400 - 10 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "9999秒");
		DrawLine(drawPlayerRankingLeftLineX, game->puzzleY - 100, drawPlayerRankingLeftLineX, game->puzzleY - 20, GetColor(0, 0, 0), 1);
		DrawLine(drawPlayerRankingRightLineX, game->puzzleY - 100, drawPlayerRankingRightLineX, game->puzzleY - 20, GetColor(0, 0, 0), 1);
		int time = (game->finishTime / 1000) - (game->startTime / 1000);
		int rank = 0;
		for (int i = 0; i < 10; i++) {
			if (puzzle->ranking[i].cleartime > time) {
				rank = i + 1;
				if (game->inputNicknameDisplayFlag == -1) {
					game->inputNicknameDisplayFlag = 0;
				}
				SetActiveKeyInput(game->keyHandle);
				break;
			}
		}
		int drawPlayerRankingNumX = drawPlayerRankingLeftLineX - (drawPlayerRankingLeftLineX - game->puzzleX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "%d", rank) / 2;
		int drawPlayerRankingNameX = drawPlayerRankingRightLineX - (drawPlayerRankingRightLineX - drawPlayerRankingLeftLineX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "あなた") / 2;
		int drawPlayerRankingTimeX = drawPlayerRankingRightLineX + (game->puzzleX + 400 - drawPlayerRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "%d秒", time) / 2;
		if (rank > 0) {
			DrawFormatStringToHandle(drawPlayerRankingNumX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "%d", rank);
		}
		else {
			DrawFormatStringToHandle(game->puzzleX + 5, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "ランク外");
		}
		DrawFormatStringToHandle(drawPlayerRankingNameX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "あなた");
		DrawFormatStringToHandle(drawPlayerRankingTimeX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "%d秒", time);

		if (game->inputNicknameDisplayFlag == 0) {
			DrawBox(350, 200, 800, 500, GetColor(200, 200, 200), TRUE);
			DrawBox(350, 200, 800, 500, GetColor(0, 0, 0), FALSE);
			DrawBox(370, 450, 440, 490, GetColor(255, 255, 255), TRUE);
			DrawBox(370, 450, 440, 490, GetColor(0, 0, 0), FALSE);
			DrawFormatString(395, 465, GetColor(0, 0, 0), "OK");

			int str1X = 350 + (800 - 350) / 2 - GetDrawFormatStringWidth("ランキングTOP10入りおめでとうございます!!") / 2;
			int str2X = 350 + (800 - 350) / 2 - GetDrawFormatStringWidth("ランキングに乗せるニックネームを入力してください") / 2;
			DrawFormatString(str1X, 210, GetColor(0, 0, 0), "ランキングTOP10入りおめでとうございます!!");
			DrawFormatString(str2X, 230, GetColor(0, 0, 0), "ランキングに乗せるニックネームを入力してください");

			DrawBox(460, 340, 700, 370, GetColor(0, 0, 0), FALSE);
			
			Button_t tmpButton;
			setButton(370, 450, 440, 490, NULL, NULL, *mouse, &tmpButton);
			if (tmpButton.mState) {
				game->inputNicknameDisplayFlag = 1;
				DeleteKeyInput(game->keyHandle);
			}
			else {
				int black = GetColor(255, 255, 255);
				SetKeyInputStringColor(black, black, black, black, black, black, black);
				DrawKeyInputString(462, 344, game->keyHandle);
			}
		}
	}

	mouse->waitRelease = 1;
	return GameScr;
}


// M19:チュートリアル/ゲームプレイ終了処理
void FinalizeGame(Game_t* game, Puzzle_t* puzzle) {
	InitGraph();
	InitFontToHandle();
}