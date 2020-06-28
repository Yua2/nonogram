#pragma once
#include "Information.h"
typedef struct {
	int i;
}Selection_t;
int InitializeSelection(Selection_t*);
int UpdateSelection(Selection_t*, Puzzle_t*);
void FinalizeSelection(Selection_t*);