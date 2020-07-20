/*********************************************
*** Designer : 藤川
*** Date     : 2020.6.30
*** Purpose  : ボタン構造体Button_tのメンバと各関数の
			   プロトタイプ宣言(M20)
**********************************************/

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "MouseState.h"

// ボタン構造体
typedef struct BUTTON {
	int	mX1;	// 左端座標
	int	mY1;	// 上端座標
	int	mX2;	// 右端座標
	int	mY2;	// 下端座標
	int	mImageHandle;	// ボタン画像
	unsigned int  mColor;	// マスの色		
	bool mState;	// ボタン判定: true = 押されている, false = 押されていない
} Button_t;

// M20:ボタン関数
void setButton(int x1, int y1, int x2, int y2, int imageHandle, unsigned int color, Mouse_t mouse, Button_t* button, int getState = 0);

#endif /* CONTROLLER_H_ */