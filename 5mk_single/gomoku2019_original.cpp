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
// Group 5
//=======================================================

int str5_tomecheck(int board[][BOARD_SIZE], int x, int y, int s, int t);
int str5_l_check(int board[][BOARD_SIZE], int x, int y, int s, int t);
int str5_hyouka2(int board[][BOARD_SIZE], int x, int y);
int str5_hyouka1(int board[][BOARD_SIZE], int x, int y);
int str5_kuu(int x, int y, int len[][BOARD_SIZE]);
int str5_sslencheck(int board[][BOARD_SIZE], int x, int y);
int str5_yonlenCheck(int board[][BOARD_SIZE], int x, int y);
int str5_golenCheck(int board[][BOARD_SIZE], int x, int y);

void strategy5(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{

	int cboard[BOARD_SIZE][BOARD_SIZE], bscore = 0, wscore = 0, bb = 0, ww = 0;
	int a = 0, b = 0, c = 0, d = 0, i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, score = 0, subscore = 0, ss;
	clock_t start, end;

	printf("Strategy 5\t");

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			cboard[i][j] = board[i][j];
		}
	}

	if (count == 0){
		*pos_x = 5;
		*pos_y = 5;
		return;
	}
	else if (count == 1){
		if (history[0].x <= 4){
			*pos_x = history[0].x + 1;
		}
		else{
			*pos_x = history[0].x - 1;
		}
		if (history[0].y <= 4){
			*pos_y = history[0].y + 1;
		}
		else{
			*pos_y = history[0].y - 1;
		}
		return;
	}
	else if (count == 2){
		if ((history[0].x == history[1].x && abs(history[0].y - history[1].y) == 1) || (history[0].y == history[1].y && abs(history[0].x - history[1].x) == 1)){
			if (history[1].x == 4 || history[1].y == 4){
				*pos_x = 4;
				*pos_y = 4;
			}
			else if (history[1].x == 6 || history[1].y == 6){
				*pos_x = 6;
				*pos_y = 6;
			}
		}
		else if (abs(history[0].x - history[1].x) == 1 && abs(history[0].y - history[1].y) == 1){
			if (history[1].x == 4){
				if (history[1].y == 4){
					*pos_x = 4;
					*pos_y = 6;
				}
				else if (history[1].y == 6){
					*pos_x = 4;
					*pos_y = 4;
				}
			}
			else if (history[1].x == 6){
				if (history[1].y == 4){
					*pos_x = 6;
					*pos_y = 6;
				}
				else if (history[1].y == 6){
					*pos_x = 6;
					*pos_y = 4;
				}
			}
		}
		return;
	}


	if (count % 2 == 0){
		score = -99999999;
	}
	else{
		score = 99999999;
	}

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (board[i][j] == STONE_SPACE && str5_kuu(j, i, cboard)){
				if (count % 2 == 0){
					cboard[i][j] = STONE_BLACK;
					ss = str5_hyouka1(cboard, j, i);
					if (ss >= 9999999){
						*pos_x = j;
						*pos_y = i;
						return;
					}
					ww = 0;
					for (a = 0; a < BOARD_SIZE; a++){
						for (b = 0; b < BOARD_SIZE; b++){
							if (cboard[a][b] == STONE_SPACE && str5_kuu(b, a, cboard)){
								cboard[a][b] = STONE_WHITE;
								if (str5_hyouka2(cboard, b, a) <= -8888888){
									*pos_x = b;
									*pos_y = a;
									return;
								}
								if ((wscore = str5_hyouka2(cboard, b, a)) <= ww){
									ww = wscore;
								}
								cboard[a][b] = STONE_SPACE;
							}
						}
					}
					if ((subscore = ss + ww) > score){
						score = subscore;
						*pos_x = j;
						*pos_y = i;
					}
					else if (subscore == score && (*pos_x - history[count - 1].x)*(*pos_x - history[count - 1].x) + (*pos_y - history[count - 1].y)*(*pos_y - history[count - 1].y) > (j - history[count - 1].x)*(j - history[count - 1].x) + (i - history[count - 1].y)*(i - history[count - 1].y)){
						*pos_x = j;
						*pos_y = i;
					}
					cboard[i][j] = STONE_SPACE;
				}
				else{
					cboard[i][j] = STONE_WHITE;
					ss = str5_hyouka2(cboard, j, i);
					if (ss <= -9999999){
						*pos_x = j;
						*pos_y = i;
						return;
					}
					for (a = 0; a < BOARD_SIZE; a++){
						for (b = 0; b < BOARD_SIZE; b++){
							if (cboard[a][b] == STONE_SPACE && str5_kuu(b, a, cboard)){
								cboard[a][b] = STONE_BLACK;
								if (str5_hyouka1(cboard, b, a) >= 8888888){
									*pos_x = b;
									*pos_y = a;
									return;
								}
								if ((bscore = str5_hyouka1(cboard, b, a)) >= bb){
									bb = bscore;
								}
								cboard[a][b] = STONE_SPACE;
							}
						}
					}
					if ((subscore = ss + bb) < score){
						score = subscore;
						*pos_x = j;
						*pos_y = i;
					}
					else if (subscore == score && (*pos_x - history[count - 1].x)*(*pos_x - history[count - 1].x) + (*pos_y - history[count - 1].y)*(*pos_y - history[count - 1].y) > (j - history[count - 1].x)*(j - history[count - 1].x) + (i - history[count - 1].y)*(i - history[count - 1].y)){
						*pos_x = j;
						*pos_y = i;
					}
					cboard[i][j] = STONE_SPACE;
				}
			}
		}
	}
	return;
}

//5連かどうかチェック
int str5_golenCheck(int board[][BOARD_SIZE], int x, int y){
	int i, j, co = 1, kuu = 0;
	int dx[] = { 0, 0, 1, -1, 1, -1, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 1, -1, 1, -1 };

	for (i = 0; i < 8; i++){
		j = 1;
		while (1){
			if (0 > y + j*dy[i] && 0 > x + j*dx[i] && y + j*dy[i] >= BOARD_SIZE && x + j*dx[i] >= BOARD_SIZE){
				if (i % 2 == 1){
					if (co >= 5){
						return 1;
					}
					else{
						co = 1;
					}
				}
				break;
			}
			if (board[y][x] == board[y + j*dy[i]][x + j*dx[i]]){
				co++;
			}
			else{
				if (i % 2 == 1){
					if (co >= 5){
						return 1;
					}
					else{
						co = 1;
					}
				}
				break;
			}
			j++;
		}
	}
	return 0;
}


//フリー4連のチェック（飛び四は対象外)
int str5_yonlenCheck(int board[][BOARD_SIZE], int x, int y){
	int i, j, co = 1, kuu = 0;
	int dx[] = { 0, 0, 1, -1, 1, -1, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 1, -1, 1, -1 };

	for (i = 0; i < 8; i++){
		j = 1;
		while (1){
			if (0 > y + j*dy[i] && 0 > x + j*dx[i] && y + j*dy[i] >= BOARD_SIZE && x + j*dx[i] >= BOARD_SIZE){
				co = 1;
				break;
			}
			if (board[y][x] == board[y + j*dy[i]][x + j*dx[i]]){
				co++;
			}
			else if (board[y + j*dy[i]][x + j*dx[i]] == 0){
				if (i % 2 == 1){
					if (co >= 4){
						return 1;
					}
					else{
						co = 1;
					}
				}
				break;
			}
			else{
				if (i % 2 == 0){
					i++;
					co = 1;
					break;
				}
				else{
					co = 1;
					break;
				}
			}
			j++;
		}
	}
	return 0;
}

