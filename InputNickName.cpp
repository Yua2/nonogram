#include"InputNickName.h"
#include "DxLib.h"
#include "SceneMgr.h"
int InitializeInputNickName(InputNickName_t* mInputNickName) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "input");
	return InputNickNameScr;
}
int UpdateInputNickName(InputNickName_t* mInputNickName,Mouse_t *mouse) {
	DrawFormatString(500, 300, GetColor(0, 0, 0), "input");
	ScreenFlip();
	if (mouse->mButton == left) {
		mouse->waitRelease = 1;
		return MenuScr;
	}
	return InputNickNameScr;
}
void FinalizeInputNickName(InputNickName_t* mInputNickName) {

}