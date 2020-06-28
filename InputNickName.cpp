#include"InputNickName.h"
#include "DxLib.h"
#include "SceneMgr.h"
int InitializeInputNickName(InputNickName_t* mInputNickName) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "input");
	return InputNickNameScr;
}
int UpdateInputNickName(InputNickName_t* mInputNickName) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "input");
	WaitKey();
	return MenuScr;
}
void FinalizeInputNickName(InputNickName_t* mInputNickName) {

}