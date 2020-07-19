
#ifndef INPUTNICKNAME_H_
#define INPUTNICKNAME_H_


#include <iostream>
#include <string>
#include "MouseState.h"
#include "KeyboardState.h"


//テスト用
#define SCR_SIZE_X 1080
#define SCR_SIZE_Y 720


#define FONT_SIZE 16
#define NAME_MAX 20

enum InputNickNamePhase { standard, textBox, check, used };

typedef struct InputNickName_t {
	enum InputNickNamePhase phase = standard;
	std::string nickName;
}InputNickName_t;

//本番用
int InitializeInputNickName(InputNickName_t*);
int UpdateInputNickName(InputNickName_t*, Mouse_t*);
void FinalizeInputNickName(InputNickName_t*);


//int click(Mouse_t*, int, int, int, int);
int checkNickName(std::string);

/*テスト用
int test_Initialize(InputNickName_t *);
int test_Update(InputNickName_t *, Mouse_t *);
void test_Finelize(InputNickName_t *);
*/

#endif /*INPUTNICKNAME_H_*/