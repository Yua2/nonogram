#ifndef MOUSE_STATE_H_
#define MOUSE_STATE_H_

typedef struct MOUSE {
	int mButton;
	int mWheel;
	int mX;
	int mY;
} Mouse_t;

// M4:マウス処理
void GetMouseState(Mouse_t* mouse);

#endif /* MOUSESTATE_H_ */
