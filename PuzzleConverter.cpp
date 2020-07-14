/***************************************
 *** Designer : 高山
 *** Date     : 2020.7.10
 *** Purpose  : C2 パズル作成処理部
*****************************************/

#include <stdlib.h>
#include "DxLib.h"
#include "PuzzleConverter.h"

// プロトタイプ宣言
int FindColor(int r, int g, int b);

int Monokuro(int *pz_data, int width, int height) {
    if(pz_data == NULL) { return -1; }
    return 0;
}

/********************************************************
 *** Function Name : pzAdjust
 *** Designer      : 高山
 *** Date          : 2020.7.10
 *** Function      : 読み込んだ画像をパズルのデータに変換する
 *** Return        : int 
*********************************************************/
int pzAdjust(char *fileName, int pzl_w, int pzl_h, Puzzle_t *ret) {
    if(fileName == NULL) { return -1; }
    if(ret == NULL) { return -1; }
    Puzzle_t p = { 0 };
    *ret = p;
    ret->x_size = pzl_w, ret->y_size = pzl_h;

    int handle = LoadSoftImage(fileName);
    // 画像のサイズ
    int width, height; 
    GetSoftImageSize(handle, &width, &height);
    int block_w = width / pzl_w;
    int block_h = height / pzl_h;

    for(int i = 0;i < pzl_h;i++) {
        for(int j = 0;j < pzl_w;j++) {

            int red = 0, green = 0, blue = 0;
            for(int y = 0;y < block_h;y++) {
                for(int x = 0;x < block_w;x++) {
                    int r, g, b, a;
                    GetPixelSoftImage(handle, block_w * j + x, block_h * i + y, &r, &g, &b, &a);
                    red += r;
                    green += g;
                    blue += b;
                }
            }
            red /= block_h * block_w;
            green /= block_h * block_w;
            blue /= block_h * block_w;
            ret->puzzleData[i][j] = FindColor(red, green, blue);
        }
    }
    return DeleteSoftImage(handle);
}

/********************************************************
 *** Function Name : FindColor
 *** Designer      : 高山
 *** Date          : 2020.7.10
 *** Function      : パズルで使える色の中で、入力された色に最も近い色の番号を返す
 *** Return        : int 
*********************************************************/
int FindColor(int r, int g, int b) {
    int distance = (r - 255) * (r - 255) + (g - 255) * (g - 255) + (b - 255) * (b - 255);
    int colorid = 0;

    int tmp =  r *  r +  g *  g + b * b;
    if(tmp < distance) { distance = tmp, colorid = 101; }
    tmp =  r *  r +  g *  g + (b - 255) * (b - 255);
    if(tmp < distance) { distance = tmp, colorid = 102; }
    tmp =  r *  r + (g - 128) * (g - 128) +  b *  b;
    if(tmp < distance) { distance = tmp, colorid = 103; }
    tmp = (r - 128) *  r +  g *  g +  b *  b;
    if(tmp < distance) { distance = tmp, colorid = 104; }
    tmp = (r - 255) * (r - 255) + (g - 20) * (g - 20) + (b - 147) * (b - 147);
    if(tmp < distance) { distance = tmp, colorid = 105; }
    tmp = (r - 139) * (r - 139) + (g - 69) * (g - 69) + (b - 19) * (b - 19);
    if(tmp < distance) { distance = tmp, colorid = 106; }
    tmp = (r - 255) * (r - 255) + (g - 215) * (g - 215) +  b *  b;
    if(tmp < distance) { distance = tmp, colorid = 107; }
    tmp = (r - 75) * (r - 75) +  g *  g + (b - 130) * (b - 130);
    if(tmp < distance) { distance = tmp, colorid = 108; }
    tmp = (r - 220) * (r - 220) + (g - 220) * (g - 220) + (b - 220) * (b - 220);
    if(tmp < distance) { distance = tmp, colorid = 1; }
    tmp =  r *  r + (g - 255) * (g - 255) + (b - 255) * (b - 255);
    if(tmp < distance) { distance = tmp, colorid = 2; }
    tmp =  r *  r + (g - 255) * (g - 255) + (b - 127) * (b - 127);
    if(tmp < distance) { distance = tmp, colorid = 3; }
    tmp = (r - 255) * (r - 255) +  g *  g +  b *  b;
    if(tmp < distance) { distance = tmp, colorid = 4; }
    tmp = (r - 255) * (r - 255) +  g *  g + (b - 255) * (b - 255);
    if(tmp < distance) { distance = tmp, colorid = 5; }
    tmp = (r - 255) * (r - 255) + (g - 165) * (g - 165) +  b *  b;
    if(tmp < distance) { distance = tmp, colorid = 6; }
    tmp = (r - 255) * (r - 255) + (g - 250) * (g - 250) + (b - 205) * (b - 205);
    if(tmp < distance) { distance = tmp, colorid = 7; }
    tmp = (r - 147) * (r - 147) + (g - 112) * (g - 112) + (b - 219) * (b - 219);
    if(tmp < distance) { distance = tmp, colorid = 8; }

    return colorid;
}
