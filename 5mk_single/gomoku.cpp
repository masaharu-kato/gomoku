#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "define.h"
#include "gomoku_F19.h"

void inputPutPos(int board[][BOARD_SIZE], const int which, const int count, position *history, const int *group);
void changeTurn(int *which_turn);
int checkOutPos(int x, int y);
void gameInit(int board[][BOARD_SIZE], int *which_turn);
void boardInit(int board[][BOARD_SIZE]);
void boardPrint(int board[][BOARD_SIZE]);
int gameEndProcess(int board[][BOARD_SIZE]);
int lenCheck(int board[][BOARD_SIZE], int x, int y);
void randomStrategy(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);
void keyboardInput(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);

typedef void(*strategy)(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);

//=======================================================
// Group 15
//=======================================================

void strategy15(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	double tmp_d;
	int check, i, j;

	printf("Strategy 15\t");

	check = 0;
	while (check == 0)
	{
		//��U���
		if (count == 1){
			for (i = 0; i < BOARD_SIZE; i++){
				for (j = 0; j < BOARD_SIZE; j++){
					if (board[j][i] != STONE_SPACE){
						*pos_x = i + 1;
						*pos_y = j;
					}
				}
			}
		}

		//��U��Έ���
		else if (count == 0){
			*pos_x = 5;
			*pos_y = 5;
		}
		//��U��Γ���()
		else if (count == 2){
			int end_flag = 0;
			int kaisuu = 0;
			i = 4;
			j = 4;
			while (end_flag == 0){
				if ((board[j][i] == STONE_WHITE) && ((j + i) % 2 == 1)){
					if (i == 4 || i == 6){
						*pos_x = i;
						*pos_y = j + 1;
						end_flag = 1;
					}
					else {
						*pos_x = i + 1;
						*pos_y = j;
						end_flag = 1;
					}
				}
				else if ((board[j][i] == STONE_WHITE) && ((j + i) % 2 == 0)){
					if (i == 4){
						*pos_x = j;
						*pos_y = i + 2;
						end_flag = 1;
					}
					else {
						*pos_x = j;
						*pos_y = i - 2;
						end_flag = 1;
					}
				}
				else {
					if (kaisuu < 3){
						j++;
						kaisuu++;
					}
					else if (kaisuu == 3){
						i = 5;
						j = 4;
						kaisuu++;
					}
					else if (kaisuu < 6){
						j++;
						kaisuu++;
					}
					else if (kaisuu == 6){
						i = 6;
						j = 4;
						kaisuu++;
					}
					else if (kaisuu < 9){
						j++;
						kaisuu++;
					}
					else {
						*pos_x = 4;
						*pos_y = 6;
						end_flag = 1;
					}
				}
			}
		}


		// ����ȍ~�̐헪�i��U�j
		else if (count % 2 == 0){

			int x, y, max_p;
			int count2, plus, lp;
			int ans, tmp_x, tmp_y;
			int check_1[] = { 2, 2, 2, 0 };
			int check_2[] = { 2, 2, 0, 2, 0 };
			int check_3[] = { 1, 1, 1, 0 };
			int check_4[] = { 1, 1, 0, 1, 0 };

			max_p = 0;

			int BLACK_POINT[BOARD_SIZE][BOARD_SIZE];
			int WHITE_POINT[BOARD_SIZE][BOARD_SIZE];
			int BOARD_POINT[BOARD_SIZE][BOARD_SIZE];

			boardInit(BLACK_POINT);
			boardInit(WHITE_POINT);

			int dx[] = { 1, 0, -1, -1, -1, 0, 1, 1 };
			int dy[] = { -1, -1, -1, 0, 1, 1, 1, 0 };

			for (x = 0; x < BOARD_SIZE; x++){
				for (y = 0; y < BOARD_SIZE; y++){
					for (i = 0; i < 8; i++) {
						for (j = 1; j <= 4; j++) {
							if (board[y][x] == STONE_BLACK
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								BLACK_POINT[y][x] = -100;
								BLACK_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
							else if (board[y][x] == STONE_WHITE
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								WHITE_POINT[y][x] = -100;
								WHITE_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
						}
					}
				}
			}
			//������4�A��5�A�ɂ���
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (BLACK_POINT[y][x] >= 4 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}

								}
								count2 = 0;
							}
						}
					}
				}
			}
			// �����4�A���~�߂�
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (WHITE_POINT[y][x] >= 4 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}
								}
								count2 = 0;
							}
						}
					}
				}
			}
			//�����̗��[��3�A��4�A�ɂ���
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (BLACK_POINT[y][x] >= 3 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_3[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){

								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_4[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_4[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}
			// �����3�A���~�߂�(���[���J���Ă���ꍇ�̂�
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (WHITE_POINT[y][x] >= 3 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_1[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){
								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_2[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_2[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}


			int win, lose, win_count, win_x, win_y, place, max_win, max_i, take, draw, stop;
			place = 0;
			max_win = 0;
			int win_sum[100];
			int tmp_board[BOARD_SIZE][BOARD_SIZE];
			int retry_board[BOARD_SIZE][BOARD_SIZE];
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					tmp_board[i][j] = board[i][j];
					retry_board[i][j] = board[i][j];
				}
			}
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					BOARD_POINT[i][j] = BLACK_POINT[i][j] + WHITE_POINT[i][j];
					win_count = count;
					win = 0;
					lose = 0;
					draw = 0;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							retry_board[k][l] = board[k][l];
						}
					}
					if (BOARD_POINT[i][j] > 2){
						tmp_board[i][j] = STONE_BLACK;
						retry_board[i][j] = STONE_BLACK;
						//���[�v�񐔂����߂�
						while (win + lose + draw < 2200){
							win_count++;
							take = 0;
							for (int k = 0; k < BOARD_SIZE; k++) {
								for (int l = 0; l < BOARD_SIZE; l++) {
									if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_BLACK) {
										win++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_WHITE){
										lose++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
								}
								if (win_count == count){
									break;
								}
							}
							if (win_count % 2 == 0 && win_count != count){
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_BLACK;
										take = 1;
									}
									else if (win_count >= 100){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
							else if (win_count % 2 == 1 && win_count != count) {
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_WHITE;
										take = 1;
									}
									else if (win_count >= 99){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
						}
					}
					win_sum[place] = win;
					place++;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							tmp_board[k][l] = board[k][l];
						}
					}
				}
			}
			for (i = 0; i < 100; i++){
				if (max_win < win_sum[i]){
					max_win = win_sum[i];
					max_i = i;
				}
			}
			y = max_i / 10;
			x = max_i % 10;
			*pos_y = y;
			*pos_x = x;
		}


		//��U�̐헪
		else if (count % 2 == 1){

			int x, y, max_p;
			int count2, plus, lp;
			int ans, tmp_x, tmp_y;
			int check_1[] = { 2, 2, 2, 0 };
			int check_2[] = { 2, 2, 0, 2, 0 };
			int check_3[] = { 1, 1, 1, 0 };
			int check_4[] = { 1, 1, 0, 1, 0 };

			max_p = 0;

			int BLACK_POINT[BOARD_SIZE][BOARD_SIZE];
			int WHITE_POINT[BOARD_SIZE][BOARD_SIZE];
			int BOARD_POINT[BOARD_SIZE][BOARD_SIZE];

			boardInit(BLACK_POINT);
			boardInit(WHITE_POINT);

			int dx[] = { 1, 0, -1, -1, -1, 0, 1, 1 };
			int dy[] = { -1, -1, -1, 0, 1, 1, 1, 0 };

			for (x = 0; x < BOARD_SIZE; x++){
				for (y = 0; y < BOARD_SIZE; y++){
					for (i = 0; i < 8; i++) {
						for (j = 1; j <= 4; j++) {
							if (board[y][x] == STONE_BLACK
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								BLACK_POINT[y][x] = -100;
								BLACK_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
							else if (board[y][x] == STONE_WHITE
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								WHITE_POINT[y][x] = -100;
								WHITE_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
						}
					}
				}
			}

			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					//������4�A��5�A�ɂ���
					if (WHITE_POINT[y][x] >= 4 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}

								}
								count2 = 0;
							}
						}
					}
				}
			}
			// �����4�A���~�߂�
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (BLACK_POINT[y][x] >= 4 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}
								}
								count2 = 0;
							}
						}
					}
				}
			}
			//�����̋�3�A��4�A�ɂ���
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (WHITE_POINT[y][x] >= 3 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_1[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){
								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_2[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_2[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					// �����3�A���~�߂�(���[���J���Ă���ꍇ�̂�
					if (BLACK_POINT[y][x] >= 3 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_3[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){
								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_4[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_4[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}

			int win, lose, win_count, win_x, win_y, place, max_win, max_i, take, draw, stop;
			place = 0;
			max_win = 0;
			int win_sum[100];
			int tmp_board[BOARD_SIZE][BOARD_SIZE];
			int retry_board[BOARD_SIZE][BOARD_SIZE];
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					tmp_board[i][j] = board[i][j];
					retry_board[i][j] = board[i][j];
				}
			}
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					BOARD_POINT[i][j] = BLACK_POINT[i][j] + WHITE_POINT[i][j];
					win_count = count;
					win = 0;
					lose = 0;
					draw = 0;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							retry_board[k][l] = board[k][l];
						}
					}
					if (BOARD_POINT[i][j] > 2){
						tmp_board[i][j] = STONE_WHITE;
						retry_board[i][j] = STONE_WHITE;
						while (win + lose + draw < 2200){
							win_count++;
							take = 0;
							for (int k = 0; k < BOARD_SIZE; k++) {
								for (int l = 0; l < BOARD_SIZE; l++) {
									if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_WHITE) {
										win++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_BLACK){
										lose++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
								}
								if (win_count == count){
									break;
								}
							}
							if (win_count % 2 == 0 && win_count != count){
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_WHITE;
										take = 1;
									}
									else if (win_count >= 100){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
							else if (win_count % 2 == 1 && win_count != count) {
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_BLACK;
										take = 1;
									}
									else if (win_count >= 99){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
						}
					}
					win_sum[place] = win;
					place++;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							tmp_board[k][l] = board[k][l];
						}
					}
				}
			}
			for (i = 0; i < 100; i++){
				if (max_win < win_sum[i]){
					max_win = win_sum[i];
					max_i = i;
				}
			}
			y = max_i / 10;
			x = max_i % 10;
			*pos_y = y;
			*pos_x = x;
		}

		if (board[*pos_y][*pos_x] == STONE_SPACE){
			check = 1;
		}
	}
	return;
}