//三三以上になるかチェック
int str5_sslencheck(int board[][BOARD_SIZE], int x, int y){
	int i, j, co, tui = 0, situi = 0, mk[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }, kuu = 0, ok = 0;
	int dx[] = { 0, 0, 1, -1, 1, -1, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 1, -1, 1, -1 };

	for (i = 0; i < 8; i++){
		j = 1;
		while (1){
			if (0 > y + j*dy[i] || 0 > x + j*dx[i] || y + j*dy[i] >= BOARD_SIZE || x + j*dx[i] >= BOARD_SIZE){
				if (i % 2 == 1){
					if (co >= 4){
						situi++;
					}
				}
				co = 1;
				break;
			}
			if (board[y][x] == board[y + j*dy[i]][x + j*dx[i]]){
				co++;
			}
			else if (board[y + j*dy[i]][x + j*dx[i]] == 0){
				if (i % 2 == 0){
					break;
				}
				else{
					if (co >= 4){
						situi++;
					}
					if (co >= 3){
						tui++;
						mk[i - 1] = 1;
						mk[i] = 1;
						co = 1;
						break;
					}
					else{
						co = 1;
						break;
					}
				}
				break;
			}
			else{
				if (co >= 4){
					tui++;
				}
				co = 1;
				break;
			}
			j++;
		}
	}
	if (situi >= 2){
		return 1;
	}
	else if (tui >= 2){
		for (i = 0; i < 8; i++){
			if (mk[i] == 1){
				j = 1;
				kuu = 0;
				while (1){
					if (0 > y + j*dy[i] || 0 > x + j*dx[i] || y + j*dy[i] >= BOARD_SIZE || x + j*dx[i] >= BOARD_SIZE || (board[y + j*dy[i]][x + j*dx[i]] != board[y][x] && board[y + j*dy[i]][x + j*dx[i]] != 0)){
						if (kuu >= 2){
							ok++;
						}
						break;
					}
					else if (board[y + j*dy[i]][x + j*dx[i]] == 0){
						kuu++;
					}
					j++;
				}
			}
		}
		if (ok >= 2){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}
//仮に打つ場所の周りに石があるかどうか
int str5_kuu(int x, int y, int len[][BOARD_SIZE]){
	int i, j;
	for (i = y - 1; i <= y + 1; i++){
		for (j = x - 1; j <= x + 1; j++){
			if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE){
				if (len[i][j] != 0){
					return 1;
				}
			}
		}
	}
	return 0;
}

//黒の手の評価
int str5_hyouka1(int board[][BOARD_SIZE], int x, int y){
	int i, j, co = 0, l = 0;
	int b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, w1 = 0, w2 = 0, w3 = 0, w4 = 0, w5 = 0, w6 = 0;

	//打った手で5連、フリーの4連(結構弱い)ができるかどうか
	if (str5_golenCheck(board, x, y)){
		return 9999999;
	}
	else if (str5_yonlenCheck(board, x, y)){
		return 8888888;
	}
	else if (str5_sslencheck(board, x, y)){
		return 7777777;
	}

	for (i = y - 1; i <= y + 1; i++){
		for (j = x - 1; j <= x + 1; j++){
			if (i < 0 || j < 0 || i >= BOARD_SIZE || j >= BOARD_SIZE){
				break;
			}
			if (board[y][x] == STONE_WHITE){
				co = str5_tomecheck(board, x, y, j, i);
			}
			else if (board[y][x] == STONE_BLACK){
				l = str5_l_check(board, x, y, j, i);
			}
			else{
				l = 1;
			}
			if (l >= 5){
				b5++;
			}
			else if (l == 4){
				b4++;
			}
			else if (l == 3){
				b3++;
			}
			else if (l == 2){
				b2++;
			}
			else if (l == 1){
				b1++;
			}
			else if (co == 1){
				w1 += co;
			}
			else if (co == 2){
				w2 += co;
			}
			else if (co == 3){
				w3 += co;
			}
			else if (co == 4){
				w4 += co;
			}
			else if (co <= 5){
				w5 += co;
			}
		}
	}

	return b1 * 1 + b2 * 100 + b3 * 150 + b4 * 500 + b5 * 1000 + w1 * 10 + w2 * 1000 + w3 * 10000 + w4 * 15000 + w5 * 20000;
}

//白の手の評価
int str5_hyouka2(int board[][BOARD_SIZE], int x, int y){
	int i, j, co = 0, l = 0;
	int b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, w1 = 0, w2 = 0, w3 = 0, w4 = 0, w5 = 0;

	//打った手で5連、フリーの4連ができるかどうか
	if (str5_golenCheck(board, x, y)){
		return -9999999;
	}
	else if (str5_yonlenCheck(board, x, y)){
		return -8888888;
	}
	else if (str5_sslencheck(board, x, y)){
		return -7777777;
	}

	for (i = y - 1; i <= y + 1; i++){
		for (j = x - 1; j <= x + 1; j++){
			if (i < 0 || j < 0 || i >= BOARD_SIZE || j >= BOARD_SIZE){
				break;
			}
			if (board[i][j] == STONE_BLACK){
				co = str5_tomecheck(board, x, y, j, i);
			}
			else if (board[i][j] == STONE_WHITE){
				l = str5_l_check(board, x, y, j, i);
			}
			else{
				l = 1;
			}

			if (co >= 5){
				b5 += co;
			}
			else if (co == 4){
				b4 += co;
			}
			else if (co == 3){
				b3 += co;
			}
			else if (co == 2){
				b2 += co;
			}
			else if (co == 1){
				b1 += co;
			}
			else if (l == 1){
				w1++;
			}
			else if (l == 2){
				w2++;
			}
			else if (l == 3){
				w3++;
			}
			else if (l == 4){
				w4++;
			}
			else if (l <= 5){
				w5++;
			}
		}
	}

	return -(b1 * 10 + b2 * 1000 + b3 * 10000 + b4 * 15000 + b5 * 20000 + w1 * 1 + w2 * 100 + w3 * 150 + w4 * 500 + w5 * 1000);
}

//自分の連鎖が作れているかどうか
int str5_l_check(int board[][BOARD_SIZE], int x, int y, int s, int t){
	int i, j = 1, k, co = 2, stop[] = { 0, 0, 0, 0, 0, 0, 0, 0 }, kuu = 1;
	int dx[] = { 0, 0, 1, -1, 1, -1, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 1, -1, 1, -1 };

	if (board[t][s] == 0){
		return 0;
	}

	if (t == y - 1){
		if (s == x - 1){
			while (t + j*dy[5] >= 0 && s + j*dx[5] >= 0){
				if (board[t][s] == board[t + j*dy[5]][s + j*dx[5]]){
					co++;
				}
				else if (board[t + j*dy[5]][s + j*dx[5]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x){
			while (t + j*dy[1] >= 0){
				if (board[t][s] == board[t + j*dy[1]][s + j*dx[1]]){
					co++;
				}
				else if (board[t + j*dy[1]][s + j*dx[1]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x + 1){
			while (t + j*dy[7] >= 0 && s + j*dx[7] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[7]][s + j*dx[7]]){
					co++;
				}
				else if (board[t + j*dy[7]][s + j*dx[7]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
	}
	else if (t == y){
		if (s == x - 1){
			while (s + j*dx[3] >= 0){
				if (board[t][s] == board[t + j*dy[3]][s + j*dx[3]]){
					co++;
				}
				else if (board[t + j*dy[3]][s + j*dx[3]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x + 1){
			while (s + j*dx[2] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[2]][s + j*dx[2]]){
					co++;
				}
				else if (board[t + j*dy[2]][s + j*dx[2]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
	}
	else if (t == y + 1){
		if (s == x - 1){
			while (t + j*dy[6] < BOARD_SIZE && s + j*dx[6] >= 0){
				if (board[t][s] == board[t + j*dy[6]][s + j*dx[6]]){
					co++;
				}
				else if (board[t + j*dy[6]][s + j*dx[6]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x){
			while (t + j*dy[0] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[0]][s + j*dx[0]]){
					co++;
				}
				else if (board[t + j*dy[0]][s + j*dx[0]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x + 1){
			while (j*dy[4] < BOARD_SIZE && s + j*dx[4] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[4]][s + j*dx[4]]){
					co++;
				}
				else if (board[t + j*dy[4]][s + j*dx[4]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
	}

	return co;
}


//相手の連鎖を止められているかどうか
int str5_tomecheck(int board[][BOARD_SIZE], int x, int y, int s, int t){
	int i, j = 1, k, co = 1, stop[] = { 0, 0, 0, 0, 0, 0, 0, 0 }, kuu = 0;
	int dx[] = { 0, 0, 1, -1, 1, -1, -1, 1 };
	int dy[] = { 1, -1, 0, 0, 1, -1, 1, -1 };

	if (board[t][s] == 0){
		return 0;
	}

	if (t == y - 1){
		if (s == x - 1){
			while (t + j*dy[5] >= 0 && s + j*dx[5] >= 0){
				if (board[t][s] == board[t + j*dy[5]][s + j*dx[5]]){
					co++;
				}
				else if (board[t + j*dy[5]][s + j*dx[5]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x){
			while (t + j*dy[1] >= 0){
				if (board[t][s] == board[t + j*dy[1]][s + j*dx[1]]){
					co++;
				}
				else if (board[t + j*dy[1]][s + j*dx[1]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x + 1){
			while (t + j*dy[7] >= 0 && s + j*dx[7] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[7]][s + j*dx[7]]){
					co++;
				}
				else if (board[t + j*dy[7]][s + j*dx[7]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
	}
	else if (t == y){
		if (s == x - 1){
			while (s + j*dx[3] >= 0){
				if (board[t][s] == board[t + j*dy[3]][s + j*dx[3]]){
					co++;
				}
				else if (board[t + j*dy[3]][s + j*dx[3]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x + 1){
			while (s + j*dx[2] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[2]][s + j*dx[2]]){
					co++;
				}
				else if (board[t + j*dy[2]][s + j*dx[2]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
	}
	else if (t == y + 1){
		if (s == x - 1){
			while (t + j*dy[6] < BOARD_SIZE && s + j*dx[6] >= 0){
				if (board[t][s] == board[t + j*dy[6]][s + j*dx[6]]){
					co++;
				}
				else if (board[t + j*dy[6]][s + j*dx[6]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x){
			while (t + j*dy[0] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[0]][s + j*dx[0]]){
					co++;
				}
				else if (board[t + j*dy[0]][s + j*dx[0]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
		else if (s == x + 1){
			while (j*dy[4] < BOARD_SIZE && s + j*dx[4] < BOARD_SIZE){
				if (board[t][s] == board[t + j*dy[4]][s + j*dx[4]]){
					co++;
				}
				else if (board[t + j*dy[4]][s + j*dx[4]] == 0){
					if (kuu == 0){
						kuu++;
					}
					else{
						return co + 1;
					}
				}
				else{
					return co;
				}
				j++;
			}
		}
	}

	return co;
}

//=======================================================
// Group 6
//=======================================================

void strategy6(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int i, j, k, cnt = 0, _cnt;
	int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int point[BOARD_SIZE][BOARD_SIZE] = { 0 };

	printf("Strategy 6\t");

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) cnt++;
		}
	}
	if (cnt == BOARD_SIZE * BOARD_SIZE) {
		*pos_x = BOARD_SIZE / 2 - (rand() % 2);
		*pos_y = BOARD_SIZE / 2 - (rand() % 2);
	}

	else {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE) {
					int a = 0, b = 0;
					if (i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) point[i][j] -= 100;
					if (!(i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1)) {
						_cnt = 0;
						for (k = 0; k < 8; k++) {
							if (board[i + dx[k]][j + dy[k]] != STONE_SPACE) _cnt++;
						}
						if (_cnt >= 6) point[i][j] -= 100;
					}
					for (k = 0; k < 8; k++) {
						if (i + dx[k] >= 0 && j + dy[k] >= 0 && i + dx[k] < BOARD_SIZE && j + dy[k] < BOARD_SIZE) { //1個隣だけ見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK) {
									point[i][j] += 10;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 100;
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 1;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE) {
									point[i][j] += 20;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 50;
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 1;
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE) {
									point[i][j] += 10;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 100;
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 1;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK) {
									point[i][j] += 20;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 50;
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 1;
									}
								}
							}
						}
						if ((i + 2 * dx[k] >= 0 && j + 2 * dy[k] >= 0) && (i + 2 * dx[k] < BOARD_SIZE && j + 2 * dy[k] < BOARD_SIZE)) { //2個隣まで見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 5;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 10;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10000;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 30;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10;
											}
										}
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 80;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 50;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 200;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 1300;
											}
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] -= 40;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 5;
											if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 50;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) {
											point[i][j] += 50;
											a++;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 100;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) {
											point[i][j] += 100;
											b++;
										}
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 5;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 10;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10000;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 30;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10;
											}
										}
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 80;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 50;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 200;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 1300;
											}
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] -= 40;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 5;
											if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 50;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) {
											point[i][j] += 50;
											a++;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 100;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) {
											point[i][j] += 100;
											b++;
										}
									}
								}
							}
						}
						if ((i + 3 * dx[k] >= 0 && j + 3 * dy[k] >= 0) && (i + 3 * dx[k] < BOARD_SIZE && j + 3 * dy[k] < BOARD_SIZE)) { //3個隣まで見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									point[i][j] += 100;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 500; //反対側に1個あった時
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 200;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									point[i][j] += 130;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 700; //反対側に1個あった時
									}
								}
								int l, cnt1 = 0, cnt2 = 0;
								for (l = 1; l <= 3; l++) {
									if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
										cnt1++;
										cnt2 -= 2;
									}
									else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
										cnt1 -= 2;
										cnt2++;
									}
								}
								if (cnt1 >= 2) {
									a++;
									point[i][j] += a * 120;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK)
										{
											point[i][j] += 20; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
											}
										}
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 10; //反対側に1個あった時
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 100;
									}
								}
								if (cnt2 >= 2) {
									b++;
									b += a;
									point[i][j] += b * 140;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 5; //反対側に1個あった時
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 200; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
										}
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 30;
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									point[i][j] += 100;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 500; //反対側に1個あった時
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 200;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									point[i][j] += 130;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 700; //反対側に1個あった時
									}
								}
								int l, cnt1 = 0, cnt2 = 0;
								for (l = 1; l <= 3; l++) {
									if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
										cnt1++;
										cnt2 -= 2;
									}
									else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
										cnt1 -= 2;
										cnt2++;
									}
								}
								if (cnt1 >= 2) {
									a++;
									point[i][j] += a * 120;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 20; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
											}
										}
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //反対側に1個あった時
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 100;
									}
								}
								if (cnt2 >= 2) {
									b++;
									b += a;
									point[i][j] += b * 140;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 5; //反対側に1個あった時
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 200; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
										}
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 30;
									}
								}
							}
						}
						if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) { //4個隣まで見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) {
										point[i][j] += 10000;
										if ((i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) && (i + 4 * dx[k] == 0 || i + 4 * dx[k] == BOARD_SIZE - 1 || j + 4 * dy[k] == 0 || j + 4 * dy[k] == BOARD_SIZE - 1)) point[i][j] -= 10000;
									}
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) point[i][j] += 2000;
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 700;
								}
								if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 70;
								else {
									int l, cnt1 = 0, cnt2 = 0;
									for (l = 1; l <= 4; l++) {
										if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
											cnt1++;
											cnt2 -= 2;
										}
										else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
											cnt1 -= 2;
											cnt2++;
										}
									}
									if (cnt1 >= 3) {
										point[i][j] += 90;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
												point[i][j] += 20; //反対側に1個あった時
												if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
													if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
												}
											}
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //反対側に1個あった時
										}
									}
									if (cnt2 >= 3) {
										point[i][j] += 150;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 20; //反対側に1個あった時
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 20; //反対側に1個あった時
										}
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) {
										point[i][j] += 10000;
										if ((i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) && (i + 4 * dx[k] == 0 || i + 4 * dx[k] == BOARD_SIZE - 1 || j + 4 * dy[k] == 0 || j + 4 * dy[k] == BOARD_SIZE - 1)) point[i][j] -= 10000;
									}
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) point[i][j] += 2000;
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 700;
								}
								if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 70;
								else {
									int l, cnt1 = 0, cnt2 = 0;
									for (l = 1; l <= 4; l++) {
										if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
											cnt1++;
											cnt2 -= 2;
										}
										else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
											cnt1 -= 2;
											cnt2++;
										}
									}
									if (cnt1 >= 3) {
										point[i][j] += 90;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
												point[i][j] += 20; //反対側に1個あった時
												if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
													if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
												}
											}
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //反対側に1個あった時
										}
									}
									if (cnt2 >= 3) {
										point[i][j] += 150;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 20; //反対側に1個あった時
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 20; //反対側に1個あった時
										}
									}
								}
							}
						}
					}
				}
			}
		}
		int p_max = -10000;
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (p_max < point[i][j] && board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) p_max = point[i][j];
			}
		}

		int b_flag = 0;
		while (1) {
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					if (point[i][j] == p_max && board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) {
						*pos_x = j;
						*pos_y = i;
						if (rand() % 3 == 0) b_flag = 1;
					}
					if (b_flag == 1) break;
				}
				if (b_flag == 1) break;
			}
			if (b_flag == 1) break;
		}
	}

	return;
}


