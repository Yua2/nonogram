#pragma once
#include "MouseState.h"
#include "Information.h"
typedef struct {
	int leftx, rightx, upy, downy, charx, chary;  //ボタンの左，右，上，下の境界と文字の座標
	char name[128];								  //ボタン内の文字
}menubutton;
typedef struct {
	int x, y;
	char name[128];
	menubutton b[3];
}Menu_t;


int InitializeMenu(Menu_t*);
int UpdateMenu(Menu_t*, Mouse_t*, Puzzle_t*);
void FinalizeMenu(Menu_t*);