//=======================================================
// Group D18
//=======================================================
void strategyD18(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {

	printf("Strategy D18\t");
	int score[128][128];
	int i, j, k, max, snum;
	max = 0;
	k = 0;
	int setx[128], sety[128], spacex[128], spacey[128];
	int my, opponent, mycount, yourcount, space;
	int myx[128], myy[128], yourx[128], youry[128];


	int one = 1, two = 10, ytwo = 11, three = 10, yonren = 1000, win = 10000, lose = 20000, lose1 = 100000;

	for (i = 0; i < BOARD_SIZE; i++) { // �Ֆʂ̕]���l�̃��Z�b�g 
		for (j = 0; j < BOARD_SIZE; j++) {
			score[j][i] = 0;
		}
	}

	if (count % 2 == 0) {
		my = STONE_BLACK;
		opponent = STONE_WHITE;
	}
	else {
		my = STONE_WHITE;
		opponent = STONE_BLACK;
	}

	//���

	if (count == 0) {
		score[4][4] += 10;
		score[4][5] += 10;
		score[5][4] += 10;
		score[5][5] += 10;
	}



	else if (count == 2) {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == my) {
					score[i - 1][j - 1] += 10;
					score[i - 1][j + 1] += 10;
					score[i + 1][j - 1] += 10;
					score[i + 1][j + 1] += 10;
				}
				if (board[i][j] == opponent) {
					score[i - 2][j - 2] -= 10;
					score[i - 1][j] -= 10;
					score[i - 2][j + 2] -= 10;
					score[i][j - 1] -= 10;
					score[i][j + 1] -= 10;
					score[i + 2][j - 2] -= 10;
					score[i + 1][j] -= 10;
					score[i + 2][j + 2] -= 10;
				}
			}
		}
	}

	else if (count == 4) {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == my) {
					score[i - 1][j - 1] += 10;
					score[i - 2][j] += 10;
					score[i - 1][j + 1] += 10;
					score[i][j - 2] += 10;
					score[i][j + 2] += 10;
					score[i + 1][j - 1] += 10;
					score[i + 2][j] += 10;
					score[i + 1][j + 1] += 10;
				}
			}
		}
	}

	//���

	else if (count == 1) {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == opponent) {
					score[i - 1][j - 1] += 10;
					score[i - 1][j + 1] += 10;
					score[i + 1][j - 1] += 10;
					score[i + 1][j + 1] += 10;
				}
			}
		}
	}

	/*else*/ if (count > 2) {

		//�c��
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 5; k++) {
					if (board[i][j + k] == my) {
						mycount++;
					}
					if (board[i][j + k] == opponent) {
						yourcount++;
					}
					if (board[i][j + k] == STONE_SPACE) {
						spacex[space] = i;
						spacey[space] = j + k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}

		//�c��
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 4; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 5; k++) {
					if (board[i][j - k] == my) {
						mycount++;
					}
					if (board[i][j - k] == opponent) {
						yourcount++;
					}
					if (board[i][j - k] == STONE_SPACE) {
						spacex[space] = i;
						spacey[space] = j - k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}

		//���E
		for (j = 0; j < BOARD_SIZE; j++) {
			for (i = 0; i < BOARD_SIZE - 4; i++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = i; k < i + 5; k++) {
					if (board[k][j] == my) {
						mycount++;
					}
					if (board[k][j] == opponent) {
						yourcount++;
					}
					if (board[k][j] == STONE_SPACE) {
						spacex[space] = k;
						spacey[space] = j;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}

		//����
		for (j = 0; j < BOARD_SIZE; j++) {
			for (i = 4; i < BOARD_SIZE; i++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 5; k++) {
					if (board[i - k][j] == my) {
						mycount++;
					}
					if (board[i - k][j] == opponent) {
						yourcount++;
					}
					if (board[i - k][j] == STONE_SPACE) {
						spacex[space] = i - k;
						spacey[space] = j;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}

		//�΂߉E��
		for (j = 0; j < BOARD_SIZE - 4; j++) {
			for (i = 0; i < BOARD_SIZE - 4; i++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 5; k++) {
					if (board[i + k][j + k] == my) {
						mycount++;
					}
					if (board[i + k][j + k] == opponent) {
						yourcount++;
					}
					if (board[i + k][j + k] == STONE_SPACE) {
						spacex[space] = i + k;
						spacey[space] = j + k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}

		//�΂߉E��
		for (i = 0; i < BOARD_SIZE - 4; i++) {
			for (j = 4; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 5; k++) {
					if (board[i + k][j - k] == my) {
						mycount++;
					}
					if (board[i + k][j - k] == opponent) {
						yourcount++;
					}
					if (board[i + k][j - k] == STONE_SPACE) {
						spacex[space] = i + k;
						spacey[space] = j - k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}

		//�΂ߍ���
		for (i = 4; i < BOARD_SIZE - 4; i++) {
			for (j = 4; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 5; k++) {
					if (board[i - k][j - k] == my) {
						mycount++;
					}
					if (board[i - k][j - k] == opponent) {
						yourcount++;
					}
					if (board[i - k][j - k] == STONE_SPACE) {
						spacex[space] = i - k;
						spacey[space] = j - k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}

		//�΂ߍ���
		for (j = 0; j < BOARD_SIZE - 4; j++) {
			for (i = 4; i < BOARD_SIZE; i++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 5; k++) {
					if (board[i - k][j + k] == my) {
						mycount++;
					}
					if (board[i - k][j + k] == opponent) {
						yourcount++;
					}
					if (board[i - k][j + k] == STONE_SPACE) {
						spacex[space] = i - k;
						spacey[space] = j + k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (mycount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (mycount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += two;
					}
					else if (mycount == 1 && space == 4) {
						score[spacex[snum]][spacey[snum]] += one;
					}
					else if (yourcount == 4 && space == 1) {
						score[spacex[snum]][spacey[snum]] += lose1;
					}
					else if (yourcount == 3 && space == 2) {
						score[spacex[snum]][spacey[snum]] += three;
					}
					else if (yourcount == 2 && space == 3) {
						score[spacex[snum]][spacey[snum]] += ytwo;
					}
				}

			}
		}


		//6����o�[�W����
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE - 5; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i][j + k] == my) {
						myx[mycount] = i;
						myy[mycount] = j + k;
						mycount++;

					}
					if (board[i][j + k] == opponent) {
						yourx[yourcount] = i;
						youry[yourcount] = j + k;
						yourcount++;
					}
					if (board[i][j + k] == STONE_SPACE) {
						spacex[space] = i;
						spacey[space] = j + k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i][j + k - 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i][j + k - 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}

		//�c��
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 5; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i][j - k] == my) {
						myx[mycount] = i;
						myy[mycount] = j - k;
						mycount++;
					}
					if (board[i][j - k] == opponent) {
						yourx[yourcount] = i;
						youry[yourcount] = j - k;
						yourcount++;
					}
					if (board[i][j - k] == STONE_SPACE) {
						spacex[space] = i;
						spacey[space] = j - k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i][j - k + 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i][j - k + 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}

		//���E
		for (i = 0; i < BOARD_SIZE - 5; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i + k][j] == my) {
						mycount++;
					}
					if (board[i + k][j] == opponent) {
						yourcount++;
					}
					if (board[i + k][j] == STONE_SPACE) {
						spacex[space] = k;
						spacey[space] = j;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i + k - 1][j] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i + k - 1][j] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}

		//����
		for (i = 5; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i - k][j] == my) {
						mycount++;
					}
					if (board[i - k][j] == opponent) {
						yourcount++;
					}
					if (board[i - k][j] == STONE_SPACE) {
						spacex[space] = i - k;
						spacey[space] = j;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i - k + 1][j] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i - k + 1][j] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}

		//�΂߉E��
		for (i = 0; i < BOARD_SIZE - 5; i++) {
			for (j = 0; j < BOARD_SIZE - 5; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i + k][j + k] == my) {
						mycount++;
					}
					if (board[i + k][j + k] == opponent) {
						yourcount++;
					}
					if (board[i + k][j + k] == STONE_SPACE) {
						spacex[space] = i + k;
						spacey[space] = j + k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i + k - 1][j + k - 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i + k - 1][j + k - 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}

		//�΂߉E��
		for (i = 0; i < BOARD_SIZE - 5; i++) {
			for (j = 5; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i + k][j - k] == my) {
						mycount++;
					}
					if (board[i + k][j - k] == opponent) {
						yourcount++;
					}
					if (board[i + k][j - k] == STONE_SPACE) {
						spacex[space] = i + k;
						spacey[space] = j - k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i + k - 1][j - k + 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i + k - 1][j - k + 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}

		//�΂ߍ���
		for (i = 5; i < BOARD_SIZE; i++) {
			for (j = 5; j < BOARD_SIZE; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i - k][j - k] == my) {
						mycount++;
					}
					if (board[i - k][j - k] == opponent) {
						yourcount++;
					}
					if (board[i - k][j - k] == STONE_SPACE) {
						spacex[space] = i - k;
						spacey[space] = j - k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i - k + 1][j - k + 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i - k + 1][j - k + 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}

		//�΂ߍ���
		for (i = 5; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE - 5; j++) {
				mycount = 0;
				yourcount = 0;
				space = 0;

				for (k = 0; k < 6; k++) {
					if (board[i - k][j + k] == my) {
						mycount++;
					}
					if (board[i - k][j + k] == opponent) {
						yourcount++;
					}
					if (board[i - k][j + k] == STONE_SPACE) {
						spacex[space] = i - k;
						spacey[space] = j + k;
						space++;
					}
				}
				for (snum = 0; snum < space; snum++) {
					if (mycount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i - k + 1][j + k - 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += win;
					}
					else if (yourcount == 3 && space == 3 && board[i][j] == STONE_SPACE&&board[i - k + 1][j + k - 1] == STONE_SPACE) {
						score[spacex[snum]][spacey[snum]] += lose;
					}
				}

			}
		}
	}

	//����
	max = 0;
	k = 0;
	for (i = 0; i < BOARD_SIZE; i++) {
		printf("\n");
		for (j = 0; j < BOARD_SIZE; j++) {
			printf("%5d", score[i][j]);
			if (score[i][j] > max&&board[i][j] == STONE_SPACE) {
				max = score[i][j];
			}
		}
	}

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (score[i][j] == max&&board[i][j] == STONE_SPACE) {
				setx[k] = j;
				sety[k] = i;
				k++;
			}
		}
	}

	if (k == 1) {
		*pos_x = setx[0];
		*pos_y = sety[0];
	}

	if (k > 1) {
		srand((unsigned)time(NULL));

		k = rand() % k;

		*pos_x = setx[k];
		*pos_y = sety[k];
	}


	return;

}