//=======================================================
// Group 12
//=======================================================

void strategy12(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);
void str12_thinking(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int self_stone, const int rival_stone, const int count, const position *history);
int str12_eval_firstphase(int x, int y, const int board[][BOARD_SIZE]);
int str12_eval_secondphase(int x, int y, const int board[][BOARD_SIZE]);

//-------------------------------------------------
// 味方アルゴリズム起動
//-------------------------------------------------
void strategy12(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int check;
	int phase = 0;

	printf("Strategy 12\t");

	check = 0;
	while (check == 0)
	{
		//先手後手チェック
		if (count % 2 == 0){
			phase = STONE_BLACK;
		}
		else if (count % 2 == 1){
			phase = STONE_WHITE;
		}

		if (phase == STONE_BLACK){
			str12_thinking(board, pos_x, pos_y, STONE_BLACK, STONE_WHITE, count, history);
		}
		else{
			str12_thinking(board, pos_x, pos_y, STONE_WHITE, STONE_BLACK, count, history);
		}

		if (board[*pos_y][*pos_x] == STONE_SPACE)
			check = 1;
	}

	return;
}

//決定アルゴリズム
void str12_thinking(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int self_stone, const int rival_stone, const int count, const position *history){

	int eval[BOARD_SIZE][BOARD_SIZE] = { 0 };
	int max_eval = -1;
	double max_pos_x, max_pos_y;

	//一手目・三手目判定	
	//一手目だったら中のアルゴリズムを起動
	if (count == 0){
		*pos_x = 5;
		*pos_y = 5;
		return;
		//三手目だったら中のアルゴリズムを起動
	}
	else if (count == 2){

		//浦月・花月判定

		//printf("%d\n", history[count - 2].x + history[count - 2].y);
		//printf("%d\n", history[count - 1].x + history[count - 1].y);

		if ((history[count - 2].x + history[count - 2].y - history[count - 1].x + history[count - 1].y) % 2 == 1){
			//printf("Kagetsu");
			//花月
			*pos_x = history[count - 1].x - (history[count - 2].y - history[count - 1].y);
			*pos_y = history[count - 1].y - (history[count - 2].x - history[count - 1].x);

			return;
		}
		else if ((history[count - 2].x + history[count - 2].y - history[count - 1].x + history[count - 1].y) % 2 == 0){
			//printf("Fugetsu\n");
			//浦月
			*pos_x = history[count - 2].x - (history[count - 2].x - history[count - 1].x);
			*pos_y = history[count - 2].y + (history[count - 2].y - history[count - 1].y);

			return;
		}

		//一、三手目でなければ以下のプログラムを起動
	}
	else{
		//盤面上のすべての空きスペースの評価を検索//
		for (int i = 0; i < BOARD_SIZE; i++){
			for (int j = 0; j < BOARD_SIZE; j++){
				if (board[i][j] == STONE_SPACE){
					if (self_stone == STONE_BLACK){
						eval[i][j] += str12_eval_firstphase(i, j, board);
					}
					else{
						eval[i][j] += str12_eval_secondphase(i, j, board);
					}
				}
				else{
					eval[i][j] = -1;
				}
			}
		}

		//最大評価値の検出
		for (int i = 0; i < BOARD_SIZE; i++){
			for (int j = 0; j < BOARD_SIZE; j++){
				if (max_eval < eval[i][j]){
					max_eval = eval[i][j];
					//printf("new max_eval => %d%d %d\n", i, j, max_eval);
					max_pos_x = j;
					max_pos_y = i;
				}
				else if (max_eval == eval[i][j]){
					if ((max_pos_x + max_pos_y) - (10 - i + j) < 0){
						max_eval = eval[i][j];
						//printf("new max_eval => %d%d %d\n", j, i, max_eval);
						max_pos_x = j;
						max_pos_y = i;
					}
				}
			}
		}

		//printf("%d\n", eval[3][1]);

		*pos_x = max_pos_x;
		*pos_y = max_pos_y;

		return;
	}

	return;

}
//決定アルゴリズム終わり

