/******************************************************
*** Designer      : 杉田
*** Date          : 2020.6.24
*** Purpose       : W5 パズル作成画面
*******************************************************/


#include "DxLib.h"
#include "MouseState.h"
#include "Controller.h"
#include "MakePuzzle.h"
#include "Information.h"
#include "SceneMgr.h"
#include "PuzzleConverter.h"
#include "InputNickName.h"
#include "file.h"
#include <string.h>
#include <stdlib.h>
#include <windows.h>
static int flag = 0;
static Puzzle_t puzzle;
static Player_t player;
int click(Mouse_t* mouse, int x1, int y1, int x2, int y2);
/******************************************************
/******************************************************
*** Function Name	: InitializeMakePuzzle
*** Designer		: 杉田
*** Date			: 2020.6.28
*** Function		: パズル作成画面を表示する
*** Return			: MakePuzzleScr
******************************************************/
int InitializeMakePuzzle(MakePuzzle_t* create) {
	Mouse_t mouse;


	GetMouseState(&mouse);

	int backImageHandle = LoadGraph("graph/backButton.bmp");
	int resetImageHandle = LoadGraph("graph/resetButton.bmp");
	//int penImageHandle = LoadGraph("graph/penButton.bmp");
	int eraserImageHandle = LoadGraph("graph/eraserButton.bmp");

	setButton(5, 630, 95, 720, backImageHandle, NULL, mouse, &(create->backButton));
	setButton(150, 630, 240, 720, resetImageHandle, NULL, mouse, &(create->resetButton));
	setButton(20, 20, 110, 110, eraserImageHandle, NULL, mouse, &(create->eraserButton));

	DrawGraph(create->backButton.mX1, create->backButton.mY1, create->backButton.mImageHandle, FALSE);
	DrawGraph(create->resetButton.mX1, create->resetButton.mY1, create->resetButton.mImageHandle, FALSE);
	DrawGraph(create->eraserButton.mX1, create->eraserButton.mY1, create->eraserButton.mImageHandle, FALSE);

	DrawBox(280, 30, 370, 80, create->black, FALSE);
	DrawBox(430, 30, 520, 80, create->black, FALSE);
	DrawBox(580, 30, 670, 80, create->black, FALSE);
	DrawBox(730, 30, 820, 80, create->black, FALSE);
	DrawBox(880, 30, 970, 80, create->black, FALSE);

	DrawBox(830, 180, 1000, 230, create->black, FALSE);
	DrawBox(830, 330, 1000, 380, create->black, FALSE);
	//DrawBox(830, 430, 1000, 480, GetColor(0, 0, 0), FALSE);

	DrawFormatString(290, 50, create->black, "10 × 10");
	DrawFormatString(440, 50, create->black, "15 × 10");
	DrawFormatString(590, 50, create->black, "15 × 15");
	DrawFormatString(740, 50, create->black, "20 × 15");
	DrawFormatString(890, 50, create->black, "20 × 20");

	DrawFormatString(850, 200, create->black, "画像を取り込む");
	DrawFormatString(850, 350, create->black, "パズルを保存する");

	DrawFormatString(820, 450, create->black, "16色から好きな色を選んで");
	DrawFormatString(820, 480, create->black, "パズルを作成しましょう!");
	DrawFormatString(820, 560, create->black, "・左クリックで色を選び");
	DrawFormatString(820, 590, create->black, "  マスを塗ることができます");
	DrawFormatString(820, 620, create->black, "・色選択マスの上段は");
	DrawFormatString(820, 650, create->black, "  ゲーム画面で黒く塗られます");
	DrawFormatString(80, 410, create->black, "ゲーム時の画面");


	// DrawFormatString(850, 450, GetColor(0, 0, 0), "パズルに変換");


	// パズルの初期画面表示と縮小パズルの初期画面表示　色は白
	for (int j = 0; j < create->y_size; j++) {
		for (int i = 0; i < create->x_size; i++) {
			int S = create->posi + i * create->sellsize + 200; // パズルマスの横
			int T = create->posi + j * create->sellsize; // パズルマスの縦
			int s = create->posi + i * create->semisize - 50; // 縮小パズルマスの横
			int t = create->posi + j * create->semisize + 100; // 縮小パズルマスの縦
			puzzle.x_size = create->x_size;
			puzzle.y_size = create->y_size;
			DrawBox(s, t, s + create->semisize - 1, t + create->semisize - 1, create->white, TRUE);
			DrawBox(S, T, S + create->sellsize - 1, T + create->sellsize - 1, create->white, TRUE);
		}
	}

	// 縮小パズルの初期画面表示、色は白
	/*for (int j = 0; j < Size; j++) {
		for (int i = 0; i < Size; i++) {
			int s = Posi + i * semisize-50; // パズルマスの横
			int t = Posi + j * semisize + 50; // パズルマスの縦
			DrawBox(s, t, s + semisize - 1, t + semisize - 1, white, TRUE);
		}
	}*/

	// 色を選択するマスを表示
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 8; i++) {
			int S = create->posi + i * create->selectsize + 270; // パズルマスの横
			int T = create->posi + j * create->selectsize; // パズルマスの縦
			DrawBox(S, T + 520, S + create->selectsize - 1, T + create->selectsize - 1 + 520, create->col[i][j], TRUE); // 520とかはパズル画面表示の調整
		}
	}

	return MakePuzzleScr;
}

