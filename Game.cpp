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
#include <string.h>
#include <string>

using namespace std;

/*****************************************************
*** Function Name : InitializeGame (M12) 
*** Designer      : 藤川
*** Date          : 2020.7.19
*** Function      : 引数でとったチュートリアル/ゲームプレイ画面の
					構造体(Game_t)のすべてのメンバを初期化する
*** Return        : 次の遷移先のシーン番号(GameScr)
******************************************************/
int InitializeGame(Game_t* game, Puzzle_t* puzzle) {
	Mouse_t mouse;	// ボタン設定用の一時的なマウス変数
	GetMouseState(&mouse);	// マウス変数の状態を更新(M4)

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
	// 縦横どちらか一辺が15の時
	else if (puzzle->x_size == 15 || puzzle->y_size == 15) {
		game->puzzleGridSize = 35;	// パズルのマスのサイズ(15×15)
	}
	// 両辺とも10の時
	else {
		game->puzzleGridSize = 54;	// パズルのマスのサイズ(10×10)
	}

	// 塗る場所を指示する上辺と左辺の数列を2重for文で0に初期化
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 20; i++) {
			game->drawGrid_H[i][j] = 0;	// 上辺の数列を0に初期化
			game->drawGrid_V[i][j] = 0;	// 左辺の数列を0に初期化
		}
	}

	// 塗る場所を指示する左辺数列を2重for文でセット
	for (int j = 0; j < puzzle->y_size; j++) {
		int vIndex = 0;	// 左辺数列の塗る数の塊のインデックス
		for (int i = 0; i < puzzle->x_size; i++) {
			// パズルデータのi行j列の値が100より大きい(濃い色)とき
			if (puzzle->puzzleData[i][j] > 100) {
				game->drawGrid_V[j][vIndex]++;	//　左辺数列の塗る数(表示する数字)をインクリメント
			}
			// パズルデータのi行j列の値が100より小さい(薄い色)とき
			else if (game->drawGrid_V[j][vIndex] > 0) {
				vIndex++;	// 塊のひとつずらす
			}
		}
	}

	// 塗る場所を指示する上辺数列を2重for文でセット
	for (int j = 0; j < puzzle->x_size; j++) {
		int hIndex = 0;	// 上辺数列の塗る数の塊のインデックス
		for (int i = 0; i < puzzle->y_size; i++) {
			// パズルデータのi行j列の値が100より大きい(濃い色)とき
			if (puzzle->puzzleData[j][i] > 100) {
				game->drawGrid_H[j][hIndex]++;	// 上辺数列の塗る数(表示する数字)をインクリメント
			}
			// パズルデータのi行j列の値が100より小さい(薄い色)とき
			else if (game->drawGrid_H[j][hIndex] > 0) {
				hIndex++;	// 塊をひとつずらす
			}
		}
	}

	// 塗る場所を示す数を2重for文で逆順の右詰めに直す
	for (int i = 0; i < puzzle->y_size; i++) {
		int tmp_V[10];	// 左辺数列を逆順に数列を格納する配列
		int tmp_H[10];	// 上辺数列を逆順に数列を格納する配列
		// i行の数列を逆順に格納する
		for (int j = 0; j < 10; j++) {
			tmp_V[j] = game->drawGrid_V[i][9 - j];	// 左辺行列を逆順に格納
			tmp_H[j] = game->drawGrid_H[i][9 - j];	// 上辺行列を逆順に格納
		}

		int count_V = 0;	// 左辺数列の塊のインデックス番号
		int count_H = 0;	// 上辺数列の塊のインデックス番号
		// 各数列のi行の逆順数列を右図目に直す
		for (int j = 0; j < 10; j++) {
			// 左辺数列の逆順数列の塊がある(0より大きい値が入っている)とき
			if (tmp_V[j] > 0) {
				game->drawGrid_V[i][count_V] = tmp_V[j];	// 右詰めする
				count_V++;	// 塊のインデックス番号をひとつずらす
			}
			// 左辺数列の逆順数列の塊がない(0より大きい値が入っていない)とき
			else {
				game->drawGrid_V[i][9 - j] = tmp_V[j];	// 左詰めする
			}

			// 上辺数列の逆順数列の塊がある(0より大きい値が入っている)とき
			if (tmp_H[j] > 0) {
				game->drawGrid_H[i][count_H] = tmp_H[j];	// 右詰めする
				count_H++;
			}
			// 上辺数列の逆順数列の塊がない(0より大きい値が入っていない)とき
			else {
				game->drawGrid_H[i][9 - j] = tmp_H[j];	// 左詰めする
			}
		}
	}

	game->checkPuzzle = *puzzle;	// 正誤判定用のパズルに正解パズルのランキングデータなどを格納

	// 2重for文で正誤判定用パズルをすべて何も塗られていないマス(-1)に初期化
	for (int j = 0; j < 20; j++) {
		for (int i = 0; i < 20; i++) {
			game->checkPuzzle.puzzleData[i][j] = -1;	// 白いマス(-1)に初期化
		}
	}

	int backImageHandle = LoadGraph("graph/backButton.bmp");	// 「戻る」ボタンの画像をハンドルに格納
	int hintImageHandle = LoadGraph("graph/hintButton.bmp");	// 「ヒント」ボタンの画像をハンドルに格納
	int resetImageHandle = LoadGraph("graph/resetButton.bmp");	// 「リセット」ボタンの画像をハンドルに格納
	int nextImageHandle = LoadGraph("graph/nextButton.bmp");	// チュートリアルの「次へ」ボタンの画像をハンドルに格納
	int returnImageHandle = LoadGraph("graph/returnButton.bmp");	// チュートリアルの「ひとつ前に戻る」ボタンの画像をハンドルに格納
	game->pressedPenButtonImageHandle = LoadGraph("graph/penbutton_pressed.bmp");	// 有効状態の「塗る」ボタンの画像をハンドルに格納
	game->releasePenButtonImageHandle = LoadGraph("graph/penbutton_release.bmp");	// 無効状態の「塗る」ボタンの画像をハンドルに格納
	game->pressedEraserButtonImageHandle = LoadGraph("graph/crossbutton_pressed.bmp");	// 有効状態の「バツをつける」ボタンの画像をハンドルに格納
	game->releaseEraserButtonImageHandle = LoadGraph("graph/crossbutton_release.bmp");	// 無効状態の「バツをつける」ボタンの画像をハンドルに格納
	game->puzzleTitleFontHandle = CreateFontToHandle(NULL, 30, 4);	// パズルタイトルを表示する際のフォントハンドルを格納
	game->timeFontHandle = CreateFontToHandle(NULL, 40, 4);	// プレイ時間を表示する際のフォントハンドルを格納
	game->drawNumFontHandle = CreateFontToHandle(NULL, 8, 2);	// 塗る場所を支持する数列を表示する際のフォントハンドルを格納
	game->rankingTitleFontHnadle = CreateFontToHandle(NULL, 34, 3);	// ランキングタイトルを表示する際のフォントハンドルを格納
	game->rankingFontHandle = CreateFontToHandle(NULL, 35, 3);	// ランキングを表示する際のフォントハンドルをフォントハンドルを格納
	game->playerRankingFontHandle = CreateFontToHandle(NULL, 30, 3);	// パズルをプレイしたプレイヤーのタイムやランキングを表示する際のフォントハンドルを格納
	game->keyHandle = MakeKeyInput(16, FALSE, FALSE, FALSE);	// キー入力用ハンドルを格納
	game->hintcounter = 0;	// ヒント利用回数を0回に初期化

	setButton(55, 435, 145, 525, backImageHandle, NULL, mouse, &(game->backButton), TRUE);	// 「戻る」ボタンの座標と画像と状態を設定
	setButton(100, 315, 190, 405, game->pressedPenButtonImageHandle, NULL, mouse, &(game->penButton), TRUE);	// 「塗る」ボタンの座標と画像と状態を設定
	setButton(250, 315, 340, 405, game->releaseEraserButtonImageHandle, NULL, mouse, &(game->eraserButton), TRUE);	// 「バツをつける」ボタンの座標と画像と状態を設定

	// 遊ぶパズルがチュートリアルパズルでない(パズルIDが0でない)とき
	if (puzzle->puzzleId != 0) {
		setButton(175, 435, 265, 525, hintImageHandle, NULL, mouse, &(game->hintButton), TRUE);	// 「ヒント」ボタンの座標と画像と状態を設定
		setButton(295, 435, 385, 525, resetImageHandle, NULL, mouse, &(game->resetButton), TRUE);	// 「リセット」ボタンの座標と画像と状態を設定
		game->tutorialProcess = -1;	// チュートリアル説明番号を説明終了に設定(チュートリアルをプレイしないので説明しなくていい)
	}
	// 遊ぶパズルがチュートリアルパズル(パズルIDが0)のとき
	else {
		setButton(345, 370, 390, 405, nextImageHandle, NULL, mouse, &(game->nextButton), TRUE);	// チュートリアルの「ひとつ前に戻る」ボタンの座標と画像と状態を設定
		setButton(30, 375, 76, 405, returnImageHandle, NULL, mouse, &(game->returnButton), TRUE);	// チュートリアルの「次へ」ボタンの座標と画像と状態を設定
		game->tutorialProcess = 0;	// チュートリアル説明番号を説明開始(0)に設定

		// 3重for文でチュートリアルの正解パズルのデータをセット(p:説明番号に対応した正解パズルデータ，j:パズルの行番号，i:パズルの列番号)
		for (int p = 0; p < 13; p++) {
			for (int j = 0; j < 10; j++) {
				for (int i = 0; i < 10; i++) {
					// 説明番号によりセットするパズルを分岐
					switch (p) {
					case 0 :	// 説明番号0に対応する正解データを格納
						game->processAnswerData[p][i][j] = -1;	// 正解パズルをすべて白いマス(-1)にする
						break;

					case 1 :	// 説明番号1に対応する正解データを格納
						game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルをコピー
						break;

					case 2 :	// 説明番号2に対応する正解データを格納
						game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルをコピー
						break;

					case 3 :	// 説明番号1に対応する正解データを格納
						// 上下左右辺のとき
						if (i == 0 || i == 9 || j == 0 || j == 9) {
							game->processAnswerData[p][i][j] = 1;	// 正解パズルのi行j列を黒いマス(1)にする
						}
						// 上下左右辺でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;

					case 4 :	// 説明番号3に対応する正解データを格納
						game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルをコピー
						break;

					case 5 :	// 説明番号4に対応する正解データを格納
						// 2行目の2~9列目のとき
						if ((1 <= i && i <= 8) && j == 1) {
							game->processAnswerData[p][i][j] = 0;	// 正解データのi行j列を×印付きマス(0)にする
						}
						// 1行目の2~4列目でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルをコピー
						}
						break;

					case 6 :	// 説明番号6に対応する正解データを格納
						// 3行目の2~4列目のとき
						if ((1 <= i && i <= 2) && j == 2) {
							game->processAnswerData[p][i][j] = 1;	// 正解パズルのi行j列を黒いマス(1)にする
						}
						// 3行目の2~4列目でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;

					case 7 :	// 説明番号7に対応する正解データを格納
						// 4行目の2~6列目，または，5，6行目の2~7列目のとき
						if ((1 <= i && i <= 5) && j == 3 || (1 <= i && i <= 6) && (4 <= j && j <= 5)) {
							game->processAnswerData[p][i][j] = 1;	// 正解パズルのi行j列を黒いマス(1)にする
						}
						// 4行目の2~6列目，または，5，6行目の2~7列目でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;

					case 8 :	// 説明番号8に対応する正解データを格納
						// 6行目のよりも上で，ひとつ前の正解パズルデータが白いマス(-1)のとき
						if (j <= 5 && game->processAnswerData[p - 1][i][j] == -1) {
							game->processAnswerData[p][i][j] = 0;	// 正解データのi行j列を×印付きマス(0)にする
						}
						// 6行目のよりも下で，ひとつ前の正解パズルデータが白いマス(-1)でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;

					case 9 :	// 説明番号9に対応する正解データを格納
						// 8列目の8，9行目のとき
						if (i == 7 && (7 <= j && j <= 8)) {
							game->processAnswerData[p][i][j] = 1;	// 正解パズルのi行j列を黒いマス(1)にする
						}
						// 8列目の8，9行目でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;

					case 10 :	// 説明番号10に対応する正解データを格納
						// 7列目の7~9行目のとき
						if (i == 6 && (6 <= j && j <= 8)) {
							game->processAnswerData[p][i][j] = 1;	// 正解パズルのi行j列を黒いマス(1)にする
						}
						// 7列目の7~9行目でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;

					case 11 :	// 説明番号11に対応する正解データを格納
						// 4，5列目の7~9行目のとき
						if ((4 <= i && i <= 5) && (6 <= j && j <= 8)) {
							game->processAnswerData[p][i][j] = 1;	// 正解パズルのi行j列を黒いマス(1)にする
						}
						// 4，5列目の7~9行目でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;

					case 12 :	// 説明番号12に対応する正解データを格納
						// 6行3列のとき
						if (i == 3 && j == 6) {
							game->processAnswerData[p][i][j] = 1;	// 正解パズルのi行j列を黒いマス(1)にする
						}
						// 6行3列でないとき
						else {
							game->processAnswerData[p][i][j] = game->processAnswerData[p - 1][i][j];	// 前の正解パズルのi行j列をコピー
						}
						break;
					}
				}
			}
		}
	}

	game->penButton.mState = true;	// 「塗る」ボタンを有効化
	game->clearFlag = true;	// クリアフラグを立てる(パズルをクリアしていない)
	game->inputNicknameDisplayFlag = -1;	// ランキング入力ウィンドウを未入力非表示に設定
	game->fromWhiteChangeLeft = false;	// 左クリックによる白いマスからの変化フラグをおろす
	game->toWhiteChangeLeft = false;	// 左クリックによる白いマスへの変化フラグをおろす
	game->fromWhiteChangeRight = false;	// 右クリックによる白いマスからの変化フラグをおろす
	game->toWhiteChangeRight = false;	// 右クリックによる白いマスへの変化フラグをおろす

	// 2重for文で表示パズルの各マス(ボタン)の座標，色，状態をセット(j:表示時の列，i:表示時の行，k:セットする座標の右端，l:セットする座標の上端)
	for (int j = 0, k = game->puzzleY; j < puzzle->y_size; j++, k += game->puzzleGridSize - 1) {
		for (int i = 0, l = game->puzzleX; i < puzzle->x_size; i++, l += game->puzzleGridSize - 1) {
			setButton(l, k, l + game->puzzleGridSize, k + game->puzzleGridSize, NULL, GetColor(255, 255, 255), mouse, &(game->puzzleGrid[i][j]), TRUE);	// 各マスボタンに状態セット
			game->puzzleState[i][j] = true;	// すべてのマスを可変に設定
		}
	}

	DrawGraph(game->backButton.mX1, game->backButton.mY1, game->backButton.mImageHandle, FALSE);	// 戻るボタン表示
	DrawGraph(game->hintButton.mX1, game->hintButton.mY1, game->hintButton.mImageHandle, FALSE);	// ヒントボタン表示
	DrawGraph(game->resetButton.mX1, game->resetButton.mY1, game->resetButton.mImageHandle, FALSE);	// リセットボタン表示
	DrawGraph(game->penButton.mX1, game->penButton.mY1, game->penButton.mImageHandle, FALSE);	// 塗るボタン表示
	DrawGraph(game->eraserButton.mX1, game->eraserButton.mY1, game->eraserButton.mImageHandle, FALSE);	// バツをつけるボタン表示
	DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->timeFontHandle, "Time : 00:00");	// プレイ時間を0秒表示
	int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);	// パズルタイトルを表示する文字列のx中心座標
	DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);	// パズルタイトル表示

	// 2重for文でパズルのマス表示
	for (int j = 0; j < puzzle->y_size; j++) {
		for (int i = 0; i < puzzle->x_size; i++) {
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mColor, TRUE);	// パズルマスの色でマスを塗りつぶす
			DrawBox(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 0), FALSE);	// パズルマスを黒い枠で囲む
		}
	}


	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX) - 100, game->puzzleY, GetColor(0, 0, 0));	// 塗る場所を指示する左辺数列を表示するための上端の横線を描画
	DrawLine(game->puzzleX, game->puzzleY, (game->puzzleX), game->puzzleY - 100, GetColor(0, 0, 0));	// 塗る場所を指示する上辺数列を表示するための左端の縦線を描画
	
	// 2重for文でパズルの塗る場所を指示する線を描画
	for (int j = 1; j <= puzzle->y_size; j++) {
		for (int i = 1; i <= puzzle->x_size; i++) {
			DrawLine(game->puzzleX, (game->puzzleY) + (game->puzzleGridSize - 1) * j, (game->puzzleX) - 100, (game->puzzleY) + (game->puzzleGridSize - 1) * j, GetColor(0, 0, 0));	// 塗る場所を指示する左辺数列を表示するための横線を描画
			DrawLine(game->puzzleX + (game->puzzleGridSize - 1) * i, game->puzzleY, (game->puzzleX) + (game->puzzleGridSize - 1) * i, (game->puzzleY) - 100, GetColor(0, 0, 0));	// 塗る場所を指示する上辺数列を表示するための縦線を描画
		}
	}

	// 2重for文で左辺数列を描画(i:行番号，j:列番号，y:文字描画のy座標，x:文字描画のx座標)
	for (int i = 0, y = ((game->puzzleY)); i < (puzzle->y_size); i++, y += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, x = ((game->puzzleX) - 10); j < 10; j++, x -= 10) {
			// 左辺数列のi行のj番目の値が0でないのとき
			if ((game->drawGrid_V[i][j]) != 0) {
				int drawY = y + (game->puzzleGridSize / 2) - 4;	// 表示する文字が各行の真ん中になるようにy座標を計算
				DrawFormatStringToHandle(x, drawY, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_V[i][j]));	// 左辺数列の数字を表示
			}
		}
	}

	// 2重for文で上辺数列を描画(i:行番号，j:列番号，y:文字描画のy座標，x:文字描画のx座標)
	for (int i = 0, x = ((game->puzzleX)); i < (puzzle->x_size); i++, x += ((game->puzzleGridSize) - 1)) {
		for (int j = 0, y = ((game->puzzleY) - 10); j < 10; j++, y -= 10) {
			// 上辺数列のi行のj番目の値が0でないのとき
			if ((game->drawGrid_H[i][j]) != 0) {
				int drawX = x + (game->puzzleGridSize / 2) - (GetDrawFormatStringWidthToHandle(game->drawNumFontHandle, "%d", game->drawGrid_H[i][j]) / 2);	// 表示する文字が各行の真ん中になるようにx座標を計算
				DrawFormatStringToHandle(drawX, y, GetColor(0, 0, 0), game->drawNumFontHandle, "%d", (game->drawGrid_H[i][j]));	// 上辺数列の数字を表示
			}
		}
	}

	game->startTime = GetNowCount();	// パズルプレイスタートの現在の時刻(ミリ秒)を格納

	return GameScr;	// 遷移先のシーンのシーン番号をGameScrとして返す
}

