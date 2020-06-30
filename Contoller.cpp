#include "DxLib.h"
#include "Controller.h"
#include "MouseState.h"

// ボタン関数
Button_t setButton(int x1, int y1, int x2, int y2, int imageHandle, unsigned int color, Mouse_t mouse) {
	Button_t ret;

	ret.mX1 = x1;
	ret.mY1 = y1;
	ret.mX2 = x2;
	ret.mY2 = y2;

	if (color != NULL) {
		ret.mImageHandle = imageHandle;
		ret.mColor = NULL;
	}
	else {
		ret.mImageHandle = NULL;
		ret.mColor = color;
	}

	if (mouse.mX > x1 && mouse.mX < x2 && mouse.mY > y1 && mouse.mY < y2 && mouse.mButton == left)
		ret.mState = true;
	else
		ret.mState = false;

	return ret;
}