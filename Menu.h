#pragma once
#include "MouseState.h"
#include "Information.h"
typedef struct {
	int leftx, rightx, upy, downy, charx, chary;  //ボタンの左，右，上，下の境界と文字の座標
	char name[128];								  //ボタン内の文字
}menubutton;
typedef struct {
	int x,y;									  //メニュー画面に表示する文字の座標
	char name[128];								  //メニュー画面に表示する文字の座標
	menubutton b[3];							  //メニュー画面に表示するボタンの情報
}Menu_t; 


int InitializeMenu(Menu_t*);
int UpdateMenu(Menu_t*,Mouse_t*,Puzzle_t*);
void FinalizeMenu(Menu_t*);
