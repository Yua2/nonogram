#include "DxLib.h"
#include "MouseState.h"

void GetMouseState(Mouse_t* mouse) {
	mouse->mButton = GetMouseInput();			// マウスの入力状態取得
	mouse->mWheel = GetMouseWheelRotVol();		// マウスのホイール回転量取得
	GetMousePoint(&(mouse->mX), &(mouse->mY));	// マウスの座標取得
}