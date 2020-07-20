/*********************************************
*** Designer : 藤川
*** Date     : 2020.6.30
*** Purpose  : ボタン関数の実装
**********************************************/

#include "DxLib.h"
#include "Controller.h"
#include "MouseState.h"

/*******************************************************
*** Function Name : setButton (M20)
*** Designer      : 藤川
*** Date          : 2020.7.17
*** Function      : 引数でとった座標と色，画像ハンドルを引数でとった
					ボタン変数へのポインタに格納し，mouse変数の値を
					使ってボタンの有効無効を決める．getStateを1に
					した場合左クリックをもとに判定．
*** Return        : void  --戻り値なし
********************************************************/
void setButton(int x1, int y1, int x2, int y2, int imageHandle, unsigned int color, Mouse_t mouse, Button_t* button, int getState) {
	button->mX1 = x1;	// ボタンの右端座標を格納
	button->mY1 = y1;	// ボタンの上端座標を格納
	button->mX2 = x2;	// ボタンの下端座標を格納
	button->mY2 = y2;	// ボタンの左端座標を格納

	// 引数のcolorがNULLだったとき
	if(color == NULL) {
		button->mImageHandle = imageHandle;	// 引数のイメージハンドルを格納
		button->mColor = NULL;	// 色をNULLにする
	}
	//引数のcolorがNULLでないとき
	else {
		button->mImageHandle = NULL;	// 引数のイメージハンドルをNULLにする
		button->mColor = color;	// 色をcolorに設定
	}

	// 引数のgetStateが0(FALSE)のとき
	if (getState == 0) {
		// マウスがボタン上にあり，マウスの左ボタンが押されていたら
		if (mouse.mX > x1 && mouse.mX < x2 && mouse.mY > y1 && mouse.mY < y2 && mouse.mButton == left) {
			button->mState = true;	// ボタンを有効にする
		}
		// マウスがボタン上にあり，マウスの左ボタンが押されていなかったら
		else {
			button->mState = false;	// ボタンを無効にする
		}
	}
	// 引数のgetStateが1(TRUE)のとき
	else {
		// マウスがボタン上にあり，マウスの左クリックされていたら
		if (mouse.mX > x1 && mouse.mX < x2 && mouse.mY > y1 && mouse.mY < y2 && mouse.mState == leftClick) {
			button->mState = true;	// ボタンを有効にする
		}
		// マウスがボタン上にあり，マウスの左クリックされていなかったら 
		else {
			button->mState = false;	// ボタンを無効にする
		}
	}
}