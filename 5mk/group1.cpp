#include "groups.h"

//=======================================================
// Group 1
//=======================================================

int str1_alphabeta(int board[][BOARD_SIZE], int depth, int which, int count, int alpha, int beta);
int str1_eval(int board[][BOARD_SIZE], int which, int count);

void strategy1( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
	int val;
	int depth = 3, alpha = -9999, beta = 9999;
	static int loop;
	double tmp_d;
	int check;
	int tmp_board[BOARD_SIZE][BOARD_SIZE];

	int which = count % 2 == 0 ? STONE_BLACK : STONE_WHITE;
	for( int i = 0; i < BOARD_SIZE; ++i ){
		for( int j = 0; j < BOARD_SIZE; ++j )
			tmp_board[i][j] = board[i][j];
	}
	

	if (loop == 3 || loop == 4)
	{
		check = 0;
		while (check == 0)
		{
			tmp_d = (double)rand() / ((double)RAND_MAX + 1);
			*pos_x = (int)(tmp_d * 10.0);

			tmp_d = (double)rand() / ((double)RAND_MAX + 1);
			*pos_y = (int)(tmp_d * 10.0);

			if (board[*pos_y][*pos_x] == STONE_SPACE && *pos_y <= 5 && *pos_x <= 5 && *pos_y >= 4 && *pos_x >= 4)
				check = 1;
		}
	} else
	{
		val = str1_alphabeta(tmp_board, depth, which, count, alpha, beta);
		*pos_x = val % BOARD_SIZE;
		*pos_y = val / BOARD_SIZE;
	}

	loop++;
}

int str1_alphabeta(int board[][BOARD_SIZE], int depth, int which, int count, int alpha, int beta){
	int i, j;
	int best_x, best_y, val;
	if (depth == 0){
		return str1_eval(board, which, count);
	}

	if (depth % 2 == 1){
		alpha = -9999;
	}
	else{
		beta = 9999;
	}


	for (i = 0; i<BOARD_SIZE; i++){
		for (j = 0; j<BOARD_SIZE; j++){
			if (board[i][j] == STONE_SPACE){
				board[i][j] = which;
				changeTurn(&which);
				val = str1_alphabeta(board, depth - 1, which, count, alpha, beta);
				board[i][j] = STONE_SPACE;
				if (depth % 2 == 1){
					if (val >= alpha){
						alpha = val;
						best_x = j;
						best_y = i;
					}
					if (alpha > beta){
						return alpha;
					}
				}
				else{
					if (val <= beta){
						beta = val;
						best_x = j;
						best_y = i;
					}
					if (alpha > beta){
						return beta;
					}
				}
			}
		}
	}
	if (depth == 3){
		return best_x + best_y*BOARD_SIZE;
	}
	else if (depth % 2 == 1){
		return alpha;
	}
	else{
		return beta;
	}
}


int str1_eval(int board[][BOARD_SIZE], int which, int count){
	int three_cnt = 0, four_cnt = 0, five_cnt = 0, stone_cnt[3] = { 0, 0, 0 };
	int i, j, k, l;
	if (count < 4){
		if (count % 2 == 1){
			if (board[4][4] == which || board[4][5] == which){
				return 10000;
			}
		}
		else if (count % 2 == 0){
			if (board[5][4] == which || board[5][5] == which){
				return 10000;
			}
		}
	}

	else{
		for (i = 0; i < BOARD_SIZE; i++){
			for (j = 0; j < BOARD_SIZE; j++){
				if (i <= BOARD_SIZE / 2){
					if (j <= BOARD_SIZE / 2){
						if (board[i][j] == which){
							for (k = 1; k < 5; k++){
								if (board[i][j + k] == which){
									stone_cnt[0]++;
								}
								if (board[i + k][j] == which){
									stone_cnt[1]++;
								}
								if (board[i + k][j + k] == which){
									stone_cnt[2]++;
								}
							}
							if (stone_cnt[0] == 3 || stone_cnt[1] == 3 || stone_cnt[2] == 3){
								three_cnt++;
							}
							if (stone_cnt[0] == 4 || stone_cnt[1] == 4 || stone_cnt[2] == 4){
								four_cnt++;
							}
							if (stone_cnt[0] == 5 || stone_cnt[2] == 5 || stone_cnt[2] == 5){
								five_cnt++;
							}
						}
						for (l = 0; l < 3; l++){
							stone_cnt[l] = 0;
						}
					}
					else{
						if (board[i][j] == which){
							for (k = 1; k < 5; k++){
								if (board[i][j - k] == which){
									stone_cnt[0]++;
								}
								if (board[i + k][j] == which){
									stone_cnt[1]++;
								}
								if (board[i + k][j - k] == which){
									stone_cnt[2]++;
								}
							}
							if (stone_cnt[0] == 3 || stone_cnt[1] == 3 || stone_cnt[2] == 3){
								three_cnt++;
							}
							if (stone_cnt[0] == 4 || stone_cnt[1] == 4 || stone_cnt[2] == 4){
								four_cnt++;
							}
							if (stone_cnt[0] == 5 || stone_cnt[2] == 5 || stone_cnt[2] == 5){
								five_cnt++;
							}
						}
						for (l = 0; l < 3; l++){
							stone_cnt[l] = 0;
						}
					}
				}
				else{
					if (j <= BOARD_SIZE / 2){	
						if (board[i][j] == which){
							for (k = 1; k < 5; k++){
								if (board[i][j + k] == which){
									stone_cnt[0]++;
								}
								if (board[i - k][j] == which){
									stone_cnt[1]++;
								}
								if (board[i - k][j + k] == which){
									stone_cnt[2]++;
								}
							}
							if (stone_cnt[0] == 3 || stone_cnt[1] == 3 || stone_cnt[2] == 3){
								three_cnt++;
							}
							if (stone_cnt[0] == 4 || stone_cnt[1] == 4 || stone_cnt[2] == 4){
								four_cnt++;
							}
							if (stone_cnt[0] == 5 || stone_cnt[2] == 5 || stone_cnt[2] == 5){
								five_cnt++;
							}
						}
						for (l = 0; l < 3; l++){
							stone_cnt[l] = 0;
						}
					}
					else{	
						if (board[i][j] == which){
							for (k = 1; k < 5; k++){
								if (board[i][j - k] == which){
									stone_cnt[0]++;
								}
								if (board[i - k][j] == which){
									stone_cnt[1]++;
								}
								if (board[i - k][j - k] == which){
									stone_cnt[2]++;
								}
							}
							if (stone_cnt[0] == 3 || stone_cnt[1] == 3 || stone_cnt[2] == 3){
								three_cnt++;
							}
							if (stone_cnt[0] == 4 || stone_cnt[1] == 4 || stone_cnt[2] == 4){
								four_cnt++;
							}
							if (stone_cnt[0] == 5 || stone_cnt[2] == 5 || stone_cnt[2] == 5){
								five_cnt++;
							}
						}
						for (l = 0; l < 3; l++){
							stone_cnt[l] = 0;
						}
					}
				}
			}
		}
		if (three_cnt == 0 && four_cnt == 0 && five_cnt == 0){
			return stone_cnt[0];
		}
		else{
			return three_cnt + 1000 * four_cnt + 50000 * five_cnt;
		}
	}
}
