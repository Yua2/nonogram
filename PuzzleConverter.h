#ifndef PUZZLE_CONVERTER_H_
#define PUZZLE_CONVERTER_H_

#include "Information.h"

int Monokuro(int *pz_data, int width, int height);
int pzAdjust(char *fileName, int pzl_w, int pzl_h, Puzzle_t *ret);

#endif /* PUZZLE_CONVERTER_H_ */