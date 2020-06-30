#ifndef INFORMATION_H_
#define INFORMATION_H_

// F1: プレイヤー情報
typedef struct PLAYER {
	int  playerId;	// プレイヤーID
	char nickName[256];	// ニックネーム
} Player_t;

// F2: パズル/ランキング情報
typedef struct PUZZLE {
	int  puzzleId;				// パズルID
	char puzzleTitle[256];		// パズルタイトル
	int  puzzleData[20][20];	// パズルデータ
	int  puzzleMakerID;			// パズル作成者ID
	int* playedPlayerId;		// プレイ済みプレイヤーのID
	int* timeRanking;			// タイムランキング
	int  ranking;				// ランキング
} Puzzle_t;

#endif // INFORMATION_H_

