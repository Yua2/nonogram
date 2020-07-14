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
#include "file.h"

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
	int nextImageHandle = LoadGraph("graph/nextButton.bmp");
	game->pressedPenButtonImageHandle = LoadGraph("graph/penbutton_pressed.bmp");
	game->releasePenButtonImageHandle = LoadGraph("graph/penbutton_release.bmp");
	game->pressedEraserButtonImageHandle = LoadGraph("graph/crossbutton_pressed.bmp");
	game->releaseEraserButtonImageHandle = LoadGraph("graph/crossbutton_release.bmp");
	game->puzzleTitleFontHandle = CreateFontToHandle(NULL, 30, 4);
	game->timeFontHandle = CreateFontToHandle(NULL, 40, 4);
	game->drawNumFontHandle = CreateFontToHandle(NULL, 8, 2);
	game->rankingTitleFontHnadle = CreateFontToHandle(NULL, 34, 3);
	game->rankingFontHandle = CreateFontToHandle(NULL, 50, 3);
	game->playerRankingFontHandle = CreateFontToHandle(NULL, 30, 3);
	game->keyHandle = MakeKeyInput(32, FALSE, FALSE, FALSE);
	game->hintcounter = 0;

	setButton(55, 435, 145, 525, backImageHandle, NULL, mouse, &(game->backButton));

	if (puzzle->puzzleId != 0) {
		setButton(175, 435, 265, 525, hintImageHandle, NULL, mouse, &(game->hintButton));
		setButton(295, 435, 385, 525, resetImageHandle, NULL, mouse, &(game->resetButton));
		setButton(100, 315, 190, 405, game->pressedPenButtonImageHandle, NULL, mouse, &(game->penButton));
		setButton(250, 315, 340, 405, game->releaseEraserButtonImageHandle, NULL, mouse, &(game->eraserButton));
		game->tutorialProcess = -1;
	}
	else {
		setButton(175, 435, 265, 525, game->pressedPenButtonImageHandle, NULL, mouse, &(game->penButton));
		setButton(295, 435, 385, 525, game->releaseEraserButtonImageHandle, NULL, mouse, &(game->eraserButton));
		setButton(345, 370, 390, 405, nextImageHandle, NULL, mouse, &(game->nextButton));
		game->tutorialProcess = 0;
	}

	game->penButton.mState = true;
	game->clearFlag = true;
	game->inputNicknameDisplayFlag = -1;
	game->fromWhiteChange = false;
	game->toWhiteChange = false;

	for (int j = 0, k = game->puzzleY; j < puzzle->y_size; j++, k += game->puzzleGridSize - 1) {
		for (int i = 0, l = game->puzzleX; i < puzzle->x_size; i++, l += game->puzzleGridSize - 1) {
			setButton(l, k, l + game->puzzleGridSize, k + game->puzzleGridSize, NULL, GetColor(255, 255, 255), mouse, &(game->puzzleGrid[i][j]));
			game->puzzleState[i][j] = true;
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
	GetMouseState(mouse, TRUE);
	Mouse_t tmpMouse = *mouse;

	if (mouse->waitRelease == 0) {
		mouse->mButton = none;
	}

	if (game->clearFlag) {
		bool preNextButtonState = game->nextButton.mState;
		static bool process3Flag = false;
		static bool process5Flag = false;
		static bool process6Flag = false;
		static bool process7Flag = false;
		static bool process8Flag = false;
		static bool process9Flag = false;
		static bool process10Flag = false;
		static bool process11Flag = false;
		static bool process12Flag = false;
		

		switch (game->tutorialProcess) {
		case 0 :
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 1;
				}
			}
			mouse->mButton = none;

			DrawFormatString(30, 160, GetColor(0, 0, 0), "ようこそ、");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "ノノグラム・パズルのチュートリアルへ。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "このゲームは、マスの周りにある数字の情報を");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "使って、正しくマスを塗って、パズルを完成さ");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "せていくゲームです。パズルが完成すると、絵");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "が浮かび上がってきます。");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "では、実際にプレイしてみましょう。");

			break;
		case 1 :
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 2;
				}
			}
			mouse->mButton = none;

			DrawFormatString(30, 160, GetColor(0, 0, 0), "まず、マスの周りの数字の見方を説明します。");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "右のパズルの7行目の赤枠に注目してください。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "「1 4 1」と数字がならんでいますね。");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "この並びと数字は、");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "「この行のどこかに1個黒いマスがあって、");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "　その右隣に1個以上白いマスがあって、");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "　また右隣に4個連続した黒いマスがあって、");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "　さらに右隣に1個以上白いマスがあって、");
			DrawFormatString(30, 320, GetColor(0, 0, 0), "　また右隣に1個黒いマスがあって、");
			DrawFormatString(30, 340, GetColor(0, 0, 0), "　その右隣は白いマスがあるかわからない。」");
			DrawFormatString(30, 360, GetColor(0, 0, 0), "という意味を持ちます。");

			break;
		case 2:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 3;
				}
			}
			mouse->mButton = none;

			DrawFormatString(30, 160, GetColor(0, 0, 0), "つまり、7行目はまだ確実に塗ることができな");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "い行ということになります。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "これまで行についてしか説明していませんが、");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "列についても、行を右から左に見たように、");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "上から下に見ていきます。");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "また、右のパズルは10マス×10マスなので、");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "1つの行での黒いマスと白いマスの合計は10にな");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "ります。なので、10個黒いマスが並ぶ行は確実");
			DrawFormatString(30, 320, GetColor(0, 0, 0), "に塗ることができます。");
			DrawFormatString(30, 340, GetColor(0, 0, 0), "よって、青枠で示してあるマスはすべて黒く塗");
			DrawFormatString(30, 360, GetColor(0, 0, 0), "ることができます。");

			break;
		case 3:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process3Flag) {
				process3Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 4;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "では、実際に青枠の部分を塗ってみましょう。");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "まず、鉛筆マークの「塗る」ボタンが有効であ");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "ることを確認してください。ボタンが水色にな");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "っていれば今は「塗るモード」です。");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "次に塗りたいマスを左クリックしてください。");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "また、マスを左クリックして、そのままドラッ");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "グすることで、塗ることもできます。");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "もし、間違って、青枠以外の場所を塗ってしま");
			DrawFormatString(30, 320, GetColor(0, 0, 0), "った場合は、もう一度そのマスを左クリックす");
			DrawFormatString(30, 340, GetColor(0, 0, 0), "ることで白いマスに戻せます。");

			break;
		case 4:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 5;
				}
			}
			mouse->mButton = none;

			DrawFormatString(30, 160, GetColor(0, 0, 0), "次に行ごとに見ていきましょう。");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "1行目は「10」で既に塗られています。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "2行目は「1 1」で、左右の両端が塗られていま");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "す。よって2行目は");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "「1個の黒いマスと");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "　8個の白いマスと1個の黒いマス」");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "とでき、白と黒のマスの合計が10になるので、");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "2行目は完成しています。");
			DrawFormatString(30, 320, GetColor(0, 0, 0), "なので、白いマスは塗ってはいけないので、青");
			DrawFormatString(30, 340, GetColor(0, 0, 0), "枠のマスに×印をつけておきましょう。こうす");
			DrawFormatString(30, 360, GetColor(0, 0, 0), "ることで、あとでパズルが見やすくなります。");

			break;
		case 5 :
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process5Flag) {
				process5Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 6;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "では、×印をつけてみましょう。");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "×マークの「バツをつける」ボタンを左クリッ");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "クして、有効にしてください。×ボタンが水色");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "になれば「×モード」です。");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "その状態で、青枠のマスを左クリック、または");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "ドラッグでマスに×印が付きます。");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "×印はボタンの有効無効にかかわらず、右クリ");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "ックまたはドラッグでつけることができます。");
			DrawFormatString(30, 320, GetColor(0, 0, 0), "例えば、塗るモードのまま、マスを右クリック");
			DrawFormatString(30, 340, GetColor(0, 0, 0), "しても、×をつけられます。");

			break;
		case 6:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process6Flag) {
				process6Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 7;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "3行目は「3 1」で、左右端が黒いマスです。");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "この場合、");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "「左端の黒いマスを含む3個の連続した黒いマ");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "　スと右端の黒いマス1個」");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "としなければ、「3 1」となりません。");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "なので、青枠のマスを塗ることができます。");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "実際に塗るモードで塗ってみましょう");

			break;
		case 7:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process7Flag) {
				process7Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 8;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "4行目と5行目と6行目も先ほどの3行目と同じ");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "考え方で塗ることができます。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "青枠のマスを塗ってみましょう。");

			break;
		case 8:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process8Flag) {
				process8Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 9;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "今赤枠で囲んであるところは完成している部");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "分です。なので、白のマスは確実に黒ではあ");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "りません。");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "後々わかりやすいように赤枠内の白いマスに");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "×印をつけておきましょう。");

			break;
		case 9:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process9Flag) {
				process9Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 10;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "ここで列ごとに見てみましょう。");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "最も右側の9列目、10列目は完成");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "していることがわかります。");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "8列目は、3行目を完成させた考え");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "方で完成させることができます。");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "上から「1 3」とみると、下端の黒");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "いマスを含む3連続の黒いマスがで");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "きるので、青枠のマスを塗りましょ");
			DrawFormatString(30, 320, GetColor(0, 0, 0), "う。");

			break;
		case 10:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process10Flag) {
				process10Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 11;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "7列目も先ほどと似た考え方ですが");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "今の6列目の状態は「1 2 1」なの");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "で、真ん中の2つの黒いマスと下端");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "の黒いマスをどちらも含む6個の連");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "続した黒いマスがないと「1 6」に");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "ならないので、青枠のマスを塗るこ");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "とができます。");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "塗るモードで塗りましょう。");

			break;
		case 11:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process11Flag) {
				process11Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = 12;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "6列目と5列目も7列目と同じ考え方");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "で完成させることができます。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "青枠のマスを黒く塗りましょう。");

			break;
		case 12:
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);
			if (process12Flag) {
				process12Flag = false;
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton));
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);
				mouse->mButton = none;
			}
			if (game->nextButton.mState) {
				if (!preNextButtonState) {
					game->tutorialProcess = -1;
				}
			}

			DrawFormatString(30, 160, GetColor(0, 0, 0), "4列目は「1 4 1」で、今の状態は「1 3 1」");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "です。真ん中の「3」の上に1つ黒を足すか");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "下に1つ黒を足すかで考える場面ですが、");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "上は×印がついているので、下に1つ足すと");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "出来そうです。なので青枠を黒く塗ると…");

			break;
		default :
			break;
		}
	
		bool penState = game->penButton.mState;
		bool eraserState = !penState;

		setButton(55, 435, 145, 525, game->backButton.mImageHandle, NULL, tmpMouse, &(game->backButton));
		setButton(175, 435, 265, 525, game->penButton.mImageHandle, NULL, *mouse, &(game->penButton));
		setButton(295, 435, 385, 525, game->eraserButton.mImageHandle, NULL, *mouse, &(game->eraserButton));

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

		int nowMiliSec = GetNowCount();
		int sec = ((nowMiliSec - (game->startTime)) % 60000) / 1000;
		int minute = (nowMiliSec - (game->startTime)) / 60000;
		DrawGraph(game->backButton.mX1, game->backButton.mY1, game->backButton.mImageHandle, FALSE);
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

				if (game->puzzleGrid[i][j].mState && game->puzzleState[i][j]) {
					switch (mouse->mState) {
					case leftClick:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->penButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 1;
								game->fromWhiteChange = true;
								game->toWhiteChange = false;
							}
							else if (game->eraserButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 0;
								game->fromWhiteChange = true;
								game->toWhiteChange = false;
							}
						}
						else {
							game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
							game->checkPuzzle.puzzleData[i][j] = -1;
							game->fromWhiteChange = false;
							game->toWhiteChange = true;
						}
						break;
					case leftDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->fromWhiteChange) {
								if (game->penButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 1;
								}
								else if (game->eraserButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 0;
								}
							}
						}
						else {
							if (game->toWhiteChange) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
								game->checkPuzzle.puzzleData[i][j] = -1;
							}
						}
						break;
					case rightClick:
						game->checkPuzzle.puzzleData[i][j] = 0;
						break;
					case rightDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							game->checkPuzzle.puzzleData[i][j] = 0;
						}
						break;
					case none:
						game->fromWhiteChange = false;
						game->toWhiteChange = false;
						break;
					}
				}


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
				else {
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
				}
			}
		}

		for (int j = 0; j <= puzzle->y_size; j++) {
			for (int i = 0; i <= puzzle->x_size; i++) {
				DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0));
				DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0));

				if (j % 5 == 0) {
					DrawLine(game->puzzleX + ((game->puzzleGridSize) * (puzzle->x_size)) - (puzzle->x_size), (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0), 3);
				}

				if (i % 5 == 0) {
					DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY + ((game->puzzleGridSize) * (puzzle->y_size)) - (puzzle->y_size), (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0), 3);
				}
			}
		}

		if (game->tutorialProcess == 1) {
			DrawLine(game->puzzleX - 100, game->puzzleY + (game->puzzleGridSize - 1) * 6, game->puzzleX, game->puzzleY + (game->puzzleGridSize - 1) * 6, GetColor(255, 0, 0), 3);
			DrawLine(game->puzzleX, game->puzzleY + (game->puzzleGridSize - 1) * 6, game->puzzleX, game->puzzleY + (game->puzzleGridSize - 1) * 7, GetColor(255, 0, 0), 3);
			DrawLine(game->puzzleX, game->puzzleY + (game->puzzleGridSize - 1) * 7, game->puzzleX - 100, game->puzzleY + (game->puzzleGridSize - 1) * 7, GetColor(255, 0, 0), 3);
			DrawLine(game->puzzleX - 100, game->puzzleY + (game->puzzleGridSize - 1) * 7, game->puzzleX - 100, game->puzzleY + (game->puzzleGridSize - 1) * 6, GetColor(255, 0, 0), 3);
		}

		if (game->tutorialProcess == 2) {
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (i == 0 || j == 0 || i ==puzzle->x_size - 1 || j == puzzle->y_size - 1 ) {
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
						DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
					}
				}
			}
		}

		if (game->tutorialProcess == 3) {
			int process3Counter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (i == 0 || j == 0 || i == puzzle->x_size - 1 || j == puzzle->y_size - 1) {
						if (game->checkPuzzle.puzzleData[i][j] != 1) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process3Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process3Counter--;
						}
					}
				}
			}

			if (process3Counter == 36) {
				process3Flag = true;
			}
			else {
				process3Flag = false;
			}
		}

		if (game->tutorialProcess == 4) {
			for (int i = 1; i < puzzle->x_size - 1; i++) {
				DrawLine(game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY1, game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY1, GetColor(0, 0, 255), 3);
				DrawLine(game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY1, game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY2, GetColor(0, 0, 255), 3);
				DrawLine(game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY2, game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY2, GetColor(0, 0, 255), 3);
				DrawLine(game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY2, game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY1, GetColor(0, 0, 255), 3);
			}
		}

		if (game->tutorialProcess == 5) {
			int process5Counter = 0;
			for (int j = 1; j < puzzle->y_size - 1; j++) {
				for (int i = 1; i < puzzle->x_size - 1; i++) {
					if (j == 1) {
						if (game->checkPuzzle.puzzleData[i][j] != 0) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process5Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process5Counter--;
						}
					}
				}
			}

			if (process5Counter == 8) {
				process5Flag = true;
			}
			else {
				process5Flag = false;
			}
		}

		if (game->tutorialProcess == 6) {
			int process6Counter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (j == 2 && (i > 0 && i < 3)) {
						if (game->checkPuzzle.puzzleData[i][j] != 1) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process6Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process6Counter--;
						}
					}
				}
			}

			if (process6Counter + 36 + 8 == 2) {
				process6Flag = true;
			}
			else {
				process6Flag = false;
			}
		}

		if (game->tutorialProcess == 7) {
			int process7Counter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (((j == 3) && (i > 0 && i < 6)) || ((3 < j && j < 6) && (0 < i && i < 7))) {
						if (game->checkPuzzle.puzzleData[i][j] != 1) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process7Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process7Counter--;
						}
					}
				}
			}

			if (process7Counter + 36 + 8 + 2 == 17) {
				process7Flag = true;
			}
			else {
				process7Flag = false;
			}
		}

		if (game->tutorialProcess == 8) {
			int process8Counter = 0;
			DrawLine(game->puzzleX, game->puzzleY, game->puzzleX + ((game->puzzleGridSize - 1) * puzzle->x_size), game->puzzleY, GetColor(255, 0, 0), 3);
			DrawLine(game->puzzleX + ((game->puzzleGridSize - 1) * puzzle->x_size), game->puzzleY, game->puzzleX + ((game->puzzleGridSize - 1) * puzzle->x_size), game->puzzleY + ((game->puzzleGridSize - 1) * 6), GetColor(255, 0, 0), 3);
			DrawLine(game->puzzleX + ((game->puzzleGridSize - 1) * puzzle->x_size), game->puzzleY + ((game->puzzleGridSize - 1) * 6), game->puzzleX, game->puzzleY + ((game->puzzleGridSize - 1) * 6), GetColor(255, 0, 0), 3);
			DrawLine(game->puzzleX, game->puzzleY + ((game->puzzleGridSize - 1) * 6), game->puzzleX, game->puzzleY, GetColor(255, 0, 0), 3);
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (j < 6) {
						if (game->checkPuzzle.puzzleData[i][j] == 0 && puzzle->puzzleData[i][j] < 100) {
							process8Counter++;
						}
						else {
							process8Counter--;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process8Counter--;
						}
					}
				}
			}

			if (process8Counter + 36 + 2 + 17 == 21) {
				process8Flag = true;
			}
			else {
				process8Flag = false;
			}
		}

		if (game->tutorialProcess == 9) {
			int process9Counter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if ((i == 7) && (6 < j && j < 9)) {
						if (game->checkPuzzle.puzzleData[i][j] != 1) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process9Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process9Counter--;
						}
					}
				}
			}

			if (process9Counter + 36 + 2 + 17 + 21 == 2) {
				process9Flag = true;
			}
			else {
				process9Flag = false;
			}
		}

		if (game->tutorialProcess == 10) {
			int process10Counter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if ((i == 6) && (5 < j && j < 9)) {
						if (game->checkPuzzle.puzzleData[i][j] != 1) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process10Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process10Counter--;
						}
					}
				}
			}

			if (process10Counter + 36 + 2 + 17 + 21 + 2 == 3) {
				process10Flag = true;
			}
			else {
				process10Flag = false;
			}
		}

		if (game->tutorialProcess == 11) {
			int process11Counter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (((3 < i && i < 6) && (5 < j && j < 9))) {
						if (game->checkPuzzle.puzzleData[i][j] != 1) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process11Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process11Counter--;
						}
					}
				}
			}

			if (process11Counter + 36 + 2 + 17 + 21 + 2 + 3 == 6) {
				process11Flag = true;
			}
			else {
				process11Flag = false;
			}
		}

		if (game->tutorialProcess == 12) {
			int process12Counter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (i == 3 && j == 6) {
						if (game->checkPuzzle.puzzleData[i][j] != 1) {
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
							DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						}
						else {
							process12Counter++;
						}
					}
					else {
						if (game->checkPuzzle.puzzleData[i][j] != -1) {
							process12Counter--;
						}
					}
				}
			}

			if (process12Counter + 36 + 2 + 17 + 21 + 2 + 3 + 6 == 1) {
				process12Flag = true;
			}
			else {
				process12Flag = false;
			}
		}

		for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, x = ((game->puzzleX) - 10); j < 10; j++, x -= 10) {
				if ((game->drawGrid_V[i][j]) != 0) {
					int drawY = y + (game->puzzleGridSize / 2) - 4;
					DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%2d", (game->drawGrid_V[i][j]));
				}
			}
		}

		for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, y = ((game->puzzleY) - 10); j < 10; j++, y -= 10) {
				if ((game->drawGrid_H[i][j]) != 0) {
					int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "%2d", game->drawGrid_H[i][j]) / 2);
					DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%2d", (game->drawGrid_H[i][j]));
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

		if (diff == 0 && game->tutorialProcess) {
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

		DrawGraph(215, 610, game->backButton.mImageHandle, FALSE);
		setButton(215, 610, 305, 700, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton));

		if (game->backButton.mState) {
			mouse->waitRelease = 1;
			return MenuScr;
		}

		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(255, 255, 255), TRUE);
		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(0, 0, 0), FALSE);
		int drawPlayerRankingRightLineX = game->puzzleX + 400 - 10 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "9999秒");
		DrawLine(drawPlayerRankingRightLineX, game->puzzleY - 100, drawPlayerRankingRightLineX, game->puzzleY - 20, GetColor(0, 0, 0), 1);
		int time = (game->finishTime / 1000) - (game->startTime / 1000);
		int drawPlayerRankingNameX = drawPlayerRankingRightLineX - (drawPlayerRankingRightLineX - game->puzzleX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "あなた") / 2;
		int drawPlayerRankingTimeX = drawPlayerRankingRightLineX + (game->puzzleX + 400 - drawPlayerRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "%d秒", time) / 2;
		DrawFormatStringToHandle(drawPlayerRankingNameX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "あなた");
		DrawFormatStringToHandle(drawPlayerRankingTimeX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "%d秒", time);

		DrawString(50, 340, "クリアおめでとうございます。", GetColor(0, 0, 0));
		DrawString(50, 380, "他のパズルや、自分で作ったパズルも遊んでみてください。", GetColor(0, 0, 0));
	}

	mouse->waitRelease = 1;
	return GameScr;
}

