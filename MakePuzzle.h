#pragma once
typedef struct {
	int i;
}MakePuzzle_t;
int InitializeMakePuzzle(MakePuzzle_t*);
int UpdateMakePuzzle(MakePuzzle_t*);
void FinalizeMakePuzzle(MakePuzzle_t*);