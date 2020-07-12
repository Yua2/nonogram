#pragma once
#include "Information.h"
#include "MouseState.h"
typedef struct {
	int puzzleId;
	char puzzleTitle[256];
	char puzzleMakerName[256];
	char fastestId[256];
	int fastestTime;
	int flag;
}SimplePuzzle_t;
typedef struct {
	int size;
	int Font1;
	int Font2;
	int scroll;
	int num;
	int HandleP1;
	int HandleP2;
	int HandleS1;
	int HandleS2;
	int HandleS3;
	int HandleS4;
	int HandleS5;
	int HandleR1;
	int HandleR2;
	int HandleUp;
	int HandleDown;
	int scroll_mouse_y;
	int scrollbar_on;
	SimplePuzzle_t *Spz;
}Selection_t;

int InitializeSelection(Selection_t*);
int UpdateSelection(Selection_t*, Puzzle_t*,Mouse_t*);
void FinalizeSelection(Selection_t*);