// M18:ゲームプレイ更新
int UpdateGame(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse, int* key) {
	GetMouseState(mouse, TRUE);
	
	if (mouse->waitRelease == 0) {
		mouse->mButton = none;
	}

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
		
		// ヒントボタンが押された場合
		if (game->hintButton.mState) {
			// ループで回ってきたとき，ヒントボタンを前のループでクリックしていなかった場合
			if (!hintState) {
				game->hintcounter++;	// ヒントボタンを押した回数(0より大きいとランキングに反映されない)
				int rand = GetRand(1);	// 乱数でヒントを列で出すか行で出すかの決定(0:行, 1:列)
				// 行でヒントを出す場合
				if (rand == 0) {
					int line = 0;	// 乱数で何行目をヒントとして表示するかの決定
					bool breakFlag = true;	// whileループのブレークフラグ
					// 正しくないマスが1つでもあるような行が見つかるまでlineを更新
					while (breakFlag) {
						line = GetRand(puzzle->y_size - 1);	// lineの更新

						// ヒントとして表示する行がすべて正しく塗られているか判定し，間違っているマスが見つかればwhileループのブレークフラグをfalseにする
						for (int i = 0; i < puzzle->x_size; i++) {
							// ユーザによって塗られていないマス(空白マスまたは×マス)が間違っている場合(もし遊んでいるパズルのパズルデータのline行目のi列が100より大きく(黒く塗るマス)，かつ，正誤判定用のパズルデータのline行目のi列が黒く塗られていない)
							if (puzzle->puzzleData[i][line] > 100 && game->checkPuzzle.puzzleData[i][line] != 1) {
								breakFlag = false;	// whileループを抜ける
								break;	// ループを抜ける
							}
							// ユーザによって塗られているマスが間違っている場合(もし遊んでいるパズルのパズルデータのline行目のi列100がより小さく(塗らないマス)，かつ，正誤判定用のパズルデータのline行目のi列が黒く塗られている)
							else if (puzzle->puzzleData[i][line] <= 100 && game->checkPuzzle.puzzleData[i][line] == 1) {
								breakFlag = false;	// whileループを抜ける
								break;	// ループを抜ける
							}
						}
					}

					// ヒントとして表示する行の正解データをセットし，表示するマスを不変のマスにセットする
					for (int i = 0; i < puzzle->x_size; i++) {
						// もし遊んでいるパズルのパズルデータのline行目のi列が100より大きい(黒く塗るマス)の場合
						if (puzzle->puzzleData[i][line] > 100) {
							game->checkPuzzle.puzzleData[i][line] = 1;	// 正誤判定用パズルのパズルデータのline行目のiを黒マス(1)にセット
						}
						// 遊んでいるパズルのパズルデータのline行目のi列が100以下(塗らなくていいますマス)の場合
						else {
							game->checkPuzzle.puzzleData[i][line] = 0;	// 正誤判定用パズルのパズルデータのline行目のiを×表示のマス(0)にセット
						}
						game->puzzleState[i][line] = false;	// line行目のi列を不変マスにセット
					}
				}
				// 列でヒントを出す場合
				else {	// rand == 1
					int cols = 0;	// 乱数で何列目をヒントとして表示するかの決定
					bool breakFlag = true;	// whileループのブレークフラグ
					// 正しくないマスが1つでもあるような列が見つかるまでcolsを更新
					while (breakFlag) {
						cols = GetRand(puzzle->x_size - 1);	// colsの更新

						// ヒントとして表示する列がすべて正しく塗られているか判定し，間違っているマスが見つかればwhileループのブレークフラグをfalseにする
						for (int j = 0; j < puzzle->y_size; j++) {
							// ユーザによって塗られていないマス(空白マスまたは×マス)が間違っている場合(もし遊んでいるパズルのパズルデータのcols列目のj行が100より大きく(黒く塗るマス)，かつ，正誤判定用のパズルデータのcols列目のj行が黒く塗られていない)
							if (puzzle->puzzleData[cols][j] > 100 && game->checkPuzzle.puzzleData[cols][j] != 1) {
								breakFlag = false;	// whileループを抜ける
								break;	// ループを抜ける
							}
							// ユーザによって塗られているマスが間違っている場合(もし遊んでいるパズルのパズルデータのcols列目のj行が100より小さく(塗らないマス)，かつ，正誤判定用のパズルデータのcols列目のj行が黒く塗られている)
							else if (puzzle->puzzleData[cols][j] <= 100 && game->checkPuzzle.puzzleData[cols][j] == 1) {
								breakFlag = false;	// whileループを抜ける
								break;	// ループを抜ける
							}
						}
					}
									
					// ヒントとして表示する列の正解データをセットし，表示するマスを不変のマスにセットする
					for (int j = 0; j < puzzle->y_size; j++) {
						// もし遊んでいるパズルのパズルデータのcols行目のi行が100より大きい(黒く塗るマス)の場合
						if (puzzle->puzzleData[cols][j] > 100) {
							game->checkPuzzle.puzzleData[cols][j] = 1;	// 正誤判定用パズルのパズルデータのcols列目のj行を黒マス(1)にセット
						}
						// 遊んでいるパズルのパズルデータのline行目のi列が100以下(塗らなくていいますマス)の場合
						else {
							game->checkPuzzle.puzzleData[cols][j] = 0;	// 正誤判定用パズルのパズルデータのline行目のj行を×表示のマス(0)にセット
						}

						game->puzzleState[cols][j] = false;	// cols列目のj行を不変マスにセット
					}
				}
			}
		}


		if (game->resetButton.mState) {
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (game->puzzleState[i][j]) {
						game->checkPuzzle.puzzleData[i][j] = -1;
						game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
					}
				}
			}
		}

		int nowMiliSec = GetNowCount();
		int sec = ((nowMiliSec - (game->startTime)) % 60000) / 1000;
		int minute = (nowMiliSec - (game->startTime)) / 60000;
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

				if (game->puzzleGrid[i][j].mState && game->puzzleState[i][j]) {
					switch (mouse->mState) {
					case leftClick:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->penButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 1;
								game->fromWhiteChange = true;
								game->toWhiteChange = false;
							}
							else if (game->eraserButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 0;
								game->fromWhiteChange = true;
								game->toWhiteChange = false;
							}
						}
						else {
							game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
							game->checkPuzzle.puzzleData[i][j] = -1;
							game->fromWhiteChange = false;
							game->toWhiteChange = true;
						}
						break;
					case leftDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->fromWhiteChange) {
								if (game->penButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 1;
								}
								else if (game->eraserButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 0;
								}
							}
						}
						else {
							if (game->toWhiteChange) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
								game->checkPuzzle.puzzleData[i][j] = -1;
							}
						}
						break;
					case rightClick:
						game->checkPuzzle.puzzleData[i][j] = 0;
						break;
					case rightDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							game->checkPuzzle.puzzleData[i][j] = 0;
						}
						break;
					case none:
						game->fromWhiteChange = false;
						game->toWhiteChange = false;
						break;
					}
				}


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
				else {
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);
					DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);
				}
			}
		}

		for (int j = 0; j <= puzzle->y_size; j++) {
			for (int i = 0; i <= puzzle->x_size; i++) {
				DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0));
				DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0));
				
				if (j % 5 == 0) {
					DrawLine(game->puzzleX + ((game->puzzleGridSize) * (puzzle->x_size)) - (puzzle->x_size), (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0), 3);
				}

				if (i % 5 == 0) {
					DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY + ((game->puzzleGridSize) * (puzzle->y_size)) - (puzzle->y_size), (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0), 3);
				}
			}
		}

		for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, x = ((game->puzzleX) - 10); j < 10; j++, x -= 10) {
				if ((game->drawGrid_V[i][j]) != 0) {
					int drawY = y + (game->puzzleGridSize / 2) - 4;
					DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%2d", (game->drawGrid_V[i][j]));
				}
			}
		}

		for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
			for (int j = 0, y = ((game->puzzleY) - 10); j < 10; j++, y -= 10) {
				if ((game->drawGrid_H[i][j]) != 0) {
					int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "%2d", game->drawGrid_H[i][j]) / 2);
					DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%2d", (game->drawGrid_H[i][j]));
				}
			}
		}

		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				if (game->puzzleState[i][j] == false) {
					DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(255, 0, 0), 3);
					DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(255, 0, 0), 3);
					DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(255, 0, 0), 3);
					DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(255, 0, 0), 3);
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
			int drawRankingNameX = drawRankingRightLineX - (drawRankingRightLineX - drawRankingLeftLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, puzzle->ranking[i].playerId) / 2;
			DrawFormatStringToHandle(drawRankingNameX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, puzzle->ranking[i].playerId);
			int drawRankingTimeX = drawRankingRightLineX + 2 + (500 - drawRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime) / 2;
			DrawFormatStringToHandle(drawRankingTimeX, 52 + (54 * i), GetColor(0, 0, 0), game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime);
		}

		DrawGraph(215, 610, game->backButton.mImageHandle, FALSE);
		setButton(215, 610, 305, 700, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton));

		if (game->backButton.mState) {
			mouse->waitRelease = 1;
			return MenuScr;

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
					SetActiveKeyInput(game->keyHandle);
				}
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

		if (game->inputNicknameDisplayFlag == 0 && game->hintcounter == 0) {
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
				char player[256];
				GetKeyInputString(player, game->keyHandle);
				updateRanking(puzzle, player, (game->finishTime - game->startTime) / 1000);
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