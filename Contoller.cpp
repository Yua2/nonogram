#include "DxLib.h"
#include "Controller.h"
#include "MouseState.h"

// ボタン関数
void setButton(int x1, int y1, int x2, int y2, int imageHandle, unsigned int color, Mouse_t mouse, Button_t* button) {
	button->mX1 = x1;
	button->mY1 = y1;
	button->mX2 = x2;
	button->mY2 = y2;

	if (color == NULL) {
		button->mImageHandle = imageHandle;
		button->mColor = NULL;
	}
	else {
		button->mImageHandle = NULL;
		button->mColor = color;
	}

	if ((mouse.mX > x1) && (mouse.mX < x2) && (mouse.mY > y1) && (mouse.mY < y2) && (mouse.mButton == left)) {
		button->mState = true;
	}
	else {
		button->mState = false;
	}
}