//連続石チェック先手
int str12_eval_firstphase(int x, int y, const int board[][BOARD_SIZE]){

	int seq[4][9];
	int count[4] = { 0 }, count_self = 0;
	int distance = 0;
	int seq_enemy = 0;
	int eval = 0;

	//右肩下がり
	for (int i = 0; i < 9; i++){
		if (x - 4 + i < 0 || x - 4 + i > 9 || y - 4 + i < 0 || y - 4 + i > 9){
			seq[0][i] = -1;
		}
		else{
			seq[0][i] = board[x - 4 + i][y - 4 + i];
		}
	}

	//右肩上がり
	for (int i = 0; i < 9; i++){
		if (x - 4 + i < 0 || x - 4 + i > 9 || y + 4 - i < 0 || y + 4 - i > 9){
			seq[1][i] = -1;
		}
		else{
			seq[1][i] = board[x - 4 + i][y + 4 - i];
		}
	}

	//横まっすぐ
	for (int i = 0; i < 9; i++){
		if (x - 4 + i < 0 || x - 4 + i > 9){
			seq[2][i] = -1;
		}
		else{
			seq[2][i] = board[x - 4 + i][y];
		}
	}

	//縦まっすぐ
	for (int i = 0; i < 9; i++){
		if (y - 4 + i < 0 || y - 4 + i > 9){
			seq[3][i] = -1;
		}
		else{
			seq[3][i] = board[x][y - 4 + i];
		}
	}



	//評価計算
	//８方向評価
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (seq[i][5 + j] == STONE_BLACK){
				count[i]++;
				distance += 10 - (5 + j);
			}
			else if (seq[i][5 + j] == STONE_WHITE || seq[i][5 + j] == -1){
				break;
			}
		}
		for (int j = 0; j < 4; j++){
			if (seq[i][3 - j] == STONE_BLACK){
				count[i]++;
				distance += 3 - j + 1;
			}
			else if (seq[i][3 - j] == STONE_WHITE || seq[i][3 - j] == -1){
				break;
			}
		}

		if (seq_enemy < count[i]){
			seq_enemy = count[i];
		}

		for (int j = 0; j < 4; j++){
			if (seq[i][5 + j] == STONE_WHITE){
				count_self++;
			}
			else if (seq[i][5 + j] == STONE_BLACK || seq[i][5 + j] == -1){
				break;
			}
		}
		for (int j = 0; j < 4; j++){
			if (seq[i][3 - j] == STONE_WHITE){
				count_self++;
			}
			else if (seq[i][3 - j] == STONE_BLACK || seq[i][3 - j] == -1){
				break;
			}
		}

		if (count_self > 2){
			eval += 10;
			break;
		}
		else{
			count_self = 0;
		}
	}

	eval += seq_enemy * seq_enemy + distance;

	return eval;

}

//連続石チェック後手
int str12_eval_secondphase(int x, int y, const int board[][BOARD_SIZE]){

	int seq[4][9];
	int count[4] = { 0 }, count_self = 0;
	int distance = 0;
	int seq_enemy = 0;
	int eval = 0;

	//右肩下がり
	for (int i = 0; i < 9; i++){
		if (x - 4 + i < 0 || x - 4 + i > 9 || y - 4 + i < 0 || y - 4 + i > 9){
			seq[0][i] = -1;
		}
		else{
			seq[0][i] = board[x - 4 + i][y - 4 + i];
		}
	}

	//右肩上がり
	for (int i = 0; i < 9; i++){
		if (x - 4 + i < 0 || x - 4 + i > 9 || y + 4 - i < 0 || y + 4 - i > 9){
			seq[1][i] = -1;
		}
		else{
			seq[1][i] = board[x - 4 + i][y + 4 - i];
		}
	}

	//横まっすぐ
	for (int i = 0; i < 9; i++){
		if (x - 4 + i < 0 || x - 4 + i > 9){
			seq[2][i] = -1;
		}
		else{
			seq[2][i] = board[x - 4 + i][y];
		}
	}

	//縦まっすぐ
	for (int i = 0; i < 9; i++){
		if (y - 4 + i < 0 || y - 4 + i > 9){
			seq[3][i] = -1;
		}
		else{
			seq[3][i] = board[x][y - 4 + i];
		}
	}

	//評価計算
	//８方向評価
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (seq[i][5 + j] == STONE_BLACK){
				count[i]++;
				distance += 10 - (5 + j);
			}
			else if (seq[i][5 + j] == STONE_WHITE || seq[i][5 + j] == -1){
				break;
			}
		}
		for (int j = 0; j < 4; j++){
			if (seq[i][3 - j] == STONE_BLACK){
				count[i]++;
				distance += 3 - j + 1;
			}
			else if (seq[i][3 - j] == STONE_WHITE || seq[i][3 - j] == -1){
				break;
			}
		}

		//printf("count[%d] = %d\n", i, count[i]);
		if (seq_enemy < count[i]){
			seq_enemy = count[i];
		}

		for (int j = 0; j < 4; j++){
			if (seq[i][5 + j] == STONE_WHITE){
				count_self++;
			}
			else if (seq[i][5 + j] == STONE_BLACK || seq[i][5 + j] == -1){
				break;
			}
		}
		for (int j = 0; j < 4; j++){
			if (seq[i][3 - j] == STONE_WHITE){
				count_self++;
			}
			else if (seq[i][3 - j] == STONE_BLACK || seq[i][3 - j] == -1){
				break;
			}
		}

		if (count_self > 3){
			eval += 10000;
			break;
		}
		else{
			count_self = 0;
		}
	}

	eval += seq_enemy * seq_enemy + distance;

	return eval;

}

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
		//後攻定石
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

		//先攻定石一手目
		else if (count == 0){
			*pos_x = 5;
			*pos_y = 5;
		}
		//先攻定石二手目()
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


		// それ以降の戦略（先攻）
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
			//自分の4連を5連にする
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
			// 相手の4連を止める
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
			//自分の両端空き3連を4連にする
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
			// 相手の3連を止める(両端が開いている場合のみ
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
						//ループ回数を決める
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


		//後攻の戦略
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
					//自分の4連を5連にする
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
			// 相手の4連を止める
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
			//自分の空き3連を4連にする
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
					// 相手の3連を止める(両端が開いている場合のみ
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
// Group A18
//=======================================================
#include <functional>

double str1_ef5[32][5] = {
	/* 00000 */{ 0, 0, 0, 0, 0 },
	/* 00001 */{ 0.1, 0.2, 0.5, 1, 0 },
	/* 00010 */{ 0.2, 0.5, 1, 0, 1 },
	/* 00011 */{ 0.5, 1, 2, 0, 0 },
	/* 00100 */{ 0.5, 1, 0, 1, 0.5 },
	/* 00101 */{ 1, 2, 0, 5, 0 },
	/* 00110 */{ 1, 2, 0, 0, 2 },
	/* 00111 */{ 16, 32, 0, 0, 0 },

	/* 01000 */{ 1, 0, 1, 0.5, 0.2 },
	/* 01001 */{ 1, 0, 2, 2, 0 },
	/* 01010 */{ 2, 0, 5, 0, 1 },
	/* 01011 */{ 2, 0, 4, 0, 0 },
	/* 01100 */{ 4, 0, 0, 4, 2 },
	/* 01101 */{ 2, 0, 0, 4, 0 },
	/* 01110 */{ 100, 0, 0, 0, 80 },
	/* 01111 */{ 100, 0, 0, 0, 0 },


	/* 10000 */{ 0, 1, 0.5, 0.2, 0.1 },
	/* 10001 */{ 0, 1.5, 0.6, 1.5, 0 },
	/* 10010 */{ 0, 4, 4, 0, 2 },
	/* 10011 */{ 0, 4, 4, 0, 0 },
	/* 10100 */{ 0, 5, 0, 2, 0 },
	/* 10101 */{ 0, 6, 0, 6, 0 },
	/* 10110 */{ 0, 100, 0, 0, 80 },
	/* 10111 */{ 0, 100, 0, 0, 0 },

	/* 11000 */{ 0, 0, 2, 1, 0.5 },
	/* 11001 */{ 0, 0, 4, 4, 0 },
	/* 11010 */{ 0, 0, 100, 0, 80 },
	/* 11011 */{ 0, 0, 100, 0, 0 },
	/* 11100 */{ 0, 0, 0, 100, 80 },
	/* 11101 */{ 0, 0, 0, 100, 0 },
	/* 11110 */{ 0, 0, 0, 0, 100 },
	/* 11111 */{ 0, 0, 0, 0, 0 },
};


void str1_calcScore(int i1len,
	std::function<int(int)> getI2Length,
	std::function<int(int, int)> getStone,
	std::function<double*(int, int)> getScore,
	int mystone,
	double rate
	) {

	for (int i1 = 0; i1<i1len; i1++) {

		int len_beg = -1;
		unsigned long pattern = 0;

		int i2len = getI2Length(i1);
		for (int i2 = 0; i2<i2len; i2++) {
			int cstone = getStone(i1, i2);

			//	自分の石か、空白なら
			if (!cstone || cstone == mystone) {
				//	連続開始の場合、連続開始位置を設定
				if (len_beg < 0) len_beg = i2;
				//	石の有無を記録
				pattern = (pattern << 1) | (!!cstone);
			}
			else{
				//	連続を終了
				len_beg = -1;
			}

			//	五連続になれば
			if (i2 - len_beg + 1 == 5) {
				double* cef5 = str1_ef5[pattern & 31];
				for (int i = 0; i<5; i++) *getScore(i1, len_beg + i) += rate * cef5[i];
			}

		}
	}

}

