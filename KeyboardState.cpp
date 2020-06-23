#include "DxLib.h"
#include "KeyboardState.h"

void GetKeyBoardState(int* key) {
	char tmpKey[256];			// 現在のキーの入力状態を格納する変数
	GetHitKeyStateAll(tmpKey);	// すべてのキーの入力状態を得る

	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) {	// i番目のキーに対応するキーが押されてれば
			key[i]++;			// キーボード状態格納変数のi番目を加算
		}
		else {					// 押されていなければ
			key[i] = 0;			// キーボード状態格納変数のi番目は0にする
		}
	}
}

char CheckHitKey(void) {
	char keyState[256], key = -1;
	GetHitKeyStateAll(keyState);
	int i;

	//read 0-9
	for (i = 2; i < 12; i++) {
		if (keyState[i]) {
			key = (i - 1) % 10 + 48;
		}
	}

	//read a-z
	if (keyState[16]) key = 113;
	if (keyState[17]) key = 119;
	if (keyState[18]) key = 101;
	if (keyState[19]) key = 114;
	if (keyState[20]) key = 116;
	if (keyState[21]) key = 121;
	if (keyState[22]) key = 117;
	if (keyState[23]) key = 105;
	if (keyState[24]) key = 111;
	if (keyState[25]) key = 112;

	if (keyState[30]) key = 97;
	if (keyState[31]) key = 115;
	if (keyState[32]) key = 100;
	if (keyState[33]) key = 102;
	if (keyState[34]) key = 103;
	if (keyState[35]) key = 104;
	if (keyState[36]) key = 106;
	if (keyState[37]) key = 107;
	if (keyState[38]) key = 108;

	if (keyState[44]) key = 122;
	if (keyState[45]) key = 120;
	if (keyState[46]) key = 99;
	if (keyState[47]) key = 118;
	if (keyState[48]) key = 98;
	if (keyState[49]) key = 110;
	if (keyState[50]) key = 109;

	//check shift key
	if (keyState[42] && key > 64 || keyState[54] && key > 64) {
		key -= 32;
	}

	//blank
	if (keyState[57]) key = 32;
	//back space
	if (keyState[14]) key = 8;

	return key;
}