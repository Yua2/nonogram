#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "MouseState.h"
// テキスト構造体
typedef struct TEXT {
	int   mX;			// x座標
	int   mY;			// y座標
	char* mSentence;	// 表示文
} Text_t;
// テキストボックス構造体
typedef struct TEXT_BOX {
	int   mX1;				// 左端座標
	int   mY1;				// 上端座標
	int   mX2;				// 右端座標
	int   mY2;				// 下端座標
	char* inputSentence;	// 入力された文
} TextBox_t;
// ボタン構造体
typedef struct BUTTON {
	int			  mX1;			// 左端座標
	int			  mY1;			// 上端座標
	int			  mX2;			// 右端座標
	int			  mY2;			// 下端座標
	int			  mImageHandle;	// ボタン画像
	unsigned int  mColor;		// マスの色		
	bool          mState;		// ボタン判定: true = 押されている, false = 押されていない
} Button_t;
// ボタン関数
void setButton(int, int, int, int, int, unsigned int, Mouse_t, Button_t*);
#endif /* CONTROLLER_H_ */