void str1_calcStoneAll(const int board[][BOARD_SIZE], double score[][BOARD_SIZE], int mystone, double rate) {

	//	X方向
	str1_calcScore(BOARD_SIZE,
		[](int i1){ return BOARD_SIZE; },
		[&](int i1, int i2){ return  board[i1][i2]; },
		[&](int i1, int i2){ return &score[i1][i2]; },
		mystone, rate
		);

	//	Y方向
	str1_calcScore(BOARD_SIZE,
		[](int i1){ return BOARD_SIZE; },
		[&](int i1, int i2){ return  board[i2][i1]; },
		[&](int i1, int i2){ return &score[i2][i1]; },
		mystone, rate
		);

	//	右下がり斜め方向
	str1_calcScore(BOARD_SIZE * 2 - 1,
		[](int i1){ return (i1 <= 9) ? i1 + 1 : 19 - i1; },
		[&](int i1, int i2){ return  board[i1 < 9 ? 9 - i1 + i2 : i2][i1 < 9 ? i2 : i1 - 9 + i2]; },
		[&](int i1, int i2){ return &score[i1 < 9 ? 9 - i1 + i2 : i2][i1 < 9 ? i2 : i1 - 9 + i2]; },
		mystone, rate
		);

	//	左下がり斜め方向
	str1_calcScore(BOARD_SIZE * 2 - 1,
		[](int i1){ return (i1 <= 9) ? i1 + 1 : 19 - i1; },
		[&](int i1, int i2){ return  board[i1 < 9 ? i2 : i2 + i1 - 9][i1 < 9 ? i1 - i2 : 9 - i2]; },
		[&](int i1, int i2){ return &score[i1 < 9 ? i2 : i2 + i1 - 9][i1 < 9 ? i1 - i2 : 9 - i2]; },
		mystone, rate
		);

}


void strategyA18(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {

	printf("Strategy A18\t");

	//	今の色
	int mystone = (count % 2) ? STONE_BLACK : STONE_WHITE;
	int enstone = (mystone == STONE_WHITE) ? STONE_BLACK : STONE_WHITE;

	//	評価値
	double score[BOARD_SIZE][BOARD_SIZE] = { 0 };

	//	評価値を計算
	str1_calcStoneAll(board, score, enstone, 2.0);
	//	str1_calcStoneAll(board, score, mystone, 1.0);

	//	
	double cmax = 0;
	int xmax = -1;
	int ymax = -1;

	for (int iy = 0; iy<BOARD_SIZE; iy++) {
		for (int ix = 0; ix<BOARD_SIZE; ix++) {
			if (board[iy][ix]) continue;
			if (xmax < 0 || ymax < 0){ xmax = ix; ymax = iy; }
			double cscore = score[iy][ix];
			if (cmax < cscore){
				cmax = cscore;
				xmax = ix;
				ymax = iy;
			}
		}
	}

	*pos_x = xmax;
	*pos_y = ymax;
}


//=======================================================
// Group B18
//=======================================================
int strB18_Count_FreeTwo(const int board[][BOARD_SIZE], int AA, int BB, int twocount);
void strB18_AttackPhase(const int board[][BOARD_SIZE], int point[][BOARD_SIZE], int AA, int BB, int me);
void strB18_AddPoint_AttackME(int point[][BOARD_SIZE], int x, int y, int len, int isFree, int mytwo);
void strB18_AddPoint_AttackOP(int point[][BOARD_SIZE], int x, int y, int len, int isFree, int mytwo);
void strB18_AddPoint_Defense(int point[][BOARD_SIZE], int x, int y, int len, int isFree);
#define TRUE 1
#define FALSE 0

void strategyB18(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int point[BOARD_SIZE][BOARD_SIZE];
	position choices[BOARD_SIZE*BOARD_SIZE];
	int i, j, k, l;
	int mytwo = 0, optwo = 0;
	int len, max = 0, x_tmp = 0, y_tmp = 0;
	int dx[] = { 0, 1, 1, -1 };
	int dy[] = { 1, 0, 1, 1 }; //順番に{下、右、右下、左下};

	printf("Strategy B18\t\n");

	//自分(me)と相手(opponent)の確認
	int me = (count % 2 == 0 ? STONE_BLACK : STONE_WHITE);
	int opponent = (count % 2 == 0 ? STONE_WHITE : STONE_BLACK);

	//定石(1～3手目まで)
	switch (count){
	case 0:
		*pos_x = 5;
		*pos_y = 5;
		return;
	case 1:
		*pos_x = history[count - 1].x;
		*pos_y = (history[count - 1].y < 5 ? history[count - 1].y + 1 : history[count - 1].y - 1);
		return;
	case 2:
		if (board[4][5] == opponent || board[5][4] == opponent || board[6][6] == opponent){
			*pos_x = 4;
			*pos_y = 4;
			return;
		}
		else if (board[4][6] == opponent){
			*pos_x = 4;
			*pos_y = 6;
			return;
		}
		else if (board[4][4] == opponent){
			*pos_x = 6;
			*pos_y = 4;
			return;
		}
		else if (board[5][6] == opponent || board[6][4] == opponent || board[6][5] == opponent){
			*pos_x = 6;
			*pos_y = 6;
			return;
		}
		else break;
	default: break;
	}

	//点数(point)の初期化
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			point[i][j] = 0;
			if (board[i][j] == STONE_SPACE)
				point[i][j] += 1;
		}
	}

	//2連チェック
	//printf("kの方向\t 0:下 1:右 2:右下 3:左下\n");
	mytwo = strB18_Count_FreeTwo(board, me, opponent, mytwo);
	optwo = strB18_Count_FreeTwo(board, opponent, me, optwo);

	if (optwo == 0 || mytwo > optwo){		//攻め
		strB18_AttackPhase(board, point, me, opponent, TRUE);		//自分にとって有利なマスを評価する
		strB18_AttackPhase(board, point, opponent, me, FALSE);	//相手にとって有利なマスを評価する
	}

	else{		//守り
		for (i = 0; i < BOARD_SIZE; i++){
			for (j = 0; j < BOARD_SIZE; j++){
				if (board[i][j] == opponent){
					for (k = 0; k < 4; k++){
						if (board[i - dy[k]][j - dx[k]] == opponent
							&& i - dy[k] < BOARD_SIZE
							&& j - dx[k] < BOARD_SIZE
							&& i - dy[k] >= 0
							&& j - dx[k] >= 0)
							continue;	//進行方向の逆側が opponent かつ盤内 はチェック済みやからスキップ

						for (l = 1, len = 1; l <= 4; l++){
							if (board[i + l*dy[k]][j + l*dx[k]] == opponent){	//進行方向が opponent
								len++;
								if (l == 4){
									if (board[i - dy[k]][j - dx[k]] == me)
										strB18_AddPoint_Defense(point, x_tmp, y_tmp, len, FALSE);
									else
										strB18_AddPoint_Defense(point, x_tmp, y_tmp, len, TRUE);
								}
								continue;
							}

							else if (board[i + l*dy[k]][j + l*dx[k]] == me		//進行方向が me または盤外
								|| i + l*dy[k] >= BOARD_SIZE
								|| j + l*dx[k] >= BOARD_SIZE
								|| i + l*dy[k] < 0
								|| j + l*dx[k] < 0)
							{
								if (len < 4					//lenが4に満たず進行方向の逆側が盤外なら
									&& (i + l*dy[k] >= BOARD_SIZE
									|| j + l*dx[k] >= BOARD_SIZE
									|| i + l*dy[k] < 0
									|| j + l*dx[k] < 0))
									break;
								else if (board[i - dy[k]][j - dx[k]] == STONE_SPACE				//進行方向の逆側が空なら
									&& i - dy[k] < BOARD_SIZE
									&& j - dx[k] < BOARD_SIZE
									&& i - dy[k] >= 0
									&& j - dx[k] >= 0)
									strB18_AddPoint_Defense(point, j - dx[k], i - dy[k], len, FALSE);		//進行方向の逆側に加点

								break;
							}

							else{												//進行方向が 空
								if (board[i + (l + 1)*dy[k]][j + (l + 1)*dx[k]] == opponent && l != 4){ 	//進行方向の1つ奥が opponentなら
									x_tmp = j + l*dx[k];													//空マスを保存して続行
									y_tmp = i + l*dy[k];
									continue;
								}

								if (board[i - dy[k]][j - dx[k]] == STONE_SPACE					//進行方向の逆側が空なら
									&& i - dy[k] < BOARD_SIZE
									&& j - dx[k] < BOARD_SIZE
									&& i - dy[k] >= 0
									&& j - dx[k] >= 0)
								{
									strB18_AddPoint_Defense(point, j + l*dx[k], i + l*dy[k], len, TRUE);	//進行方向に加点
									strB18_AddPoint_Defense(point, j - dx[k], i - dy[k], len, TRUE);		//進行方向の逆側にも加点
								}

								else															//空でないなら
									strB18_AddPoint_Defense(point, j + l*dx[k], i + l*dy[k], len, FALSE);	//進行方向にだけ加点

								break;
							}
						}
					}
				}
			}
		}
	}

	/*	pointの確認
	printf(" 0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
	for (i = 0; i < BOARD_SIZE; i++){
	printf("%d ", i);
	for (j = 0; j < BOARD_SIZE; j++)
	printf("%d\t", point[i][j]);
	//		printf("\n");
	}
	*/

	//最高点のマスを(choices[]に)記録
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			if (point[i][j] > max)
				max = point[i][j];				//maxを保存

	for (i = 0, k = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (point[i][j] == max){
				choices[k].x = j;
				choices[k].y = i;
				k++;
			}
		}
	}

	//最高点のマス(choices[])からランダムに選択
	double tmp_d = (double)rand() / ((double)RAND_MAX + 1);
	int pos = tmp_d * k;
	*pos_x = choices[pos].x;
	*pos_y = choices[pos].y;

	return;
}

