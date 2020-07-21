
/*************************
***Designer	:笹川
***Date		:2020.7.21
***Purpose	:マウスの状態(クリックされているかどうか、
			クリックが長押しかどうか)を判定する。
*************************/

#ifndef MOUSE_STATE_H_
#define MOUSE_STATE_H_

#include "DxLib.h"

enum mButtonState { none, left, right, middle, rightDrag, leftDrag, rightClick, leftClick };	//ボタンの状態を示す列挙体

typedef struct MOUSE {
	int mButton;	//マウスクリックの状態
	int mWheel;	//マウスホイールの回転量
	int mX;		//マウスのX座標
	int mY;		//マウスのY座標
	int waitRelease;//マウスのボタンが離されたかどうかを保持する
	int mState;	//マウスがドラックされている場合に状態を保持する
} Mouse_t;

// M4:マウス処理
void GetMouseState(Mouse_t*, int getState = FALSE);

#endif /* MOUSESTATE_H_ */