//=======================================================
// main
//=======================================================
//=======================================================
// main
//=======================================================
int main(int argc, char **argv)
{
	// �ϐ��錾
	int board[BOARD_SIZE][BOARD_SIZE];
	position *history;
	int which_turn;
	int count;
	int group[2];

	if (argc != 3)
	{
		printf("gobang group_first group_second\n");
		printf("  group_first: ��U�̃O���[�v�ԍ�\n");
		printf("  group_first: ��U�̃O���[�v�ԍ�\n");
		exit(1);
	}
	else
	{
		group[0] = atoi(argv[1]);
		group[1] = atoi(argv[2]);
		printf("group_first (�w) = %d\n", group[0]);
		printf("group_second (�n) = %d\n", group[1]);

		fprintf(stderr, "First: %d, Second: %d\n", group[0], group[1]);
	}

	// ��������
	gameInit(board, &which_turn);
	history = (position*)malloc(sizeof(position) * BOARD_SIZE * BOARD_SIZE);
	boardPrint(board);
	srand((unsigned)time(NULL));

	//---- ���C�����[�v
	for (count = 0; count < BOARD_SIZE * BOARD_SIZE; count++){
		//--- ���͏���
		inputPutPos(board, which_turn, count, history, group);

		//--- �o�͏���
		boardPrint(board);

		//--- �I������
		if (gameEndProcess(board)) { break; }

		//--- ���Z����
		changeTurn(&which_turn);
	}

	if (count == BOARD_SIZE * BOARD_SIZE){
		fprintf(stderr, "���������ł�\n");
		printf("���������ł�\n");
	}

	free(history);

	return 0;
}

