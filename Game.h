#pragma once
#include "Information.h"
typedef struct {
	int i;
}Game_t;
int InitializeGame(Game_t*);
int UpdateGame(Game_t*,Puzzle_t*);
void FinalizeGame(Game_t*);