#include "DxLib.h"
#include "MouseState.h"

/*********************************
***Function Name: GetMouseState
***Designer	: 笹川
***Date		: 2020.7.21
***Function	: 
***Return	:void	--戻り値なし
*********************************/
void GetMouseState(Mouse_t* mouse, int getState) {
	//マウスが左クリックされている場合
	if (GetMouseInput() & MOUSE_INPUT_LEFT) mouse->mButton = left;
	//マウスが右クリックされている場合
	else if (GetMouseInput() & MOUSE_INPUT_RIGHT) mouse->mButton = right;
	//マウスのホイール(または中央のボタン)がクリックされている場合
	else if (GetMouseInput() & MOUSE_INPUT_MIDDLE) mouse->mButton = middle;
	//マウスがクリックされていない場合
	else mouse->mButton = none;
	
	//mouse->mButton = GetMouseInput();
	mouse->mWheel = GetMouseWheelRotVol();
	GetMousePoint(&(mouse->mX), &(mouse->mY));
	static int preState = none;	//

	//
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