//-------------------------------------------------
// �u���ꏊ����
//-------------------------------------------------
void inputPutPos(int board[][BOARD_SIZE], const int which, const int count, position *history, const int *group)
{
	int pos_x, pos_y;
	char str[256];
	strategy strtgy[16] = {
		randomStrategy,	//	0
		strategyF19,	//	1
		strategy15,		//	2
		strategyD18,	//	3
		keyboardInput,	//	4
	};
	struct tm *date;
	time_t current_time;

	time_t stime, etime;
	int ptime;

	printf("%s", (which == 1) ? "�w" : "�n");
	printf("�̔Ԃł��B�ǂ��ɒu���܂���\n");

	while (1) {

		// ��̐��Ǝ�����\��
		current_time = time(NULL);
		date = localtime(&current_time);
		strftime(str, 255, "%Y, %B, %d, %A %p%I:%M:%S", date);
		printf("�� %d ��v�l�J�n�F Time: %s\n", count + 1, str);
		printf("> ");
		/*
		fprintf( stderr, "�� %d ��v�l�J�n�F Time: %s\n", count + 1, str );
		*/

		time(&stime);
		if (which == STONE_BLACK)
		{
			strtgy[group[0]](board, &pos_x, &pos_y, count, history);
		}
		else if (which == STONE_WHITE) {
			strtgy[group[1]](board, &pos_x, &pos_y, count, history);
		}
		else
		{
			printf("Invalid value of which in inputPutPos\n");
			printf("  which: %d\n", which);
			exit(1);
		}
		time(&etime);

		date = localtime(&current_time);
		strftime(str, 255, "%Y, %B, %d, %A %p%I:%M:%S", date);
		printf("�� %d ��v�l�I���F Time: %s\n", count + 1, str);
		/*
		fprintf( stderr, "�� %d ��v�l�I���F Time: %s\n", count + 1, str );
		*/

		ptime = difftime(etime, stime);

		if (ptime >= 61)
		{
			printf("���ԃI�[�o�[:%d\n", ptime);
			printf("%s�̏����ł��B\n", (which == 1) ? "�n" : "�w");
			/*
			fprintf( stderr, "���ԃI�[�o�[:%d\n", ptime);
			fprintf( stderr, "%s�̏����ł��B\n", (which == 1) ? "�n" : "�w" );
			*/
			exit(1);
		}
		else
		{
			printf("�v�l����:%d\n", ptime);
		}
		printf("%d %d\n", pos_x, pos_y);

		if (checkOutPos(pos_x, pos_y) && board[pos_y][pos_x] == STONE_SPACE) { break; }
		printf("�s���ȓ��͂ł�\n> ");
		printf("%s�̏����ł��B\n", (which == 1) ? "�n" : "�w");
		/*
		fprintf( stderr, "�s���ȓ��͂ł�\n> ");
		fprintf( stderr, "%s�̏����ł��B\n", (which == 1) ? "�n" : "�w" );
		*/
		exit(1);
	}
	board[pos_y][pos_x] = which;

	history[count].x = pos_x;
	history[count].y = pos_y;
}