int strB18_Count_FreeTwo(const int board[][BOARD_SIZE], int AA, int BB, int twocount)
{
	int i, j, k, l, len;
	int dx[] = { 0, 1, 1, -1 };
	int dy[] = { 1, 0, 1, 1 }; //順番に{下、右、右下、左下};

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (board[i][j] == AA){
				for (k = 0; k < 4; k++){
					if (board[i - dy[k]][j - dx[k]] == AA
						&& i - dy[k] < BOARD_SIZE
						&& j - dx[k] < BOARD_SIZE
						&& i - dy[k] >= 0
						&& j - dx[k] >= 0)
						continue;

					for (l = 1, len = 1; l <= 4; l++){
						if (l < 4){					//進行方向に敵がいる or 盤外
							if (board[i + l*dy[k]][j + l*dx[k]] == BB
								|| i + l*dy[k] >= BOARD_SIZE
								|| j + l*dx[k] >= BOARD_SIZE
								|| i + l*dy[k] < 0
								|| j + l*dx[k] < 0)
								break;

							else if (board[i + l*dy[k]][j + l*dx[k]] == AA){
								len++;
								continue;
							}
						}

						else{			//(l ==4)
							if (board[i + l*dy[k]][j + l*dx[k]] == AA)
								len++;

							if (len == 4){
								if ((board[i + l*dy[k]][j + l*dx[k]] == BB
									|| i + l*dy[k] >= BOARD_SIZE
									|| j + l*dx[k] >= BOARD_SIZE
									|| i + l*dy[k] < 0
									|| j + l*dx[k] < 0)
									&& (board[i - dy[k]][j - dx[k]] == BB
									|| i - dy[k] >= BOARD_SIZE
									|| j - dx[k] >= BOARD_SIZE
									|| i - dy[k] < 0
									|| j - dx[k] < 0))
									break;

								else if (board[i + l*dy[k]][j + l*dx[k]] != BB || board[i - dy[k]][j - dx[k]] != BB){
									twocount = 10;
									//printf("四連検知 / i:%d, j:%d, k:%d, len:%d\n", i, j, k, len);
								}
							}
							else if (len >1){
								if (board[i - dy[k]][j - dx[k]] == BB)
									break;
								//printf("x:%d, y:%d, k:%d, len:%d\n", i, j, k, len);
								twocount++;
							}
						}
					}
				}
			}
		}
	}
	//printf("twocount: %d\n", twocount);
	return twocount;
}

void strB18_AttackPhase(const int board[][BOARD_SIZE], int point[][BOARD_SIZE], int AA, int BB, int me)
{
	int i, j, k, l;
	int len, mytwo = 0, optwo = 0;
	int x_tmp = 0, y_tmp = 0;
	int dx[] = { 0, 1, 1, -1, 0, -1, -1, 1 };
	int dy[] = { 1, 0, 1, 1, -1, 0, -1, -1 };		//下、右、右下、左下 | 上、左、左上、右上

	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if (board[i][j] == STONE_SPACE){ //まずはSTONE_SPACEを見つける
				for (k = 0; k < 8; k++){
					x_tmp = 0, y_tmp = 0;

					for (l = 1, len = 0; l <= 4; l++){
						if (board[i + l*dy[k]][j + l*dx[k]] == AA						//進行方向が AA
							&& i - dy[k] < BOARD_SIZE
							&& j - dx[k] < BOARD_SIZE
							&& i - dy[k] >= 0
							&& j - dx[k] >= 0)
						{
							len++;

							if (l == 4 && x_tmp != 0 && y_tmp != 0){ 							//飛び4検知
								if (me)
									strB18_AddPoint_AttackME(point, x_tmp, y_tmp, len, FALSE, mytwo);
								else
									strB18_AddPoint_AttackOP(point, x_tmp, y_tmp, len, TRUE, mytwo);
							}
							continue;
						}
						else if (board[i + l*dy[k]][j + l*dx[k]] == BB				//進行方向が BB または盤外
							|| i + l*dy[k] >= BOARD_SIZE
							|| j + l*dx[k] >= BOARD_SIZE
							|| i + l*dy[k] < 0
							|| j + l*dx[k] < 0)
						{
							if (len < 4					//lenが4に満たず進行方向の逆側が盤外なら
								&& (i + l*dy[k] >= BOARD_SIZE
								|| j + l*dx[k] >= BOARD_SIZE
								|| i + l*dy[k] < 0
								|| j + l*dx[k] < 0))
								break;
							else if (len > 0){
								if (me)
									strB18_AddPoint_AttackME(point, j, i, len, FALSE, mytwo);
								else
									strB18_AddPoint_AttackOP(point, j, i, len, FALSE, mytwo);
							}

							break;
						}
						else if (board[i + l*dy[k]][j + l*dx[k]] == STONE_SPACE){	//進行方向が 空
							if (board[i + (l + 1)*dy[k]][j + (l + 1)*dx[k]] == AA && l != 4){		//進行方向の1つ奥がAAなら
								x_tmp = j + l*dx[k];												//空マスを保存して続行
								y_tmp = i + l*dy[k];
								continue;
							}

							else if (len > 0){
								if (board[i - dy[k]][j - dx[k]] == BB 						//進行方向の逆側が BB または盤外なら
									|| i + l*dy[k] >= BOARD_SIZE
									|| j + l*dx[k] >= BOARD_SIZE
									|| i + l*dy[k] < 0
									|| j + l*dx[k] < 0)
								{
									if (me)
										strB18_AddPoint_AttackME(point, j, i, len, FALSE, mytwo);
									else
										strB18_AddPoint_AttackOP(point, j, i, len, FALSE, mytwo);
								}
								else{
									if (me)
										strB18_AddPoint_AttackME(point, j, i, len, TRUE, mytwo);
									else
										strB18_AddPoint_AttackOP(point, j, i, len, TRUE, mytwo);
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

void strB18_AddPoint_AttackME(int point[][BOARD_SIZE], int x, int y, int len, int isFree, int mytwo)
{
	if (mytwo >= 2){				//二連が2つ以上の場合
		if (isFree){						//両空
			switch (len){
			case 1: point[y][x] += 30; break;
			case 2: point[y][x] += 60; break;
			case 3: point[y][x] += 20000; break;
			case 4: point[y][x] += 1919810; break;
			}
		}

		else{								//片空
			switch (len){
			case 1: point[y][x] += 5; break;
			case 2: point[y][x] += 10; break;
			case 3: point[y][x] += 15; break;
			case 4: point[y][x] += 114514; break;
			}
		}
	}

	else{							//二連が1つ以下の場合
		if (isFree){						//両空
			switch (len){
			case 1: point[y][x] += 10; break;
			case 2: point[y][x] += 9; break;
			case 3: point[y][x] += 200; break;
			case 4: point[y][x] += 1919810; break;
			}
		}

		else{								//片空
			switch (len){
			case 1: point[y][x] += 10; break;
			case 2: point[y][x] += 5; break;
			case 3: point[y][x] += 10000; break;
			case 4: point[y][x] += 114514; break;
			}
		}
	}
}

void strB18_AddPoint_AttackOP(int point[][BOARD_SIZE], int x, int y, int len, int isFree, int mytwo)
{
	if (mytwo > 1){					//二連が2つ以上の場合
		if (isFree){						//両空
			switch (len){
			case 1: point[y][x] += 30; break;
			case 2: point[y][x] += 60; break;
			case 3: point[y][x] += 20000; break;
			case 4: point[y][x] += 1919810; break;
			}
		}

		else{								//片空
			switch (len){
			case 1: point[y][x] += 5; break;
			case 2: point[y][x] += 10; break;
			case 3: point[y][x] += 15; break;
			case 4: point[y][x] += 114514; break;
			}
		}
	}

	else{							//二連が1つ以下
		if (isFree){						//両空
			switch (len){
			case 1: point[y][x] += 10; break;
			case 2: point[y][x] += 9; break;
			case 3: point[y][x] += 200; break;
			case 4: point[y][x] += 1919810; break;
			}
		}

		else{								//片空
			switch (len){
			case 1: point[y][x] += 10; break;
			case 2: point[y][x] += 5; break;
			case 3: point[y][x] += 100; break;
			case 4: point[y][x] += 114514; break;
			}
		}
	}
}

void strB18_AddPoint_Defense(int point[][BOARD_SIZE], int x, int y, int len, int isFree)
{
	if (isFree) {							//両空
		switch (len){
		case 1: point[y][x] += 4; break;
		case 2: point[y][x] += 55; break;
		case 3: point[y][x] += 100; break;
		case 4: point[y][x] += 810; break;
		}
	}

	else{									//片空
		switch (len){
		case 1: point[y][x] += 2; break;
		case 2: point[y][x] += 10; break;
		case 3: point[y][x] += 50; break;
		case 4: point[y][x] += 405; break;
		}
	}
}

//=======================================================
// Group C18
//=======================================================
//主幹となる関数は一番下にあります(strategy1)

namespace C18
{

	position choice;

	static int state = 0;

	int myColor;
	int opsColor;

	void boardcpy(const int orig[][BOARD_SIZE], int cpy[][BOARD_SIZE]) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				cpy[i][j] = orig[i][j];
			}
		}
		return;
	}

	void boardshow(const int b[][BOARD_SIZE]) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				printf("%5d ", b[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}

	void init(int count) {
		if (count == 0) {
			state = 1;
			myColor = STONE_BLACK;
			opsColor = STONE_WHITE;
		}
		else {
			state = 2;
			myColor = STONE_WHITE;
			opsColor = STONE_BLACK;
		}
	}

	int ft_m(int t) {
		switch (t) {
		case 1:
			return 1;
		case 2:
			return 3;
		case 3:
			return 10;//5
		case 4:
			return 15;//10
		default:
			return 0;
		}
	}

	int ft_o(int t) {
		switch (t) {
		case 1:
			return 4;//2
		case 2:
			return 8;
		case 3:
			return 50;//10
		case 4:
			return 100;//20
		default:
			return 0;
		}
	}

	int strategyC18(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
	{

		choice.x = -1;
		choice.y = -1;

		if (state == 0) init(count);

		if (state == 1) {
			*pos_x = 4;
			*pos_y = 5;
			state = 2;
			return 0;
		}

		int board_eval[BOARD_SIZE][BOARD_SIZE] = { 0 };

		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {

				//board_eval[i][j] = 0;
				if (board[i][j] != STONE_SPACE) board_eval[i][j] -= 9999;
				//->
				if (j <= 5) {
					int t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i][j + k] == opsColor) break;
						if (board[i][j + k] == myColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i][j + p] += ft_m(t);
							}
						}
					}

					t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i][j + k] == myColor) break;
						if (board[i][j + k] == opsColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i][j + p] += ft_o(t);
							}
						}
					}
				}

				//|
				//v
				if (i <= 5) {
					int t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i + k][j] == opsColor) break;
						if (board[i + k][j] == myColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i + p][j] += ft_m(t);
							}
						}
					}

					t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i + k][j] == myColor) break;
						if (board[i + k][j] == opsColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i + p][j] += ft_o(t);
							}
						}
					}
				}

				//|->
				if (j <= 5 && i <= 5) {
					int t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i + k][j + k] == opsColor) break;
						if (board[i + k][j + k] == myColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i + p][j + p] += ft_m(t);
							}
						}
					}

					t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i + k][j + k] == myColor) break;
						if (board[i + k][j + k] == opsColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i + p][j + p] += ft_o(t);
							}
						}
					}
				}

				//<-|
				if (j >= 4 && i <= 5) {
					int t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i + k][j - k] == opsColor) break;
						if (board[i + k][j - k] == myColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i + p][j - p] += ft_m(t);
							}
						}
					}

					t = 0;
					for (int k = 0; k < 5; k++) {
						if (board[i + k][j - k] == myColor) break;
						if (board[i + k][j - k] == opsColor) t++;
						if (k == 4) {
							for (int p = 0; p < 5; p++) {
								board_eval[i + p][j - p] += ft_o(t);
							}
						}
					}
				}

			}
		}

		//boardshow(board_eval);

		int max = 0;
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (board_eval[i][j] > max) {
					max = board_eval[i][j];
					choice.x = j;
					choice.y = i;
				}
				else if (board_eval[i][j] == max) {
					if (rand() > 0.6) {
						choice.x = j;
						choice.y = i;
					}
				}
			}
		}

		if (board[choice.y][choice.x] == STONE_SPACE) {
			*pos_x = choice.x;
			*pos_y = choice.y;
		}
		else printf("???\n");

		return 0;

	}
}

