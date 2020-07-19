#ifndef SCENEMGR_H_
#define SCENEMGR_H_

#include "InputNickName.h"
#include "Menu.h"
#include "Selection.h"
#include "Game.h"
#include "MakePuzzle.h"
#include "MouseState.h"
#include "Information.h"

typedef enum {
	InputNickNameScr,
	MenuScr,
	SelectionScr,
	GameScr,
	MakePuzzleScr,
	None
}EScene;

typedef struct SCR_MGR {
	InputNickName_t mInputNickName;
	Menu_t          mMenu;
	Selection_t	    mSelection;
	Game_t          mGame;
	MakePuzzle_t    mMakePuzzle;
} ScrMgr_t;



int InitializeScene(int, ScrMgr_t*, Puzzle_t*);

int UpdateScene(int, ScrMgr_t*, Mouse_t*, int*, Puzzle_t*);

void FinalizeScene(int, ScrMgr_t*, Puzzle_t*);

#endif /* SCENEMGR_H_ */