/***************************************
*** Function Name	: UpdateMakePuzzle
*** Designer		: 杉田
*** Date			: 2020.6.28
*** Function		: パズル作成画面の更新
*** Return			: 遷移先の画面を指定
*******************************************/

int UpdateMakePuzzle(MakePuzzle_t* create) {
	InitializeMakePuzzle(create);
	Mouse_t mouse;
	GetMouseState(&mouse);

	// サイズ選択ボタンが押されたらパズルのサイズを変更する
	// 10*10
	if ((&mouse)->mButton == left && click(&(mouse), 280, 30, 370, 80)) {
		create->x_size = 10;
		create->y_size = 10;
		create->sellsize = 50;
		create->semisize = 20;
		puzzle.x_size = create->x_size;
		puzzle.y_size = create->y_size;
		DrawFormatString(290, 50, create->aqua, "10 × 10");

		//DrawBox(290, 40, 360, 70, create->gainsboro, FALSE);

	}
	// 15*10
	if ((&mouse)->mButton == left && click(&(mouse), 430, 30, 520, 80)) {
		create->x_size = 15;
		create->y_size = 10;
		create->sellsize = 33;
		create->semisize = 13;
		puzzle.x_size = create->x_size;
		puzzle.y_size = create->y_size;
		DrawFormatString(440, 50, create->aqua, "15 × 10");

	}
	// 15*15
	if ((&mouse)->mButton == left && click(&(mouse), 580, 30, 670, 80)) {
		create->x_size = 15;
		create->y_size = 15;
		create->sellsize = 33;
		create->semisize = 13;
		puzzle.x_size = create->x_size;
		puzzle.y_size = create->y_size;
		DrawFormatString(590, 50, create->aqua, "15 × 15");

	}
	// 20*15
	if ((&mouse)->mButton == left && click(&(mouse), 740, 30, 830, 80)) {
		create->x_size = 20;
		create->y_size = 15;
		create->sellsize = 25;
		create->semisize = 10;
		puzzle.x_size = create->x_size;
		puzzle.y_size = create->y_size;
		DrawFormatString(740, 50, create->aqua, "20 × 15");

	}
	// 20*20
	if ((&mouse)->mButton == left && click(&(mouse), 890, 30, 980, 80)) {
		create->x_size = 20;
		create->y_size = 20;
		create->sellsize = 25;
		create->semisize = 10;
		puzzle.x_size = create->x_size;
		puzzle.y_size = create->y_size;
		DrawFormatString(890, 50, create->aqua, "20 × 20");

	}

	// 消しゴムボタンが押されたら白色の情報を保持
	if ((&mouse)->mButton == left && click(&(mouse), 20, 20, 110, 110)) {
		create->tmp = create->white;
	}

	// 選択する色をクリックしたら、その色を保持する
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 8; i++) {
			int S = create->posi + i * create->selectsize + 270; // パズルマスの横
			int T = create->posi + j * create->selectsize; // パズルマスの縦
			// 左クリックされたら色の情報を保持
			if ((&mouse)->mButton == left && click(&(mouse), S, T + 520, S + create->selectsize - 1, T + create->selectsize - 1 + 520)) {
				DrawBox(S - 10, T + 510, S + create->selectsize - 1 + 10, T + create->selectsize - 1 + 530, create->col[i][j], TRUE);

				//DrawBox(S+1, T+1, S + create->semisize-2, T + create->semisize-2, create->white, FALSE);
				create->tmp = create->col[i][j];
			}
		}
	}

	// パズルの画面表示
	for (int j = 0; j < create->y_size; j++) {
		for (int i = 0; i < create->x_size; i++) {
			int S = create->posi + i * create->sellsize + 200; // パズルマスの横
			int T = create->posi + j * create->sellsize; // パズルマスの縦
			int s = create->posi + i * create->semisize - 50;
			int t = create->posi + j * create->semisize + 100;

			// マスを左クリックし, 色を反映させる準備
			if ((&mouse)->mButton == left && click(&(mouse), S, T, S + create->sellsize - 1, T + create->sellsize - 1)) {

				// 色塗り中であり, マスに色が塗られていない、または白色の場合リストに格納
				if (create->colorlist[i][j] == NULL || create->colorlist[i][j] == create->white) {
					create->colorlist[i][j] = create->tmp;
				}
				// 消しゴムボタンにより, 塗られているマスを白にするときの処理
				else if (create->tmp == create->white) {
					create->colorlist[i][j] = create->tmp;
				}
			}

			// colorlistの色情報を画面に反映
			if (create->colorlist[i][j] != 0) {
				DrawBox(S, T, S + create->sellsize - 1, T + create->sellsize - 1, create->colorlist[i][j], TRUE);

				// 濃い色が塗られている場合, 縮小パズルのマスを黒く塗る
				if (create->colorlist[i][j] != create->white)
					if (create->colorlist[i][j] != create->gainsboro && create->colorlist[i][j] != create->aqua &&
						create->colorlist[i][j] != create->springgreen && create->colorlist[i][j] != create->red &&
						create->colorlist[i][j] != create->pink && create->colorlist[i][j] != create->orange &&
						create->colorlist[i][j] != create->lemonchiffon && create->colorlist[i][j] != create->mediumpurple) {
						DrawBox(s, t, s + create->semisize - 1, t + create->semisize - 1, create->black, TRUE);
					}

				// 色情報を決められた値に代入する
				if (create->colorlist[i][j] == create->white)
					puzzle.puzzleData[i][j] = 0;

				if (create->colorlist[i][j] == create->black)
					puzzle.puzzleData[i][j] = 101;

				if (create->colorlist[i][j] == create->gainsboro)
					puzzle.puzzleData[i][j] = 1;

				if (create->colorlist[i][j] == create->blue)
					puzzle.puzzleData[i][j] = 102;

				if (create->colorlist[i][j] == create->aqua)
					puzzle.puzzleData[i][j] = 2;

				if (create->colorlist[i][j] == create->green)
					puzzle.puzzleData[i][j] = 103;

				if (create->colorlist[i][j] == create->springgreen)
					puzzle.puzzleData[i][j] = 3;

				if (create->colorlist[i][j] == create->maroon)
					puzzle.puzzleData[i][j] = 104;

				if (create->colorlist[i][j] == create->red)
					puzzle.puzzleData[i][j] = 4;

				if (create->colorlist[i][j] == create->deeppink)
					puzzle.puzzleData[i][j] = 105;

				if (create->colorlist[i][j] == create->pink)
					puzzle.puzzleData[i][j] = 5;

				if (create->colorlist[i][j] == create->saddlebrown)
					puzzle.puzzleData[i][j] = 106;

				if (create->colorlist[i][j] == create->orange)
					puzzle.puzzleData[i][j] = 6;

				if (create->colorlist[i][j] == create->gold)
					puzzle.puzzleData[i][j] = 107;

				if (create->colorlist[i][j] == create->lemonchiffon)
					puzzle.puzzleData[i][j] = 7;

				if (create->colorlist[i][j] == create->indigo)
					puzzle.puzzleData[i][j] = 108;

				if (create->colorlist[i][j] == create->mediumpurple)
					puzzle.puzzleData[i][j] = 8;

			}
		}
	}

	// 二重ループでパズル画面をチェック
	for (int j = 0; j < 20; j++) {
		for (int i = 0; i < 20; i++) {
			int S = create->posi + i * create->sellsize + 200; // パズルマスの横
			int T = create->posi + j * create->sellsize; // パズルマスの縦

				// リセットボタンが押されたらすべてのマスを初期状態（白色）にする
			if ((&mouse)->mButton == left && click(&(mouse), 150, 630, 240, 720)) {
				create->colorlist[i][j] = create->white;
			}
		}
	}

	// 画像を取り込むボタンが押されたらエクスプローラーを起動し、パズル作成処理部に選択された画像を送信する
	// とりあえず、今は画像を取り込むまで
	if ((&mouse)->mButton == left && click(&(mouse), 830, 180, 1000, 230)) {
		DrawFormatString(850, 200, create->aqua, "画像を取り込む");

		char FullPath[MAX_PATH], FileName[MAX_PATH], CurrentDir[MAX_PATH];

		// ウインドウモードで起動
		ChangeWindowMode(TRUE);

		OPENFILENAME ofn;

		memset(&ofn, 0, sizeof(OPENFILENAME));
		memset(FullPath, 0, sizeof(FullPath));
		memset(FileName, 0, sizeof(FileName));
		memset(CurrentDir, 0, sizeof(CurrentDir));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetMainWindowHandle();
		// 現在のカレントディレクトリを取得
		GetCurrentDirectory(MAX_PATH, CurrentDir);
		// 説明の末尾に \0 を記載して、その後ろに表示するファイルの指定、最後に \0\0 を記述
		ofn.lpstrFilter = "Bitmap File  or  Jpeg File  or  Png File\0*.bmp;*.jpg;*.png\0\0";

		// lpstrFile に指定する配列にファイルのフルパスが代入される
		ofn.lpstrFile = FullPath;
		ofn.nMaxFile = sizeof(FullPath);

		// lpstrFileTitle に指定する配列にファイル名( フォルダパスが無い )が代入される
		ofn.lpstrFileTitle = FileName;
		ofn.nMaxFileTitle = sizeof(FileName);

		ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "";

		// lpstrTitle に表示されるダイアログの説明を代入
		ofn.lpstrTitle = "画像ファイルの読み込み";

		// パスの指定が成功した場合は GetOpenFileName の戻り値は 0 以外になる
		if (GetOpenFileName(&ofn) != 0)
		{
			// ファイル名を取得できたら画像を読み込む
			pzAdjust(FullPath, puzzle.x_size, puzzle.y_size, &puzzle);
			// 読み込んだパズルデータを代入
			for (int i = 0; i < puzzle.y_size; i++) {
				for (int j = 0; j < puzzle.x_size; j++) {
					if (puzzle.puzzleData[i][j] ==   0) create->colorlist[j][i] = create->white;
					if (puzzle.puzzleData[i][j] == 101) create->colorlist[j][i] = create->black;
					if (puzzle.puzzleData[i][j] ==   1) create->colorlist[j][i] = create->gainsboro;
					if (puzzle.puzzleData[i][j] == 102) create->colorlist[j][i] = create->blue;
					if (puzzle.puzzleData[i][j] ==   2) create->colorlist[j][i] = create->aqua;
					if (puzzle.puzzleData[i][j] == 103) create->colorlist[j][i] = create->green;
					if (puzzle.puzzleData[i][j] ==   3) create->colorlist[j][i] = create->springgreen;
					if (puzzle.puzzleData[i][j] == 104) create->colorlist[j][i] = create->maroon;
					if (puzzle.puzzleData[i][j] ==   4) create->colorlist[j][i] = create->red;
					if (puzzle.puzzleData[i][j] == 105) create->colorlist[j][i] = create->deeppink;
					if (puzzle.puzzleData[i][j] ==   5) create->colorlist[j][i] = create->pink;
					if (puzzle.puzzleData[i][j] == 106) create->colorlist[j][i] = create->saddlebrown;
					if (puzzle.puzzleData[i][j] ==   6) create->colorlist[j][i] = create->orange;
					if (puzzle.puzzleData[i][j] == 107) create->colorlist[j][i] = create->gold;
					if (puzzle.puzzleData[i][j] ==   7) create->colorlist[j][i] = create->lemonchiffon;
					if (puzzle.puzzleData[i][j] == 108) create->colorlist[j][i] = create->indigo;
					if (puzzle.puzzleData[i][j] ==   8) create->colorlist[j][i] = create->mediumpurple;
				}
			}
		}
		// カレントディレクトリを元に戻す
		SetCurrentDirectory(CurrentDir);
	}
	//DrawBox(830, 280, 1000, 330, create->black, FALSE);


	// パズルを保存するボタンが押されたらflagを1にし、タイトル入力画面を表示
	if ((&mouse)->mButton == left && click((&mouse), 830, 330, 1000, 380)) {
		flag = 1;
	}
	// ニックネームを入力する処理
	if (flag == 1) {
		DrawBox(350, 200, 800, 500, create->black, TRUE);
		//DrawBox(370, 450, 440, 490, create->white, FALSE);
		//DrawFormatString(400, 460, create->white, "OK");

		DrawFormatString(420, 210, create->white, "タイトルを付けてください", TRUE);
		DrawFormatString(420, 230, create->white, "(Enterキーで決定) (Escキーで作成画面に戻る)", TRUE);
		DrawBox(460, 340, 700, 370, create->white, FALSE);
		KeyInputString(470, 350, 256, puzzle.puzzleTitle, create->white);

		// Enterキーが押されたらニックネーム入力に移る
		if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
			flag++;
		}
		// Escキーが押されたら入力画面を閉じる
		else if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			flag = 0;
		}
	}
	// ニックネーム入力画面の処理
	else if (flag == 2) {
		DrawBox(350, 200, 800, 500, create->black, TRUE);
		DrawBox(460, 340, 700, 370, create->white, FALSE);
		DrawFormatString(420, 210, create->white, "ニックネームを付けてください", TRUE);
		DrawFormatString(420, 230, create->white, "(Enterキーで決定) (Escキーで作成画面に戻る)", TRUE);
		KeyInputString(470, 350, 256, puzzle.puzzleMakerId, create->white);

		// Enterキーが押されたらflag=3に移動
		if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
			// ランキング情報を更新
			for (int i = 0; i < 10; i++) {
				puzzle.ranking[i].flag = 0;
				sprintf_s(puzzle.ranking[i].playerId, "none");
				puzzle.ranking[i].cleartime = 0;
			}
			savePuzzle(&(puzzle));

			flag++;
		}
		// Escキーが押されたら作成画面に戻る
		else if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			flag = 0;
		}
	}
	// 保存完了メッセージを表示 flag=4に移動
	else if (flag == 3) {
		DrawBox(350, 200, 800, 500, create->black, TRUE);
		DrawFormatString(500, 300, create->white, "保存しました", TRUE);
		flag++;
	}
	// 2秒間メッセージを表示させて作成画面に戻る
	else if (flag == 4) {
		Sleep(2000);
		flag = 0;
	}


	// 戻るボタンが押されたらメニュー画面に移動
	if ((&mouse)->mButton == left && click((&mouse), 5, 630, 95, 720)) {
		for (int j = 0; j < 20; j++) {
			for (int i = 0; i < 20; i++) {
				int S = create->posi + i * create->sellsize + 200; // パズルマスの横
				int T = create->posi + j * create->sellsize; // パズルマスの縦

				create->colorlist[i][j] = create->white;
			}
		}


		return MenuScr;
	}

	ScreenFlip();
	return MakePuzzleScr;
}


/******************************
*** Function Name	: FinalizelizeMakePuzzle
*** Designer		: 杉田
*** Date			: 2020.6.28
*** Function		: パズル作成画面を終了する

*** Return			: 戻り値なし
******************************/
void FinalizeMakePuzzle(MakePuzzle_t* create) {
	InitGraph();
	InitFontToHandle();
}



/******************************
*** Function Name	: click
*** Designer		: 笹川
*** Date			: 2020.6.28
*** Function		: クリック判定
*** Return			: 成功: 1 or 失敗: 0
******************************/
int click(Mouse_t* mouse, int x1, int y1, int x2, int y2) {
	if (mouse->mX > x1 && mouse->mX < x2 && mouse->mY > y1 && mouse->mY < y2) {
		return 1;
	}
	return 0;
}