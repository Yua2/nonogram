#include "Information.h"
#include <stdio.h>
#include <stdlib.h>
/******************************************************
*** Function Name: readPuzzle
*** Designer: 陳
*** Date: 2020.7.6
*** Function：csvファイル内のパズル情報をPuzzle_t構造体に格納する
*** Return: Escene
*******************************************************/
void readPuzzle(Puzzle_t *pz,char filename[10]){
    FILE* fp;
    fopen_s(&fp,filename,"r");
    if(fp == NULL)
        printf("failed\n");
    int i,j;
    fscanf_s(fp,"%d",&pz->puzzleId);
    fscanf_s(fp,"%s",&pz->puzzleTitle,256);
    fscanf_s(fp,"%d %d",&pz->x_size,&pz->y_size);
    for(j = 0;j < pz->y_size;j++)
        for(i = 0;i < pz->x_size;i++)
            fscanf_s(fp,"%d",&pz->puzzleData[j][i]);
    fscanf_s(fp,"%d",&pz->puzzleMakerId);
    for(i = 0;i < 10;i++)
        fscanf_s(fp,"%d %d",&pz->ranking[i].playerId,&pz->ranking[i].cleartime);
    fclose(fp);           
}