#include "DxLib.h"
#include "MouseState.h"

/*********************************
***Function Name: GetMouseState
***Designer	: 笹川
***Date		: 2020.7.21
***Function	: マウスの状態(クリックとドラッグ)を調べ、Mouse_t構造体のデータを更新する
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
		//マウスが押されていない場合
		if (mouse->mButton == none) {
			preState = none;
		}
		//マウスが押されている場合
		else {
			//今のマウスの状態が以前のマウスの状態と一致していない場合
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
			//今のマウスの状態が以前のマウスの状態と一致している場合
			//(マウスのボタンが長押しされている場合)
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
