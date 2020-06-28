#include "DxLib.h"
#include "MouseState.h"

void GetMouseState(Mouse_t* mouse) {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) mouse->mButton = left;
	else if (GetMouseInput() & MOUSE_INPUT_RIGHT) mouse->mButton = right;
	else if (GetMouseInput() & MOUSE_INPUT_MIDDLE) mouse->mButton = middle;
	else mouse->mButton = none;
	mouse->mButton = GetMouseInput();			// マウスの入力状態取得
	mouse->mWheel = GetMouseWheelRotVol();		// マウスのホイール回転量取得
	GetMousePoint(&(mouse->mX), &(mouse->mY));	// マウスの座標取得
}