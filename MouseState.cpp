#include "DxLib.h"
#include "MouseState.h"

void GetMouseState(Mouse_t* mouse, int getState) {
	if (GetMouseInput() & MOUSE_INPUT_LEFT) mouse->mButton = left;
	else if (GetMouseInput() & MOUSE_INPUT_RIGHT) mouse->mButton = right;
	else if (GetMouseInput() & MOUSE_INPUT_MIDDLE) mouse->mButton = middle;
	else mouse->mButton = none;
	//mouse->mButton = GetMouseInput();			// マウスの入力状態取得
	mouse->mWheel = GetMouseWheelRotVol();		// マウスのホイール回転量取得
	GetMousePoint(&(mouse->mX), &(mouse->mY));	// マウスの座標取得
	static int preState = none;

	if (getState == TRUE) {
		if (mouse->mButton == none) {
			preState = none;
		}
		else {
			if (mouse->mButton != preState) {
				switch (mouse->mButton)
				{
				case left :
					mouse->mState = leftClick;
					break;
				case right :
					mouse->mState = rightClick;
					break;
				default:
					mouse->mState = none;
					break;
				}
				preState = mouse->mButton;
			}
			else {
				switch (mouse->mButton)
				{
				case left :
					mouse->mState = leftDrag;
					break;
				case right :
					mouse->mState = rightDrag;
					break;
				default:
					mouse->mState = none;
					break;
				}
				preState = mouse->mButton;
			}
		}
	}
}