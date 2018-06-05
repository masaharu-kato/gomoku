#pragma once
#include "base.h"

void inputPutPos(int board[][BOARD_SIZE], const int which, const int count, position *history, const int *group );
void changeTurn(int *which_turn);
int checkOutPos(int x, int y);
void gameInit(int board[][BOARD_SIZE], int *which_turn);
void boardInit(int board[][BOARD_SIZE]);
void boardPrint(int board[][BOARD_SIZE]);
int gameEndProcess(int board[][BOARD_SIZE]);
int lenCheck(int board[][BOARD_SIZE], int x, int y);
void randomStrategy( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );
void keyboardInput( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );

typedef void (*strategy)( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );
