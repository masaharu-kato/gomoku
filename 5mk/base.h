#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10      // î’ñ ÉTÉCÉY 10 * 10
#define STONE_SPACE 0      // î’ñ Ç…Ç†ÇÈêŒ Ç»Çµ
#define STONE_BLACK 1      // î’ñ Ç…Ç†ÇÈêŒ çï
#define STONE_WHITE 2      // î’ñ Ç…Ç†ÇÈêŒ îí

typedef struct{
	int x;
	int y;
} position;
