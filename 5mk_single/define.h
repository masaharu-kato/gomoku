#pragma once

#define BOARD_SIZE 10      // 盤面サイズ 10 * 10
#define STONE_SPACE 0      // 盤面にある石 なし
#define STONE_BLACK 1      // 盤面にある石 黒
#define STONE_WHITE 2      // 盤面にある石 白

#define __F19_COMPILE_WITH_BASE__

typedef struct{
	int x;
	int y;
} position;
