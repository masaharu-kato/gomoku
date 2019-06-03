#pragma once
#include "define.h"
#include "board.h"

namespace F19 {

void strategy(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);

}