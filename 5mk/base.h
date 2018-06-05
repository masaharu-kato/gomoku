#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10      // 盤面サイズ 10 * 10
#define STONE_SPACE 0      // 盤面にある石 なし
#define STONE_BLACK 1      // 盤面にある石 黒
#define STONE_WHITE 2      // 盤面にある石 白

typedef struct{
	int x;
	int y;
} position;
