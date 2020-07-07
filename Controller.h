#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "MouseState.h"

// テキスト構造体
struct Text_t {
	int  mX;			// x座標
	int  mY;			// y座標
	char mSentence[256];	// 表示文
};

// テキストボックス構造体
struct TextBox_t {
	int  mX1;				// 左端座標
	int  mY1;				// 上端座標
	int  mX2;				// 右端座標
	int  mY2;				// 下端座標
	char inputSentence[256];	// 入力された文
};

// ボタン構造体
struct Button_t {
	int			  mX1;			// 左端座標
	int			  mY1;			// 上端座標
	int			  mX2;			// 右端座標
	int			  mY2;			// 下端座標
	int			  mImageHandle;	// ボタン画像
	unsigned int  mColor;		// マスの色		
	bool          mState;		// ボタン判定: true = 押されている, false = 押されていない
};

// ボタン関数
void setButton(int x1, int y1, int x2, int y2, int imageHandle, unsigned int color, Mouse_t mouse, Button_t* button);

#endif /* CONTROLLER_H_ */
