#pragma once
typedef struct {
	int i;
}InputNickName_t;
int InitializeInputNickName(InputNickName_t*);
int UpdateInputNickName(InputNickName_t*);
void FinalizeInputNickName(InputNickName_t*);