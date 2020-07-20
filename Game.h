/******************************************
*** Designer : 藤川
*** Date     : 2020.7.18
*** Purpose  : チュートリアル/ゲームプレイ画面の
			　　の構造体Game_tのメンバと各関数の
			   プロトタイプ宣言(M12~M15)
*******************************************/

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
	Button_t penButton;	// 塗るボタン変数
	int pressedPenButtonImageHandle;	// 有効状態の塗るボタンの画像ハンドル
	int releasePenButtonImageHandle;	// 無効状態の塗るボタンの画像ハンドル
	Button_t eraserButton;	// バツをつけるボタン変数
	int pressedEraserButtonImageHandle;	// 有効状態のバツをつけるボタンの画像ハンドル
	int releaseEraserButtonImageHandle;	// 無効状態のバツをつけるボタンの画像ハンドル
	Button_t hintButton;	// ヒントボタン変数
	Button_t resetButton;	// リセットボタン変数
	Button_t backButton;	// 戻るボタン変数
	int	puzzleX;	// 表示するパズルのx座標
	int puzzleY;	// 表示するパズルのy座標
	int puzzleGridSize;	// 表示するパズルの1マスのサイズ
	int drawGrid_V[20][10];	// 塗る場所を示す左辺表示数列
	int drawGrid_H[20][10];	// 塗る場所を示す上辺表示数列
	int drawNumFontHandle;	// 数列を表示する際のフォントハンドル
	Button_t puzzleGrid[20][20];	// 表示パズルのマスごとのボタン変数
	bool puzzleState[20][20];	// 変更可能なマスかどうかのチェック配列
	bool toWhiteChangeLeft;	// 左クリックによる白いマスへの変化フラグ
	bool fromWhiteChangeLeft;	// 左クリックによる白いマスからの変化フラグ
	bool toWhiteChangeRight;	// 右クリックによる白いマスへの変化フラグ
	bool fromWhiteChangeRight;	// 右クリックによる白いマスからの変化フラグ
	Puzzle_t checkPuzzle;	// 正誤判定用パズル変数
	bool clearFlag;	// クリアフラグ(true:クリアしてない，false:クリアしている) 
	int inputNicknameDisplayFlag;	// ニックネーム入力ウィンドウの状態(1:入力済み，0:入力中，-1:未入力)
	int rankingTitleFontHnadle;	// ランキングタイトルのフォントハンドル
	int rankingFontHandle;	// ランキングのフォントハンドル
	int playerRankingFontHandle;	// プレイヤーのプレイ結果用のフォントハンドル
	int keyHandle;	// 入力キーハンドル
	int tutorialProcess;	// 説明番号
	Button_t nextButton;	// チュートリアルの「次へ」ボタン
	Button_t returnButton;	// チュートリアルの「ひとつ前に戻る」ボタン
	int hintcounter;	// ヒント使用回数
	int processAnswerData[13][10][10];	// 説明番号に対応した正解パズル
};

// M16:チュートリアル/ゲームプレイ初期化
int InitializeGame(Game_t* game, Puzzle_t* puzzle);

// M17:チュートリアル画面更新
int UpdateTutorial(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse);

// M18:ゲームプレイ更新
int UpdateGame(Game_t* game, Puzzle_t* puzzle, Mouse_t* mouse);

// M19:チュートリアル/ゲームプレイ終了処理
void FinalizeGame(Game_t* game, Puzzle_t* puzzle);

#endif /* GAME_SCR_H_ */