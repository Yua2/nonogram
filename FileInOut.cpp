#include "Information.h"
#include "Selection.h"
#include <stdio.h>
#include <stdlib.h>
/******************************************************
*** Function Name: readPuzzle
*** Designer: 齔
*** Date: 2020.7.6
*** Function｣ｺcsv･ﾕ･｡･､･・ﾚ､ﾎ･ﾑ･ｺ･・驤i､ﾟﾈ｡､ﾃ､ﾆPuzzle_t・ﾔ・螟ﾋｸ{､ｹ､・
*** Return: ､ﾊ､ｷ
*******************************************************/
void readPuzzle(Puzzle_t* pz, char filename[64]) {
	FILE* fp;
	fopen_s(&fp, filename, "r");
	if (fp == NULL)
		printf("failed\n");
	int i, j;
	fscanf_s(fp, "%d", &pz->puzzleId);
	fscanf_s(fp, "%s", &pz->puzzleTitle, 17);
	fscanf_s(fp, "%d %d", &pz->x_size, &pz->y_size);
	for (j = 0; j < pz->y_size; j++)
		for (i = 0; i < pz->x_size; i++) {
			fscanf_s(fp, "%d", &pz->puzzleData[i][j]);
		}
	fscanf_s(fp, "%s", &pz->puzzleMakerId, 17);
	for (i = 0; i < 10; i++)
		fscanf_s(fp, "%s %d %d", &pz->ranking[i].playerId, 17, &pz->ranking[i].cleartime, &pz->ranking[i].flag);
	fclose(fp);
}
/******************************************************
*** Function Name: savePuzzle
*** Designer: 齔
*** Date: 2020.7.10
*** Function｣ｺﾐﾂ､ｷ､ｯﾗﾃ､ｿ･ﾑ･ｺ･・ﾕ･｡･､･・ﾋｳｦ､ｹ､・
*** Return: ､ﾊ､ｷ
*******************************************************/
void savePuzzle(Puzzle_t* pz) {
	FILE* fp;
	int i, j, x = pz->x_size, y = pz->y_size, newId;
	char filename[64];
	sprintf_s(filename, 64, "PuzzleInfo/%d%d/puzzlenum.csv", pz->x_size, pz->y_size);
	fopen_s(&fp, filename, "r+");
	fscanf_s(fp, "%d", &newId);
	fclose(fp);
	newId++;
	fopen_s(&fp, filename, "w");
	fprintf(fp, "%d", newId);
	fclose(fp);
	pz->puzzleId = newId;

	sprintf_s(filename, 64, "PuzzleInfo/%d%d/Puzzle%d.csv", pz->x_size, pz->y_size, pz->puzzleId);
	fopen_s(&fp, filename, "w");
	fprintf(fp, "%d\n", pz->puzzleId);
	fprintf(fp, "%s\n", pz->puzzleTitle);
	fprintf(fp, "%d %d\n", pz->x_size, pz->y_size);
	for (j = 0; j < y; j++) {
		for (i = 0; i < x; i++)
			fprintf(fp, "%d ", pz->puzzleData[i][j]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "%s\n", pz->puzzleMakerId);
	for (i = 0; i < 10; i++)
		fprintf(fp, "%s %d %d\n", pz->ranking[i].playerId, pz->ranking[i].cleartime, pz->ranking[i].flag);
	fclose(fp);
	sprintf_s(filename, 64, "PuzzleInfo/%d%d/simpleInformation.csv", pz->x_size, pz->y_size);
	fopen_s(&fp, filename, "a");
	fprintf(fp, "\n%d %s %s %s %d %d", pz->puzzleId, pz->puzzleTitle, pz->puzzleMakerId, pz->ranking[0].playerId, pz->ranking[0].cleartime, pz->ranking[0].flag);
	fclose(fp);

}
void updatesimpleRanking(Puzzle_t* pz, char* name, int time) {
	FILE* fp;
	char filename[64];
	int puzzleNum, i;
	SimplePuzzle_t* spt;
	//get number of pazzle from puzzlenum.csv
	sprintf_s(filename, 64, "PuzzleInfo/%d%d/puzzlenum.csv", pz->x_size, pz->y_size);
	fopen_s(&fp, filename, "r");
	fscanf_s(fp, "%d", &puzzleNum);
	//read data from simpleInfomation.csv
	spt = (SimplePuzzle_t*)malloc(sizeof(SimplePuzzle_t) * puzzleNum);
	sprintf_s(filename, 64, "PuzzleInfo/%d%d/simpleInformation.csv", pz->x_size, pz->y_size);
	fopen_s(&fp, filename, "r");
	for (i = 0; i < puzzleNum; i++)
		fscanf_s(fp, "%d %s %s %s %d %d", &spt[i].puzzleId, &spt[i].puzzleTitle, 17, &spt[i].puzzleMakerName, 17, &spt[i].fastestId, 17, &spt[i].fastestTime, &spt[i].flag);
	//update firstest player
	sprintf_s(spt[pz->puzzleId - 1].fastestId, 17, name);
	spt[pz->puzzleId - 1].fastestTime = time;
	spt[pz->puzzleId - 1].flag = 1;
	fclose(fp);
	fopen_s(&fp, filename, "w");
	//write data to simpleInfomation.csv
	for (i = 0; i < puzzleNum; i++)
		fprintf_s(fp, "%d %s %s %s %d %d\n", spt[i].puzzleId, spt[i].puzzleTitle, spt[i].puzzleMakerName, spt[i].fastestId, spt[i].fastestTime, spt[i].flag);
	fclose(fp);
}
void updateRanking(Puzzle_t* pz, char playerId[17], int time) {
	int i, j, rank = 1;
	char filename[64];
	FILE* fp;

	for (i = 0; pz->ranking[i].cleartime <= time; i++) {
		rank++;
		if (pz->ranking[i].flag == 0) {
			pz->ranking[i].flag = 1;
			sprintf_s(pz->ranking[i].playerId, 17, "%s", playerId);
			pz->ranking[i].cleartime = time;
			rank = i + 1;
			i = 10;
			break;
		}
		if (i == 9) {
			i++;
			break;
		}

	}

	if (i <= 9) {
		for (j = 9; j > i; j--) {
			sprintf_s(pz->ranking[j].playerId, 17, "%s", pz->ranking[j - 1].playerId);
			pz->ranking[j].cleartime = pz->ranking[j - 1].cleartime;
			pz->ranking[j].flag = pz->ranking[j - 1].flag;
		}
		sprintf_s(pz->ranking[i].playerId, 17, "%s", playerId);
		pz->ranking[i].cleartime = time;
		pz->ranking[i].flag = 1;
	}
	sprintf_s(filename, 64, "PuzzleInfo/%d%d/Puzzle%d.csv", pz->x_size, pz->y_size, pz->puzzleId);
	fopen_s(&fp, filename, "w");
	fprintf(fp, "%d\n", pz->puzzleId);
	fprintf(fp, "%s\n", pz->puzzleTitle);
	fprintf(fp, "%d %d\n", pz->x_size, pz->y_size);
	for (j = 0; j < pz->y_size; j++) {
		for (i = 0; i < pz->x_size; i++)
			fprintf(fp, "%d ", pz->puzzleData[i][j]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "%s\n", pz->puzzleMakerId);
	for (i = 0; i < 10; i++)
		fprintf(fp, "%s %d %d\n", pz->ranking[i].playerId, pz->ranking[i].cleartime, pz->ranking[i].flag);
	fclose(fp);
	if (rank == 1) {
		updatesimpleRanking(pz, playerId, time);
	}
}