//-------------------------------------------------
// ��Ԍ�㏈��
//-------------------------------------------------
void changeTurn(int *which_turn)
{
	*which_turn = (*which_turn == STONE_BLACK) ? STONE_WHITE : STONE_BLACK;
}

//-------------------------------------------------
// �͈͊O�`�F�b�N
//-------------------------------------------------
int checkOutPos(int x, int y)
{
	return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

//-------------------------------------------------
// �Q�[����񏉊���
//-------------------------------------------------
void gameInit(int board[][BOARD_SIZE], int *which_turn)
{
	boardInit(board);
	*which_turn = STONE_BLACK;
}

//-------------------------------------------------
// �Ֆʏ�����
//-------------------------------------------------
void boardInit(int board[][BOARD_SIZE])
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = STONE_SPACE;
		}
	}
}

//-------------------------------------------------
// �Ֆʏo��
//-------------------------------------------------
void boardPrint(int board[][BOARD_SIZE])
{
	int i, j;

	printf("  ");
	for (i = 0; i < BOARD_SIZE; i++) {
		printf("%d ", i);
	}
	puts("");
	for (i = 0; i < BOARD_SIZE; i++) {
		printf("%d ", i);
		for (j = 0; j < BOARD_SIZE; j++) {
			switch (board[i][j]) {
			case STONE_SPACE: printf("�E"); break;
			case STONE_BLACK: printf("�w"); break;
			case STONE_WHITE: printf("�n"); break;
			}
		}
		puts("");
	}
	puts("");
}

