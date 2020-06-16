#ifndef KEYBOARD_STATE_H_
#define KEYBOARD_STATE_H_

// M5:キーボード状態の取得
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

#endif /* KEYBOARD_STATE_H_ */