/*********************************************************
*** Function Name : UpdateTutorial (M13)
*** Designer      : 藤川
*** Date          : 2020.7.19
*** Function      : 引数でとったチュートリアル画面の構造体(Game_t)の
					の画面の状態を更新
*** Return        : 次の遷移先のシーン番号(GameScr)
**********************************************************/
int UpdateTutorial(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse, int* key) {
	GetMouseState(mouse, TRUE);	// 引数でとったマウ変数の状態を更新(M4)
	Mouse_t tmpMouse = *mouse;	// 戻るボタンを押したか判定するためのマウス変数

	// クリアフラグが立っていたら(クリアしていない)
	if (game->clearFlag) {
		bool preNextButtonState = game->nextButton.mState;	// 次へボタンの状態更新前の状態を格納
		bool preRetrunButtonState = game->returnButton.mState;	// ひとつ前に戻るボタンの状態更新前の状態を格納
		static bool processFlag[13];	// チュートリアルの説明の説明番号ごとの次へボタンを表示するかどうかのフラグ

		// 説明番号により表示する説明文を分岐
		switch (game->tutorialProcess) {
		case 0 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
			
			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 1;	// 説明番号を1にする
				}
			}

			mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)

			// 説明文表示
			DrawFormatString(30, 160, GetColor(0, 0, 0), "ようこそ、");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "ノノグラム・パズルのチュートリアルへ。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "このゲームは、マスの周りにある数字の情報を");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "使って、正しくマスを塗って、パズルを完成さ");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "せていくゲームです。パズルが完成すると、絵");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "が浮かび上がってきます。");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "では、実際にプレイしてみましょう。");

			break;

		case 1 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示
			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 2;	// 説明番号を2にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 0;	// 説明番号を0にする
				}
			}

			mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)

			// 説明文表示
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

		case 2:	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示
			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 3;	// 説明番号を3にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 1;	// 説明番号を1にする
				}
			}

			mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)

			// 説明文表示
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

		case 3 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 4;	// 説明番号を4にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 2;	// 説明番号を2にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
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

		case 4 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示
			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示
			DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 5;	// 説明番号を5にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 3;	// 説明番号を3にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)

			// 説明文表示
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

		case 5 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 6;	// 説明番号を6にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 4;	// 説明番号を4にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
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

		case 6 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 7;	// 説明番号を7にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 5;	// 説明番号を5にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
			DrawFormatString(30, 160, GetColor(0, 0, 0), "3行目は「3 1」で、左右端が黒いマスです。");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "この場合、");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "「左端の黒いマスを含む3個の連続した黒いマ");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "　スと右端の黒いマス1個」");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "としなければ、「3 1」となりません。");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "なので、青枠のマスを塗ることができます。");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "実際に塗るモードで塗ってみましょう");

			break;

		case 7 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 8;	// 説明番号を8にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 6;	// 説明番号を6にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
			DrawFormatString(30, 160, GetColor(0, 0, 0), "4行目と5行目と6行目も先ほどの3行目と同じ");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "考え方で塗ることができます。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "青枠のマスを塗ってみましょう。");

			break;

		case 8 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 9;	// 説明番号を9にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 7;	// 説明番号を7にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
			DrawFormatString(30, 160, GetColor(0, 0, 0), "今赤枠で囲んであるところは完成している部");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "分です。なので、白のマスは確実に黒ではあ");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "りません。");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "後々わかりやすいように赤枠内の白いマスに");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "×印をつけておきましょう。");

			break;

		case 9 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 10;	// 説明番号を10にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 8;	// 説明番号を8にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
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

		case 10 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 11;	// 説明番号を11にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 9;	// 説明番号を9にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
			DrawFormatString(30, 160, GetColor(0, 0, 0), "7列目も先ほどと似た考え方ですが");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "今の6列目の状態は「1 2 1」なの");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "で、真ん中の2つの黒いマスと下端");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "の黒いマスをどちらも含む6個の連");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "続した黒いマスがないと「1 6」に");
			DrawFormatString(30, 260, GetColor(0, 0, 0), "ならないので、青枠のマスを塗るこ");
			DrawFormatString(30, 280, GetColor(0, 0, 0), "とができます。");
			DrawFormatString(30, 300, GetColor(0, 0, 0), "塗るモードで塗りましょう。");

			break;

		case 11 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = 12;	// 説明番号を12にする
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 10;	// 説明番号を10にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
			DrawFormatString(30, 160, GetColor(0, 0, 0), "6列目と5列目も7列目と同じ考え方");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "で完成させることができます。");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "青枠のマスを黒く塗りましょう。");

			break;

		case 12 :	// 説明番号が0のとき
			DrawBox(20, 150, 400, 415, GetColor(255, 255, 255), TRUE);	// 説明文ボックスの背景表示
			DrawBox(20, 150, 400, 415, GetColor(0, 0, 0), FALSE);	// 説明文ボックスの枠表示

			// 説明番号3の次へボタン表示フラグがtrueのとき
			if (processFlag[game->tutorialProcess]) {
				processFlag[game->tutorialProcess] = false;	// 説明番号3の次へボタン表示フラグをfalseにする
				setButton(344, 375, 390, 405, game->nextButton.mImageHandle, NULL, *mouse, &(game->nextButton), TRUE);	// 次へボタンの状態を更新
				DrawGraph(344, 375, game->nextButton.mImageHandle, FALSE);	// 次へボタンを表示
				mouse->mButton = none;	// マウスを無効化(戻るボタン以外押しても反応しなくなる)
			}

			setButton(30, 375, 76, 405, game->returnButton.mImageHandle, NULL, *mouse, &(game->returnButton), TRUE);	// ひとつ前に戻るボタンの状態を更新
			DrawGraph(30, 375, game->returnButton.mImageHandle, FALSE);	// ひとつ前に戻るボタンを表示

			// 次へボタンの状態がtrueのとき 
			if (game->nextButton.mState) {
				// 前の次へボタン状態がfalse(左クリックされた)のとき
				if (!preNextButtonState) {
					game->tutorialProcess = -1;	// 説明番号を-1にする(チュートリアル終了)
				}
			}

			// ひとつ前に戻るボタンの状態がtrueのとき 
			if (game->returnButton.mState) {
				// 前のひとつ前に戻るボタン状態がfalse(左クリックされた)のとき
				if (!preRetrunButtonState) {
					game->tutorialProcess = 11;	// 説明番号を11にする

					// 2重for文で正誤判定用パズルにひとつ前の正解パズルをコピー
					for (int j = 0; j < 10; j++) {
						for (int i = 0; i < 10; i++) {
							game->checkPuzzle.puzzleData[i][j] = game->processAnswerData[game->tutorialProcess][i][j];	// 正誤判定用パズルにひとつ前の正解パズルをコピー

							// 正誤判定用パズルが黒いマス(1)のとき
							if (game->checkPuzzle.puzzleData[i][j] == 1) {
								game->puzzleGrid[i][j].mColor = GetColor(50, 50, 50);	// マスの色を黒にする
							}
							// 正誤判定用パズルが×印つきのマス(0)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == 0) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
							// 正誤判定用パズルが白いマス(-1)のとき
							else if (game->checkPuzzle.puzzleData[i][j] == -1) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// マスの色を白にする
							}
						}
					}
				}
			}

			// 説明文表示
			DrawFormatString(30, 160, GetColor(0, 0, 0), "4列目は「1 4 1」で、今の状態は「1 3 1」");
			DrawFormatString(30, 180, GetColor(0, 0, 0), "です。真ん中の「3」の上に1つ黒を足すか");
			DrawFormatString(30, 200, GetColor(0, 0, 0), "下に1つ黒を足すかで考える場面ですが、");
			DrawFormatString(30, 220, GetColor(0, 0, 0), "上は×印がついているので、下に1つ足すと");
			DrawFormatString(30, 240, GetColor(0, 0, 0), "出来そうです。なので青枠を黒く塗ると…");

			break;

		default : // 上記以外の数値のとき(-1:チュートリアル終了)
			break;
		}
	
		bool penState = game->penButton.mState;	// 更新前の塗るボタンの状態を取得
		bool eraserState = !penState;	// 更新前のバツをつけるボタンの状態を取得

		setButton(55, 435, 145, 525, game->backButton.mImageHandle, NULL, tmpMouse, &(game->backButton), TRUE);	// 戻るボタンの状態更新(いつでもメニュー画面に戻れるようにするため引数のマウスを無効化されないものにしている)
		setButton(175, 435, 265, 525, game->penButton.mImageHandle, NULL, *mouse, &(game->penButton), TRUE);	// 塗るボタンの状態更新
		setButton(295, 435, 385, 525, game->eraserButton.mImageHandle, NULL, *mouse, &(game->eraserButton), TRUE);	// バツをつけるボタンの状態更新

		// 更新前の塗るボタンの状態がtrueのとき
		if (penState) {
			// 更新後のバツをつけるボタンの状態がtrueの時
			if (game->eraserButton.mState) {
				game->eraserButton.mState = true;	// バツをつけるボタンを有効化
				game->penButton.mState = false;	// 塗るボタンを無効化
			}
			// 更新前のバツをつけるボタンの状態がfalseのとき
			else {
				game->penButton.mState = true;	// 塗るボタンを有効にしたままにする
				game->eraserButton.mState = false;	// バツをつけるボタンを無効にしたままにする
			}
		}
		// 更新前の塗るボタンの状態がfalse(バツをつけるボタンの状態がtrue)のとき
		else {
			// 更新後の塗るボタンの状態がtrueのとき
			if (game->penButton.mState) {
				game->penButton.mState = true;	// 塗るボタンを有効化
				game->eraserButton.mState = false;	// バツをつけるボタンを無効化
			}
			// 更新後の塗るボタンの状態がfalseの時
			else {
				game->eraserButton.mState = true;	// バツをつけるボタンを有効にしたままにする
				game->penButton.mState = false;	// 塗るボタンを無効にしたままにする
			}
		}

		// 塗るボタンが有効(バツをつけるボタンが無効)なとき
		if (game->penButton.mState) {
			game->penButton.mImageHandle = game->pressedPenButtonImageHandle;	// 塗るボタンの有効状態のイメージハンドルにする
			game->eraserButton.mImageHandle = game->releaseEraserButtonImageHandle;	// バツをつけるボタンの無効状態のイメージハンドルにする
		}
		// バツをつけるボタンが有効(塗るボタンが無効)なとき
		else if (game->eraserButton.mState) {
			game->eraserButton.mImageHandle = game->pressedEraserButtonImageHandle;	// バツをつけるボタンの有効状態のイメージハンドルにする
			game->penButton.mImageHandle = game->releasePenButtonImageHandle;	// 塗るボタンの無効状態のイメージハンドルにする
		}

		// 戻るボタンが押されて，メニュー画面から遷移してからマウスの左クリックが押され続けていなければ
		if (game->backButton.mState && mouse->waitRelease == 0) {
			mouse->waitRelease = 1;	// マウスのボタンを押し続けているという風にセット
			return MenuScr;	// 遷移先のシーンをシーン番号MenuScrに設定
		}

		int nowMiliSec = GetNowCount();	// 現在時刻(ミリ秒)の取得
		int sec = ((nowMiliSec - (game->startTime)) % 60000) / 1000;	// ゲームスタートしてから現在までの秒単位の時間
		int minute = (nowMiliSec - (game->startTime)) / 60000;	// ゲームスタートしてから現在までの分単位の時間
		DrawGraph(game->backButton.mX1, game->backButton.mY1, game->backButton.mImageHandle, FALSE);	// 戻るボタン描画
		DrawGraph(game->penButton.mX1, game->penButton.mY1, game->penButton.mImageHandle, FALSE);	// 塗るボタン描画
		DrawGraph(game->eraserButton.mX1, game->eraserButton.mY1, game->eraserButton.mImageHandle, FALSE);	// バツをつけるボタン描画
		DrawFormatStringToHandle(70, 100, GetColor(0, 0, 0), game->timeFontHandle, "Time : %02d:%02d", minute, sec);	// ゲームプレイ経過時間(〇〇:〇〇)描画
		int titleWidth = GetDrawFormatStringWidthToHandle(game->puzzleTitleFontHandle, puzzle->puzzleTitle);	// パズルタイトルを表示する文字列のx中心座標
		DrawFormatStringToHandle(720 - (titleWidth / 2), 10, GetColor(0, 0, 0), game->puzzleTitleFontHandle, puzzle->puzzleTitle);	// パズルタイトル描画	

		// 2重for文でパズルの描画とパズルの各マスの状態更新
		for (int j = 0; j < puzzle->y_size; j++) {
			for (int i = 0; i < puzzle->x_size; i++) {
				Button_t preState = game->puzzleGrid[i][j];	// i行j列のマスボタンの更新前の状態を格納
				// 難易度・パズル選択シーンから遷移した時にマウスが押されっぱなしでなければ
				if (mouse->waitRelease != 1) {
					setButton(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, NULL, game->puzzleGrid[i][j].mColor, *mouse, &(game->puzzleGrid[i][j]));	// マウスの左ボタンの押されているいないだけの判定でマス状態を更新
				}

				// i行j列のマスボタンの座標上で，マウスの右ボタンが押されていたら
				if (mouse->mX > game->puzzleGrid[i][j].mX1 && mouse->mX < game->puzzleGrid[i][j].mX2 && mouse->mY > game->puzzleGrid[i][j].mY1 && mouse->mY < game->puzzleGrid[i][j].mY2 && mouse->mButton == mButtonState::right) {
					game->puzzleGrid[i][j].mState = true;	// i行j列のマスの状態を変更される状態に更新
				}

				// i行j列のマスが変更される状態で，そのマスボタンが変更可の場合
				if (game->puzzleGrid[i][j].mState && game->puzzleState[i][j]) {
					//マウスの状態によって分岐
					switch (mouse->mState) {
					case leftClick :	// 左クリック状態の場合
						// i行j列の正誤判定用パズルが白いマス(-1)のとき
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							// 塗るボタンが有効状態の場合
							if (game->penButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 1;	// 正誤判定用のパズルのi行j列を黒いマス(1)にする
								game->fromWhiteChangeLeft = true;	// 左クリックによる白いマスからの変化フラグをtrueにする
								game->toWhiteChangeLeft = false;	// 左クリックによる白いマスへの変化フラグをfalseにする
							}
							// バツをつけるボタンが有効な場合
							else if (game->eraserButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 0;	// 正誤判定用のパズルのi行j列を×印付きのマス(0)にする
								game->fromWhiteChangeLeft = true;	// 左クリックによる白いマスからの変化フラグをtrueにする
								game->toWhiteChangeLeft = false;	// 左クリックによる白いマスへの変化フラグをfalseにする
							}
						}
						// i行j列の正誤判定用のパズルが白いマス(-1)でないとき
						else {
							game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);	// i行j列のマスボタンの色を白にする
							game->checkPuzzle.puzzleData[i][j] = -1;
							game->fromWhiteChangeLeft = false;
							game->toWhiteChangeLeft = true;
						}
						break;

					case leftDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->fromWhiteChangeLeft) {
								if (game->penButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 1;
								}
								else if (game->eraserButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 0;
								}
							}
						}
						else {
							if (game->toWhiteChangeLeft) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
								game->checkPuzzle.puzzleData[i][j] = -1;
							}
						}
						break;

					case rightClick:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							game->checkPuzzle.puzzleData[i][j] = 0;
							game->fromWhiteChangeRight = true;
							game->toWhiteChangeRight = false;
						}
						else {
							game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
							game->checkPuzzle.puzzleData[i][j] = -1;
							game->fromWhiteChangeRight = false;
							game->toWhiteChangeRight = true;
						}
						break;

					case rightDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->fromWhiteChangeRight) {
								game->checkPuzzle.puzzleData[i][j] = 0;
							}
						}
						else {
							if (game->toWhiteChangeRight) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
								game->checkPuzzle.puzzleData[i][j] = -1;
							}
						}
						break;

					case none:
						game->fromWhiteChangeRight = false;
						game->toWhiteChangeRight = false;
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

		else if (game->tutorialProcess == 2) {
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

		else if (game->tutorialProcess == 4) {
			for (int i = 1; i < puzzle->x_size - 1; i++) {
				DrawLine(game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY1, game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY1, GetColor(0, 0, 255), 3);
				DrawLine(game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY1, game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY2, GetColor(0, 0, 255), 3);
				DrawLine(game->puzzleGrid[i][1].mX2, game->puzzleGrid[i][1].mY2, game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY2, GetColor(0, 0, 255), 3);
				DrawLine(game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY2, game->puzzleGrid[i][1].mX1, game->puzzleGrid[i][1].mY1, GetColor(0, 0, 255), 3);
			}
		}

		else if (game->tutorialProcess != -1) {
			int processCounter = 0;
			for (int j = 0; j < puzzle->y_size; j++) {
				for (int i = 0; i < puzzle->x_size; i++) {
					if (game->checkPuzzle.puzzleData[i][j] != game->processAnswerData[game->tutorialProcess][i][j]) {
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
						DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
						DrawLine(game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, GetColor(0, 0, 255), 3);
						DrawLine(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY2, game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, GetColor(0, 0, 255), 3);
					}
					else if (game->checkPuzzle.puzzleData[i][j] == game->processAnswerData[game->tutorialProcess][i][j]) {
						processCounter++;
					}
				}
			}

			if (processCounter == 100) {
				processFlag[game->tutorialProcess] = true;
			}
			else {
				processFlag[game->tutorialProcess] = false;
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

		if (diff == 0 && game->tutorialProcess == -1) {
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
		setButton(215, 610, 305, 700, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton), TRUE);

		if (game->backButton.mState && mouse->waitRelease == 0) {
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

	return GameScr;
}

// M18:ゲームプレイ更新
int UpdateGame(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse, int* key) {
	GetMouseState(mouse, TRUE);

	if (game->clearFlag) {
		bool penState = game->penButton.mState;
		bool eraserState = !penState;

		bool hintState = game->hintButton.mState;

		setButton(55, 435, 145, 525, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton), TRUE);
		setButton(175, 435, 265, 525, game->hintButton.mImageHandle, NULL, *mouse, &(game->hintButton), TRUE);
		setButton(295, 435, 385, 525, game->resetButton.mImageHandle, NULL, *mouse, &(game->resetButton), TRUE);
		setButton(100, 315, 190, 405, game->penButton.mImageHandle, NULL, *mouse, &(game->penButton), TRUE);
		setButton(250, 315, 340, 405, game->eraserButton.mImageHandle, NULL, *mouse, &(game->eraserButton), TRUE);

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

		if (game->backButton.mState && mouse->waitRelease == 0) {
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
				if (mouse->waitRelease != 1) {
					setButton(game->puzzleGrid[i][j].mX1, game->puzzleGrid[i][j].mY1, game->puzzleGrid[i][j].mX2, game->puzzleGrid[i][j].mY2, NULL, game->puzzleGrid[i][j].mColor, *mouse, &(game->puzzleGrid[i][j]));
				}

				if (mouse->mX > game->puzzleGrid[i][j].mX1 && mouse->mX < game->puzzleGrid[i][j].mX2 && mouse->mY > game->puzzleGrid[i][j].mY1 && mouse->mY < game->puzzleGrid[i][j].mY2 && mouse->mButton == mButtonState::right) {
					game->puzzleGrid[i][j].mState = true;
				}

				if (game->puzzleGrid[i][j].mState && game->puzzleState[i][j]) {
					switch (mouse->mState) {
					case leftClick:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->penButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 1;
								game->fromWhiteChangeLeft = true;
								game->toWhiteChangeLeft = false;
							}
							else if (game->eraserButton.mState) {
								game->checkPuzzle.puzzleData[i][j] = 0;
								game->fromWhiteChangeLeft = true;
								game->toWhiteChangeLeft = false;
							}
						}
						else {
							game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
							game->checkPuzzle.puzzleData[i][j] = -1;
							game->fromWhiteChangeLeft = false;
							game->toWhiteChangeLeft = true;
						}
						break;

					case leftDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->fromWhiteChangeLeft) {
								if (game->penButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 1;
								}
								else if (game->eraserButton.mState) {
									game->checkPuzzle.puzzleData[i][j] = 0;
								}
							}
						}
						else {
							if (game->toWhiteChangeLeft) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
								game->checkPuzzle.puzzleData[i][j] = -1;
							}
						}
						break;

					case rightClick:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							game->checkPuzzle.puzzleData[i][j] = 0;
							game->fromWhiteChangeRight = true;
							game->toWhiteChangeRight = false;
						}
						else {
							game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
							game->checkPuzzle.puzzleData[i][j] = -1;
							game->fromWhiteChangeRight = false;
							game->toWhiteChangeRight = true;
						}
						break;

					case rightDrag:
						if (game->checkPuzzle.puzzleData[i][j] == -1) {
							if (game->fromWhiteChangeRight) {
								game->checkPuzzle.puzzleData[i][j] = 0;
							}
						}
						else {
							if (game->toWhiteChangeRight) {
								game->puzzleGrid[i][j].mColor = GetColor(255, 255, 255);
								game->checkPuzzle.puzzleData[i][j] = -1;
							}
						}
						break;

					case none:
						game->fromWhiteChangeRight = false;
						game->toWhiteChangeRight = false;
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
			if (puzzle->ranking[i].flag != 0) {
				int drawRankingNumX = drawRankingLeftLineX - 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%2d", 10) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%d", i + 1) / 2;
				DrawFormatStringToHandle(drawRankingNumX, 52 + (54 * i) + 5, GetColor(0, 0, 0), game->rankingFontHandle, "%d", i + 1);
				int drawRankingNameX = drawRankingRightLineX - (drawRankingRightLineX - drawRankingLeftLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, puzzle->ranking[i].playerId) / 2;
				DrawFormatStringToHandle(drawRankingNameX, 52 + (54 * i) + 5, GetColor(0, 0, 0), game->rankingFontHandle, puzzle->ranking[i].playerId);
				int drawRankingTimeX = drawRankingRightLineX + 2 + (500 - drawRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime) / 2;
				DrawFormatStringToHandle(drawRankingTimeX, 52 + (54 * i) + 5, GetColor(0, 0, 0), game->rankingFontHandle, "%d秒", puzzle->ranking[i].cleartime);
			}
		}

		DrawGraph(215, 610, game->backButton.mImageHandle, FALSE);
		setButton(215, 610, 305, 700, game->backButton.mImageHandle, NULL, *mouse, &(game->backButton), TRUE);

		if (game->backButton.mState && mouse->waitRelease == 0) {
			mouse->waitRelease = 1;
			return MenuScr;
		}

		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(255, 255, 255), TRUE);
		DrawBox(game->puzzleX, game->puzzleY - 100, game->puzzleX + 400, game->puzzleY - 20, GetColor(0, 0, 0), FALSE);
		int drawPlayerRankingLeftLineX = game->puzzleX + 10 + GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "ランク外");
		int drawPlayerRankingRightLineX = game->puzzleX + 400 - 10 - GetDrawFormatStringWidthToHandle(game->rankingFontHandle, "9999秒");
		DrawLine(drawPlayerRankingLeftLineX, game->puzzleY - 100, drawPlayerRankingLeftLineX, game->puzzleY - 20, GetColor(0, 0, 0), 1);
		DrawLine(drawPlayerRankingRightLineX, game->puzzleY - 100, drawPlayerRankingRightLineX, game->puzzleY - 20, GetColor(0, 0, 0), 1);
		int time = (game->finishTime - game->startTime) / 1000;
		int rank = 0;
		for (int i = 0; i < 10; i++) {
			if (puzzle->ranking[i].flag == 0 || puzzle->ranking[i].cleartime > time) {
				if (game->inputNicknameDisplayFlag == -1) {
					game->inputNicknameDisplayFlag = 0;
					SetActiveKeyInput(game->keyHandle);
					break;
				}
				rank = i + 1;
				break;
			}
		}

		if (game->inputNicknameDisplayFlag == 1) {
			rank--;
		}

		int drawPlayerRankingNumX = drawPlayerRankingLeftLineX - (drawPlayerRankingLeftLineX - game->puzzleX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "%d", rank) / 2;
		int drawPlayerRankingNameX = drawPlayerRankingRightLineX - (drawPlayerRankingRightLineX - drawPlayerRankingLeftLineX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "あなた") / 2;
		int drawPlayerRankingTimeX = drawPlayerRankingRightLineX + (game->puzzleX + 400 - drawPlayerRankingRightLineX) / 2 - GetDrawFormatStringWidthToHandle(game->playerRankingFontHandle, "%d秒", time) / 2;
		if (1 <= rank && rank <= 10) {
			DrawFormatStringToHandle(drawPlayerRankingNumX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "%d", rank);
		}
		else {
			DrawFormatStringToHandle(game->puzzleX + 5, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "ランク外");
		}
		DrawFormatStringToHandle(drawPlayerRankingNameX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "あなた");
		DrawFormatStringToHandle(drawPlayerRankingTimeX, game->puzzleY - 75, GetColor(0, 0, 0), game->playerRankingFontHandle, "%d秒", time);

		if (game->inputNicknameDisplayFlag == 0 && game->hintcounter == 0) {
			DrawBox(320, 200, 830, 500, GetColor(200, 200, 200), TRUE);
			DrawBox(320, 200, 830, 500, GetColor(0, 0, 0), FALSE);
			DrawBox(330, 450, 400, 490, GetColor(255, 255, 255), TRUE);
			DrawBox(330, 450, 400, 490, GetColor(0, 0, 0), FALSE);
			DrawFormatString(355, 465, GetColor(0, 0, 0), "OK");

			int str1X = 320 + (830 - 320) / 2 - GetDrawFormatStringWidth("ランキングTOP10入りおめでとうございます!!") / 2;
			int str2X = 320 + (830 - 320) / 2 - GetDrawFormatStringWidth("ランキングに乗せるニックネームを入力してください") / 2;
			int str3X = 320 + (830 - 320) / 2 - GetDrawFormatStringWidth("(半角英数字7文字 全角3文字まで。全角半角空白は使えません)") / 2;
			DrawFormatString(str1X, 210, GetColor(0, 0, 0), "ランキングTOP10入りおめでとうございます!!");
			DrawFormatString(str2X, 230, GetColor(0, 0, 0), "ランキングに乗せるニックネームを入力してください");
			DrawFormatString(str3X, 250, GetColor(0, 0, 0), "(半角英数字16文字 全角8文字まで。全角半角空白は使えません)");

			DrawBox(460, 340, 700, 370, GetColor(0, 0, 0), FALSE);

			static Button_t tmpButton;
			bool preTmpButtonState = tmpButton.mState;
			setButton(330, 450, 400, 490, NULL, NULL, *mouse, &tmpButton, TRUE);
			char player0[1024];
			GetKeyInputString(player0, game->keyHandle);

			string str = string(player0);
			
			if (!str.empty()) {
				if (str.find(" ") == str.length() - 1) {
					str.pop_back();
				}

				if (str.find("　") != -1) {
					str.pop_back();
					str.pop_back();
				}
			}

			char player[17];
			strcpy_s(player, 17, str.c_str());

			if (player[16] != '\0') {
				player[16] = '\0';
			}

			SetKeyInputString(player, game->keyHandle);
			GetKeyInputString((char*)player, game->keyHandle);

			if (tmpButton.mState || CheckHitKey(KEY_INPUT_RETURN) == 1) {
				if (!preTmpButtonState || CheckHitKey(KEY_INPUT_RETURN) == 1) {
					game->inputNicknameDisplayFlag = 1;

					if (strcmp(player, "") == 0) {
						strcpy_s((char*)player, 7, "匿名");
					}

					updateRanking(puzzle, (char*)player, time);
					DeleteKeyInput(game->keyHandle);
				}
			}
			else {
				int black = GetColor(0, 0, 0);
				int white = GetColor(255, 255, 255);
				SetKeyInputStringColor(black, black, white, black, black, white, black);
				DrawKeyInputString(462, 344, game->keyHandle);
			}
		}
	}

	return GameScr;
}

// M19:チュートリアル/ゲームプレイ終了処理
void FinalizeGame(Game_t* game, Puzzle_t* puzzle) {
	InitGraph();
	InitFontToHandle();
}