#ifndef INFORMATION_H_
#define INFORMATION_H_


// F2: パズル/ランキング情報
typedef struct RANKING {
	char playerId[17];
	int cleartime;
	int flag;
}RANKING_t;
typedef struct PUZZLE {
	int  puzzleId;				// パズルID
	char puzzleTitle[17];		// パズルタイトル
	int  x_size;
	int  y_size;
	int  puzzleData[20][20];	// パズルデータ
	char puzzleMakerId[17];			// パズル作成者ID
	RANKING_t ranking[10];
} Puzzle_t;


#endif // INFORMATION_H_

