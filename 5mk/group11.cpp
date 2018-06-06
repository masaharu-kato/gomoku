#include "groups.h"


//=======================================================
// Group 11
//=======================================================

void strategy11(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, int count, const position *history);
void str11_b_fitcheck(const int board[][BOARD_SIZE], int count, int *pos_x, int *pos_y);
void str11_b_fitInit(int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE]);
void str11_b_fitfunc(const int board[][BOARD_SIZE], int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE], int atack[8], int defense[8], int which[8], int x, int xi, int xj, int a, int b, int c);
int str11_b_fitcal1(int number, int block);
int str11_b_fitcal2(int number, int block);

void strategy11(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, int count, const position *history)
{

	if (count == 0){
		*pos_x = BOARD_SIZE / 2;
		*pos_y = BOARD_SIZE / 2;
	}
	else if (count == 1){
		*pos_x = history[0].x + ((history[0].x < BOARD_SIZE / 2) ? 1 : -1);
		*pos_y = history[0].y + ((history[0].y < BOARD_SIZE / 2) ? 1 : -1);
	}
	else{
		str11_b_fitcheck(board, count, &*pos_x, &*pos_y);
	}
}

void str11_b_fitcheck(const int board[][BOARD_SIZE], int count, int *pos_x, int *pos_y)
{
	int i, j, x, xi, xj, a, b, c, block, which[8];
	int atack[8], defense[8];
	int b_fita[BOARD_SIZE][BOARD_SIZE], b_fitd[BOARD_SIZE][BOARD_SIZE];
	int num1 = (count % 2 == 0) ? 30 : 20;
	int num2 = (count % 2 == 0) ? 20 : 30;

	position elite;
	position elite_fit;
	elite.x = 0, elite.y = 0, elite_fit.x = 0, elite_fit.y = 0;
	str11_b_fitInit(b_fita, b_fitd);

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < 8; j++){
			atack[j] = 0;
			defense[j] = 0;
			which[j] = 0;
		}
		for (j = 0; j < BOARD_SIZE; j++) {
			x = 0, xi = i, xj = j, a = 0, b = 1, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			x = 1, xi = BOARD_SIZE - i - 1, xj = BOARD_SIZE - j - 1, a = 0, b = -1, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			x = 2, xi = j, xj = i, a = 1, b = 0, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			x = 3, xi = BOARD_SIZE - j - 1, xj = BOARD_SIZE - i - 1, a = -1, b = 0, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
		}
	}
	for (i = 0; i < BOARD_SIZE - 4; i++){
		for (j = 0; j < 8; j++){
			atack[j] = 0;
			defense[j] = 0;
			which[j] = 0;
		}
		for (j = 0; j < BOARD_SIZE; j++){
			if (j + i < BOARD_SIZE){
				x = 4, xi = j, xj = j + i, a = 1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 5, xi = BOARD_SIZE - 1 - j, xj = j + i, a = -1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 6, xi = j, xj = BOARD_SIZE - 1 - (j + i), a = 1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 7, xi = BOARD_SIZE - 1 - j, xj = BOARD_SIZE - 1 - (j + i), a = -1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			}
		}
	}
	for (i = 1; i < BOARD_SIZE - 4; i++){
		for (j = 0; j < 8; j++){
			atack[j] = 0;
			defense[j] = 0;
			which[j] = 0;
		}
		for (j = 0; j < BOARD_SIZE; j++){
			if (j + i < BOARD_SIZE){
				x = 4, xi = j + i, xj = j, a = 1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 5, xi = BOARD_SIZE - 1 - (j + i), xj = j, a = -1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 6, xi = j + i, xj = BOARD_SIZE - 1 - j, a = 1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 7, xi = BOARD_SIZE - 1 - (j + i), xj = BOARD_SIZE - 1 - j, a = -1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			}
		}
	}
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] != STONE_SPACE){
				b_fita[i][j] = 0;
				b_fitd[i][j] = 0;
			}
		}
	}

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (b_fita[i][j] > num1){
				if (elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
			}
			if (b_fitd[i][j] > num2){
				elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				i = BOARD_SIZE, j = BOARD_SIZE;
				break;
			}
			else if (elite_fit.x <= b_fita[i][j] || elite_fit.y <= b_fitd[i][j]){
				if (b_fita[i][j] > 12 && elite_fit.x < b_fita[i][j] || b_fitd[i][j] > 16 && elite_fit.y < b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] > 10 && elite_fit.y < 15){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] > 10 && elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] < 8 && b_fitd[i][j] > 8 && elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] >= elite_fit.y && elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					if (0 < i && i < BOARD_SIZE - 1 && 0 < j && j < BOARD_SIZE - 1){
						elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
					}
				}
				else if (b_fita[i][j] > elite_fit.x && b_fitd[i][j] > elite_fit.y){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
			}
		}
	}

	if (i == BOARD_SIZE && j == BOARD_SIZE && elite_fit.x == 0 && elite_fit.y == 0){
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
					break;
				}
			}
		}
	}
	*pos_x = elite.y, *pos_y = elite.x;
}
void str11_b_fitfunc(const int board[][BOARD_SIZE], int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE], int atack[8], int defense[8], int which[8], int x, int xi, int xj, int a, int b, int c)
{
	if (board[xi][xj] == STONE_BLACK){
		if (defense[x] > 0){
			defense[x] = 1;
			if (atack[x] > 0 && which[x] == STONE_WHITE){
				atack[x] = 0;
			}
		}
		atack[x]++;
		which[x] = STONE_BLACK;
	}
	else if (board[xi][xj] == STONE_WHITE){
		if (atack[x] > 0){
			atack[x] = 1;
			if (defense[x] > 0 && which[x] == STONE_BLACK){
				defense[x] = 0;
			}
		}
		defense[x]++;
		which[x] = STONE_WHITE;
	}
	else{
		if (which[x] == STONE_BLACK){
			b_fitd[xi][xj] += 0;
			if (c == 0 && atack[x] < 4 || c == BOARD_SIZE - 1 && atack[x] < 4){
				b_fita[xi][xj] += 0;
			}
			else if (board[xi + a][xj + b] == STONE_WHITE && atack[x] < 4){
				b_fita[xi][xj] += 0;
			}
			else if (defense[x] == 0){
				if (board[xi + a][xj + b] == STONE_BLACK && board[xi + 2 * a][xj + 2 * b] != STONE_WHITE){
					b_fita[xi][xj] += 2 * str11_b_fitcal1(atack[x], 0);
				}
				else{
					b_fita[xi][xj] += str11_b_fitcal1(atack[x], 0);
				}
				if (0 < c && c < BOARD_SIZE - 1){
					b_fita[xi + a][xj + b] += str11_b_fitcal2(atack[x], 0);
				}
			}
			else{
				b_fita[xi][xj] += str11_b_fitcal1(atack[x], 1);
				if (0 < c && c < BOARD_SIZE - 1){
					b_fita[xi + a][xj + b] += str11_b_fitcal2(atack[x], 1);
				}
			}
		}
		else if (which[x] == STONE_WHITE){
			b_fita[xi][xj] += 0;
			if (c == 0 && defense[x] < 4 || c == BOARD_SIZE - 1 && defense[x] < 4){
				b_fitd[xi][xj] += 0;
			}
			else if (board[xi + a][xj + b] == STONE_BLACK && defense[x] < 4){
				b_fitd[xi][xj] += 0;
			}
			else if (atack[x] == 0){
				if (board[xi + a][xj + b] == STONE_WHITE && board[xi + 2 * a][xj + 2 * b] != STONE_BLACK){
					b_fitd[xi][xj] += 2 * str11_b_fitcal1(defense[x], 0);
				}
				else{
					b_fita[xi][xj] += str11_b_fitcal1(atack[x], 0);
				}
				b_fitd[xi][xj] += str11_b_fitcal1(defense[x], 0);
				if (0 < c && c < BOARD_SIZE - 1){
					b_fitd[xi + a][xj + b] += str11_b_fitcal2(defense[x], 0);
				}
			}
			else{
				b_fitd[xi][xj] += str11_b_fitcal1(defense[x], 1);
				if (0 < c && c < BOARD_SIZE - 1){
					b_fitd[xi + a][xj + b] += str11_b_fitcal2(defense[x], 1);
				}
			}
		}
		atack[x] = 0, defense[x] = 0, which[x] = 0;
	}
}

int str11_b_fitcal1(int number, int block)
{
	int n = number - 2 * block, two = 1, i;
	for (i = 0; i <= n; i++){
		two = two * 2;
	}

	return 2 * (1 - block) + two * (number - 1) + 20 * (number / 4) * block;
}

int str11_b_fitcal2(int number, int block)
{
	int n = number - 2 * block - 1, two = 1, i;
	for (i = 0; i <= n; i++){
		two = two * 2;
	}

	return 1 * (1 - block) + two * (number - 1) + 2 * (number / 3) + (1 / number - 1);
}

void str11_b_fitInit(int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE])
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			b_fita[i][j] = STONE_SPACE;
			b_fitd[i][j] = STONE_SPACE;
		}
	}
}