//-------------------------------------------------
// �Q�[���I������
//-------------------------------------------------
int gameEndProcess(int board[][BOARD_SIZE])
{
	int i, j, len_flag;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == STONE_SPACE) { continue; }
			if (lenCheck(board, j, i)) {
				printf("�ܘA�ӏ�:%d %d\n", j, i);
				printf("%s�̏����ł��B\n", (board[i][j] == STONE_BLACK) ? "�w" : "�n");
				/*
				fprintf( stderr, "%s�̏����ł��B\n", (board[i][j] == STONE_BLACK) ? "�w" : "�n");
				*/
				return 1;
			}
		}
	}
	return 0;
}


//-------------------------------------------------
// 5�A�m�F
//-------------------------------------------------
int lenCheck(int board[][BOARD_SIZE], int x, int y)
{
	int i, j, len_flag;
	int dx[] = { 0, 1, 1, -1 };
	int dy[] = { 1, 0, 1, 1 };

	for (i = 0; i < 4; i++) {
		for (j = 1, len_flag = 1; j <= 4; j++) {
			if (board[y][x] != board[y + j*dy[i]][x + j*dx[i]]
				|| y + j*dy[i] >= BOARD_SIZE
				|| x + j*dx[i] >= BOARD_SIZE
				|| y + j*dy[i] < 0
				|| x + j*dx[i] < 0)
			{
				len_flag = 0;
				break;
			}
		}
		if (len_flag == 1) { return 1; }
	}
	return 0;
}

//------------------------------------------------
// �����_���Ɏ��ł헪
//------------------------------------------------
void randomStrategy(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	double tmp_d;
	int check;

	printf("randomStrategy selects the position randomly\n");

	check = 0;
	while (check == 0)
	{
		tmp_d = (double)rand() / ((double)RAND_MAX + 1);
		*pos_x = (int)(tmp_d * 10.0);

		tmp_d = (double)rand() / ((double)RAND_MAX + 1);
		*pos_y = (int)(tmp_d * 10.0);

		if (board[*pos_y][*pos_x] == STONE_SPACE)
			check = 1;
	}

	return;
}



//------------------------------------------------
// �l�ԁi�܂��ł����́j
//------------------------------------------------
void keyboardInput(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	scanf("%d %d", pos_x, pos_y);

	return;
}