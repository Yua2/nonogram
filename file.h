/******************************************************
*** Designer      : 陳
*** Date          : 2020.7.6
*** Purpose       : ファイル入出力に関する情報
*******************************************************/
#pragma once
#include "Information.h"
void readPuzzle(Puzzle_t* , char[64]);
void savePuzzle(Puzzle_t*);
void updateRanking(Puzzle_t*, char[256], int);