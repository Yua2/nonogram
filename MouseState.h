#ifndef MOUSE_STATE_H_
#define MOUSE_STATE_H_

#include "DxLib.h"

enum mButtonState { none, left, right, middle, rightDrag, leftDrag, rightClick, leftClick };

typedef struct MOUSE {
	int mButton;
	int mWheel;
	int mX;
	int mY;
	int waitRelease;
	int mState;
} Mouse_t;

// M4:マウス処理
void GetMouseState(Mouse_t*, int getState = FALSE);

#endif /* MOUSESTATE_H_ */