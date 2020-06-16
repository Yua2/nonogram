#ifndef CONTROLLER_H_
#define CONTROLLER_H_

typedef struct TEXT {
	int mX;
	int mY;
	char[] mSentence;
} Text_t;

typedef struct TEXT_BOX {
	int mX1;
	int mY1;
	int mX2;
	int mY2;
	char[] inputSentence;
} TextBox_t;

typedef struct BUTTON {
	int mX;
	int mY;
	int mImageHandle;
} Button_t;

typedef struct PUZZLE_LIST {

};

#endif /* CONTROLLER_H_ */