//主幹となる関数
void strategyC18(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {
	printf("Strategy C18\t");
	C18::strategyC18(board, pos_x, pos_y, count, history);
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

	for (i = 0; i < BOARD_SIZE; i++) { // 盤面の評価値のリセット 
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

	//先手

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

	//後手

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

		//縦下
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

		//縦上
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

		//横右
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

		//横左
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

		//斜め右下
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

		//斜め右上
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

		//斜め左上
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

		//斜め左下
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


		//6個判定バージョン
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

		//縦上
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

		//横右
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

		//横左
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

		//斜め右下
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

		//斜め右上
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

		//斜め左上
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

		//斜め左下
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

	//判定
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
// Group E18
//=======================================================
int myEvaluation(int st, int sp, int which_turn);
int enemyEvaluation(int st, int sp, int which_turn);

void strategyE18(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int score[BOARD_SIZE][BOARD_SIZE]; // 評価値記憶用の配列


	int max = -1000000;

	int i, j, x, y;

	int dy[] = { -1, 1, 0, 0, -1, 1, -1, 1 }; //上、下、左、右、左上、右下、右上、左下の順
	int dx[] = { 0, 0, -1, 1, -1, 1, 1, -1 };

	int stonecount = 0, stone = 0, space = 0;

	printf("Strategy E18\t");

	for (i = 0; i < BOARD_SIZE; i++) { // 盤面評価値の初期化
		for (j = 0; j < BOARD_SIZE; j++) {
			score[i][j] = 1;
		}
	}

	//-------------------------------------------------------
	// 先行・後攻の判断
	//-------------------------------------------------------
	int which_turn = count % 2 == 0 ? STONE_BLACK : STONE_WHITE; //先行、後攻
	//-------------------------------------------------------
	// 評価開始
	//-------------------------------------------------------

	//１手目（先手）
	if (count == 0) {
		*pos_x = BOARD_SIZE / 2;
		*pos_y = BOARD_SIZE / 2;
	}
	//1手目（後攻）
	else if (count == 1) {//広いほうへ斜めに置く
		if (history[0].x < BOARD_SIZE / 2)
			*pos_x = history[0].x + 1;
		else
			*pos_x = history[0].x - 1;
		if (history[0].y < BOARD_SIZE / 2)
			*pos_y = history[0].y + 1;
		else
			*pos_y = history[0].y - 1;
	}
	else if (count == 2) { //absは絶対値を計算 浦月 花月
		if ((history[0].x == history[1].x && abs(history[0].y - history[1].y) == 1) || (history[0].y == history[1].y && abs(history[0].x - history[1].x) == 1)) {//上下左右のいずれかに置かれたなら
			if (history[1].x == 4 || history[1].y == 4) {//4.5,5.4なら4.4に置く
				*pos_x = 4;
				*pos_y = 4;
			}
			else if (history[1].x == 6 || history[1].y == 6) {// 6.5, 5.6なら6.6に置く
				*pos_x = 6;
				*pos_y = 6;
			}
		}
		else if (abs(history[0].x - history[1].x) == 1 && abs(history[0].y - history[1].y) == 1) {//斜めの位置に置かれたなら
			if (history[1].x == 4) {
				if (history[1].y == 4) {//4.4なら4.6へ
					*pos_x = 4;
					*pos_y = 6;
				}
				else if (history[1].y == 6) {//4.6なら4.4へ
					*pos_x = 4;
					*pos_y = 4;
				}
			}
			else if (history[1].x == 6) {
				if (history[1].y == 4) {//6.4なら4.4へ
					*pos_x = 4;
					*pos_y = 4;
				}
				else if (history[1].y == 6) {//6.6なら4.6へ
					*pos_x = 4;
					*pos_y = 6;
				}
			}
		}
		else { //隣以外に置かれたなら
			*pos_x = 4;
			*pos_y = 4;
		}
		return;
	}
	else {
		for (y = 0; y < BOARD_SIZE; y++) {
			for (x = 0; x < BOARD_SIZE; x++) {
				if (board[y][x] == STONE_SPACE) {
					for (i = 0; i < 8; i++) { //dy,dx
						for (j = 1; j < 7; j++) { //置いた位置から距離6以内に <- 飛びの評価で距離が6必要
							//自分の石が調べている場所にあるならstonecount++
							if ((board[y + j * dy[i]][x + j * dx[i]] == which_turn) && ((y + j * dy[i]) >= 0) && ((y + j * dy[i]) < 10) && ((x + j * dx[i]) >= 0) && ((x + j * dx[i]) < 10)) {
								stonecount++;
							}
							//もし自分の石が調べている場所にあり、そこが盤上ならstone++
							if ((board[y + j * dy[i]][x + j * dx[i]] == which_turn) && ((y + j * dy[i]) >= 0) && ((y + j * dy[i]) < 10) && ((x + j * dx[i]) >= 0) && ((x + j * dx[i]) < 10)) {
								stone++;
							}
							//1つ前に調べた石が自分の石、もしくは置くと予想された石で、かつ、現在調べている場所が空白ならspace++
							else if (board[y + j * dy[i]][x + j * dx[i]] == STONE_SPACE && (board[y + (j - 1) * dy[i]][x + (j - 1) * dx[i]] == which_turn || (y + (j - 1) * dy[i] == y && x + (j - 1) * dx[i] == x))) {
								space++;
								break;
							}
							else {
								break;
							}
						}
						if (i % 2 != 0) {
							//評価値
							if (stone >= 1) {
								score[y][x] *= myEvaluation(stone, space, which_turn);
							}
							//初期化
							stone = 0;
							space = 0;
							stonecount = 0;
						}
					}
				}
			}
		}
		//初期化
		max = -1000000;
		stone = 0;
		space = 0;
		stonecount = 0;
		for (y = 0; y < BOARD_SIZE; y++) {
			for (x = 0; x < BOARD_SIZE; x++) {
				if (board[y][x] == STONE_SPACE) {
					for (i = 0; i < 8; i++) {
						for (j = 1; j < 7; j++) {
							//調べている場所に相手の石
							if (board[y + j * dy[i]][x + j * dx[i]] != which_turn && ((y + j * dy[i]) >= 0) && ((y + j * dy[i]) < 10) && ((x + j * dx[i]) >= 0) && ((x + j * dx[i]) < 10)) {
								stonecount++;
							}
							if (board[y + j * dy[i]][x + j * dx[i]] != which_turn && board[y + j * dy[i]][x + j * dx[i]] != 0 && ((y + j * dy[i]) >= 0) && ((y + j * dy[i]) < 10) && ((x + j * dx[i]) >= 0) && ((x + j * dx[i]) < 10)) {
								stone++;
							}
							else if (board[y + j * dy[i]][x + j * dx[i]] == STONE_SPACE && ((board[y + (j - 1) * dy[i]][x + (j - 1) * dx[i]] != which_turn && board[y + (j - 1) * dy[i]][x + (j - 1) * dx[i]] != 0) || (y + (j - 1) * dy[i] == y && x + (j - 1) * dx[i] == x))) {
								space++;
								break;
							}
							else {
								break;
							}
						}
						if (i % 2 != 0) {
							if (stone >= 1) {
								score[y][x] *= enemyEvaluation(stone, space, which_turn);
							}
							stone = 0;
							space = 0;
							stonecount = 0;
							if (i == 7) { //全部確認した
								if (y == 0 || y == 9 || x == 0 || x == 9) { // 壁側よりも内側の方が評価が高い
									score[y][x] -= 30;
								}
								else if (y == 1 || y == 8 || x == 1 || x == 8) {
									score[y][x] -= 15;
								}
								if (max < score[y][x]) { //評価値が最大のものを抽出
									max = score[y][x];
									*pos_x = x;
									*pos_y = y;
								}
							}
						}
					}
				}
			}
		}
	}
	printf("%d", max);
}


int myEvaluation(int st, int sp, int which_turn) {
	if (st >= 4) { //5連を作る
		if (which_turn == 1) {
			return 10000000;
		}
		else {
			return 7000000;
		}
	}
	else if (st == 3 && sp >= 2) {//4連を作る 飛び4を含む
		if (which_turn == 1) {
			return 300000;
		}
		else {
			return 200000;
		}
	}
	else if (st == 3 && sp == 1) {//4連を作る 片側がブロックされている
		if (which_turn == 1) {
			return 420;
		}
		else {
			return 350;
		}
	}
	else if (st == 2 && sp >= 2) {//3連を作る 飛び3を作るのも含む
		if (which_turn == 1) {
			return 180;
		}
		else {
			return 150;
		}
	}
	else if (st == 2 && sp == 1) {//3連を作る 片側がブロックされている
		if (which_turn == 1) {
			return 6;
		}
		else {
			return 4;
		}
	}
	else if (st == 1 && sp == 2) {//フリーの2連を作る
		if (which_turn == 1) {
			return 10;
		}
		else {
			return 8;
		}
	}
	else {
		return 1;
	}
}

int enemyEvaluation(int st, int sp, int which_turn) {
	if (st >= 4) {
		if (which_turn == 1) {//5連を作らせない
			return 10000000;
		}
		else {
			return 15000000;
		}
	}
	else if (st == 3 && sp >= 2) {//4連を作らせない 飛び4を含む
		if (which_turn == 1) {
			return 500000;
		}
		else {
			return 800000;
		}
	}
	else if (st == 3 && sp == 1) {//4連を作らせない 片側をすでにブロックできている
		if (which_turn == 1) {
			return 450;
		}
		else {
			return 600;
		}
	}
	else if (st == 2 && sp >= 2) {//3連を作らせない 飛び3を含む
		if (which_turn == 1) {
			return 200;
		}
		else {
			return 250;
		}
	}
	else if (st == 2 && sp == 1) {//3連を作らせない 片側をすでにブロックできている
		return 2;
	}
	else {
		return 1;
	}
}

//=======================================================
// main
//=======================================================
int main(int argc, char **argv)
{
	// 変数宣言
	int board[BOARD_SIZE][BOARD_SIZE];
	position *history;
	int which_turn;
	int count;
	int group[2];

	if (argc != 3)
	{
		printf("gobang group_first group_second\n");
		printf("  group_first: 先攻のグループ番号\n");
		printf("  group_first: 後攻のグループ番号\n");
		exit(1);
	}
	else
	{
		group[0] = atoi(argv[1]);
		group[1] = atoi(argv[2]);
		printf("group_first (Ｘ) = %d\n", group[0]);
		printf("group_second (Ｏ) = %d\n", group[1]);

		fprintf(stderr, "First: %d, Second: %d\n", group[0], group[1]);
	}

	// 初期処理
	gameInit(board, &which_turn);
	history = (position*)malloc(sizeof(position) * BOARD_SIZE * BOARD_SIZE);
	boardPrint(board);
	srand((unsigned)time(NULL));

	//---- メインループ
	for (count = 0; count < BOARD_SIZE * BOARD_SIZE; count++){
		//--- 入力処理
		inputPutPos(board, which_turn, count, history, group);

		//--- 出力処理
		boardPrint(board);

		//--- 終了判定
		if (gameEndProcess(board)) { break; }

		//--- 演算処理
		changeTurn(&which_turn);
	}

	if (count == BOARD_SIZE * BOARD_SIZE){
		fprintf(stderr, "引き分けです\n");
		printf("引き分けです\n");
	}

	free(history);

	return 0;
}

//-------------------------------------------------
// 置く場所入力
//-------------------------------------------------
void inputPutPos(int board[][BOARD_SIZE], const int which, const int count, position *history, const int *group)
{
	int pos_x, pos_y;
	char str[256];
	strategy strtgy[16] = { randomStrategy,
		strategy5,
		strategy6,
		strategy12,
		strategy15,
		strategyA18,
		strategyB18,
		strategyC18,
		strategyD18,
		strategyE18,
		keyboardInput,// strategy10 が存在しないため，手打ち入力をここにいれている
	};
	struct tm *date;
	time_t current_time;

	time_t stime, etime;
	int ptime;

	printf("%s", (which == 1) ? "Ｘ" : "Ｏ");
	printf("の番です。どこに置きますか\n");

	while (1) {

		// 手の数と時刻を表示
		current_time = time(NULL);
		date = localtime(&current_time);
		strftime(str, 255, "%Y, %B, %d, %A %p%I:%M:%S", date);
		printf("第 %d 手思考開始： Time: %s\n", count + 1, str);
		printf("> ");
		/*
		fprintf( stderr, "第 %d 手思考開始： Time: %s\n", count + 1, str );
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
		printf("第 %d 手思考終了： Time: %s\n", count + 1, str);
		/*
		fprintf( stderr, "第 %d 手思考終了： Time: %s\n", count + 1, str );
		*/

		ptime = difftime(etime, stime);

		if (ptime >= 61)
		{
			printf("時間オーバー:%d\n", ptime);
			printf("%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ");
			/*
			fprintf( stderr, "時間オーバー:%d\n", ptime);
			fprintf( stderr, "%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ" );
			*/
			exit(1);
		}
		else
		{
			printf("思考時間:%d\n", ptime);
		}
		printf("%d %d\n", pos_x, pos_y);

		if (checkOutPos(pos_x, pos_y) && board[pos_y][pos_x] == STONE_SPACE) { break; }
		printf("不正な入力です\n> ");
		printf("%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ");
		/*
		fprintf( stderr, "不正な入力です\n> ");
		fprintf( stderr, "%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ" );
		*/
		exit(1);
	}
	board[pos_y][pos_x] = which;

	history[count].x = pos_x;
	history[count].y = pos_y;
}

//-------------------------------------------------
// 手番交代処理
//-------------------------------------------------
void changeTurn(int *which_turn)
{
	*which_turn = (*which_turn == STONE_BLACK) ? STONE_WHITE : STONE_BLACK;
}

//-------------------------------------------------
// 範囲外チェック
//-------------------------------------------------
int checkOutPos(int x, int y)
{
	return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

//-------------------------------------------------
// ゲーム情報初期化
//-------------------------------------------------
void gameInit(int board[][BOARD_SIZE], int *which_turn)
{
	boardInit(board);
	*which_turn = STONE_BLACK;
}

//-------------------------------------------------
// 盤面初期化
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
// 盤面出力
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
			case STONE_SPACE: printf("・"); break;
			case STONE_BLACK: printf("Ｘ"); break;
			case STONE_WHITE: printf("Ｏ"); break;
			}
		}
		puts("");
	}
	puts("");
}

//-------------------------------------------------
// ゲーム終了処理
//-------------------------------------------------
int gameEndProcess(int board[][BOARD_SIZE])
{
	int i, j, len_flag;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == STONE_SPACE) { continue; }
			if (lenCheck(board, j, i)) {
				printf("五連箇所:%d %d\n", j, i);
				printf("%sの勝ちです。\n", (board[i][j] == STONE_BLACK) ? "Ｘ" : "Ｏ");
				/*
				fprintf( stderr, "%sの勝ちです。\n", (board[i][j] == STONE_BLACK) ? "Ｘ" : "Ｏ");
				*/
				return 1;
			}
		}
	}
	return 0;
}


//-------------------------------------------------
// 5連確認
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
// ランダムに手を打つ戦略
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
// 人間（つまり手打ち入力）
//------------------------------------------------
void keyboardInput(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	scanf("%d %d", pos_x, pos_y);

	return;
}