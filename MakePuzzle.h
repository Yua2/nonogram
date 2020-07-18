#ifndef MAKEPUZZLE_H_
#define MAKEPUZZLE_H_
#include "Controller.h"
#include "Information.h"
#include "Dxlib.h"

typedef struct MakePuzzle {
	Button_t eraserButton;
	Button_t resetButton;
	Button_t backButton;


	int posi = 100;
	int selectsize = 40;
	int sellsize = 25; //
	int semisize = 10;  // 
	int x_size = 20; // 
	int y_size = 20;// 
	// 
	int	white = GetColor(255, 255, 255), black = GetColor(0, 0, 0), gainsboro = GetColor(220, 220, 220),
		blue = GetColor(0, 0, 255), aqua = GetColor(0, 255, 255),
		green = GetColor(0, 128, 0), springgreen = GetColor(0, 255, 127),
		maroon = GetColor(128, 0, 0), red = GetColor(255, 0, 0),
		deeppink = GetColor(255, 20, 147), pink = GetColor(255, 0, 255),
		saddlebrown = GetColor(139, 69, 19), orange = GetColor(255, 165, 0),
		gold = GetColor(255, 215, 0), lemonchiffon = GetColor(255, 250, 205),
		indigo = GetColor(75, 0, 130), mediumpurple = GetColor(147, 112, 219);

	//
	int	col[8][2] = { {black, gainsboro}, {blue, aqua}, {green, springgreen},{maroon, red},
		{deeppink, pink}, {saddlebrown, orange}, {gold, lemonchiffon}, {indigo, mediumpurple} };

	int colorlist[20][20] = { white }; //

	int tmp = white; //


	Puzzle_t puzzle;

} MakePuzzle_t;

int InitializeMakePuzzle(MakePuzzle_t*);
int UpdateMakePuzzle(MakePuzzle_t*);
void FinalizeMakePuzzle(MakePuzzle_t*);
//int click(Mouse_t*, int, int, int ,int);
void savePuzzle(Puzzle_t*);
char* replace(char*, const char*, const char*);
#endif /* MAKEPUZZLE_H_*/