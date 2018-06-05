#include "groups.h"


//=======================================================
// Group 9
//=======================================================

void strategy9(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);
void str9_len4check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag);
void str9_tobi3_1check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag);
void str9_tobi2_2check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag);
void str9_tobi3check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag);
void str9_sansanA_check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag, int color, int color2);
void str9_sansanB_check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag, int color);
void str9_len3check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag);

void strategy9( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
	int i, j;
	int color, color2;
	int pos_flag = 0;
	int score[BOARD_SIZE][BOARD_SIZE] = {0};
	double tmp_d;
	int check;
	int tateyoko_check;

	printf( "Strategy 9\t" );

	//先手の場合の１手目
	if (count == 0) {
		*pos_x = 5;
		*pos_y = 5;
		return;
	}
	//後手の場合の２手目
	else if (count == 1) {
		if (history[count-1].y > 1 && history[count-1].y < BOARD_SIZE - 2 && history[count-1].x > 1 && history[count-1].x < BOARD_SIZE - 2) {
			//相手の左右どちらに置くかランダムに決める
			tmp_d = (double)rand() / ((double)RAND_MAX + 1);
			tateyoko_check = (int)( tmp_d * 2.0 );
			if (tateyoko_check == 0) {
				*pos_x = history[count-1].x - 1;
			}
			else {
				*pos_x = history[count-1].x + 1;
			}
			//相手の上下どちらに置くかランダムに決める
			tmp_d = (double)rand() / ((double)RAND_MAX + 1);
			tateyoko_check = (int)( tmp_d * 2.0 );
			if (tateyoko_check == 0) {
				*pos_y = history[count-1].y - 1;
			}
			else {
				*pos_y = history[count-1].y + 1;
			}
		}
		else {
			*pos_x = 5;
			*pos_y = 5;
		}
		return;
	}
	
	//３手目以降
	else {
		//自分の５連を作る手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
			color = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len4check(board, j, i, pos_x, pos_y, &pos_flag); //4連から作る
					str9_tobi3_1check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(3:1)から作る
					str9_tobi2_2check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(2:2)から作る
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		
		//相手の５連を阻止する手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len4check(board, j, i, pos_x, pos_y, &pos_flag); //4連からの５連を防ぐ
					str9_tobi3_1check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(3:1)の5連を防ぐ
					str9_tobi2_2check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(2:2)の5連を防ぐ
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//自分の３連を確認して、４連（両端フリー）を作る手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len3check(board, j, i, pos_x, pos_y, &pos_flag); //3連から
					str9_tobi3check(board, j, i, pos_x, pos_y, &pos_flag); //飛び三から
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//相手の３連を確認して、４連（両端フリー）を阻止する手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
			color = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len3check(board, j, i, pos_x, pos_y, &pos_flag); //3連からの4連を防ぐ
					str9_tobi3check(board, j, i, pos_x, pos_y, &pos_flag); //飛び３からの4連を防ぐ
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//三三、三四を作る
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
			color2 = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_BLACK;
			color2 = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE) {
					str9_sansanA_check(board, j, i, pos_x, pos_y, &pos_flag, color, color2); 
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//三三、三四を阻止する
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
			color2 = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_WHITE;
			color2 = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE) {
					str9_sansanA_check(board, j, i, pos_x, pos_y, &pos_flag, color, color2);
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//三三（制限なし）を作る
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE) {
					str9_sansanB_check(board, j, i, pos_x, pos_y, &pos_flag, color);
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//三三（制限なし）を阻止する
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE) {
					str9_sansanB_check(board, j, i, pos_x, pos_y, &pos_flag, color);
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//フリーの２連を作る(より多くのぼっちを救う)
		int max = 0, max_i, max_j;
		int botti_flag = 0; //ぼっちがいるかいないか
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
			color2 = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
			color = STONE_BLACK;
			color2 = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					//左上と右下
					if (board[i-1][j-1] == STONE_SPACE 
						&& board[i+1][j+1] == STONE_SPACE 
						&& i - 1 >= 0 
						&& i + 1 < BOARD_SIZE 
						&& j - 1 >= 0 
						&& j + 1 < BOARD_SIZE ) 
						{
						if (board[i-2][j-2] != color2 && board[i+2][j+2] != color2) {
							score[i-1][j-1] += 10;
							score[i+1][j+1] += 10;
							botti_flag = 1;
						}
					}
					//右上と左下
					else if (board[i-1][j+1] == STONE_SPACE 
						&& board[i+1][j-1] == STONE_SPACE 
						&& i - 1 >= 0 
						&& i + 1 < BOARD_SIZE 
						&& j - 1 >= 0 
						&& j + 1 < BOARD_SIZE ) 
						{
						if (board[i-2][j+2] != color2 && board[i+2][j-2] != color2) {
							score[i-1][j+1] += 10;
							score[i+1][j-1] += 10;
							botti_flag = 1;
						}
					}
				}
			}
		}
		if (botti_flag == 1) {
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					if (score[i][j] > max) {
						max = score[i][j];
						max_i = i;
						max_j = j;
					}
				}
			}
			*pos_x = max_j;
			*pos_y = max_i;
			return;
		}
	}
	
	//フリーの２連を阻止する
	int max = 0, max_i, max_j;
	int botti_flag = 0; //ぼっちがいるかいないか
	if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
		color = STONE_BLACK;
		color2 = STONE_WHITE;
	}
	else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
		color = STONE_WHITE;
		color2 = STONE_BLACK;
	}
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == color) {
				//左上と右下
				if (board[i-1][j-1] == STONE_SPACE 
					&& board[i+1][j+1] == STONE_SPACE 
					&& i - 1 >= 0 
					&& i + 1 < BOARD_SIZE 
					&& j - 1 >= 0 
					&& j + 1 < BOARD_SIZE ) 
					{
					if (board[i-2][j-2] != color2 && board[i+2][j+2] != color2) {
						score[i-1][j-1] += 10;
						score[i+1][j+1] += 10;
						botti_flag = 1;
					}
				}
				//右上と左下
				else if (board[i-1][j+1] == STONE_SPACE 
					&& board[i+1][j-1] == STONE_SPACE 
					&& i - 1 >= 0 
					&& i + 1 < BOARD_SIZE 
					&& j - 1 >= 0 
					&& j + 1 < BOARD_SIZE ) 
					{
					if (board[i-2][j+2] != color2 && board[i+2][j-2] != color2) {
						score[i-1][j+1] += 10;
						score[i+1][j-1] += 10;
						botti_flag = 1;
					}
				}
			}
		}
	}
	if (botti_flag == 1) {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (score[i][j] > max) {
					max = score[i][j];
					max_i = i;
					max_j = j;
				}
			}
		}
		*pos_x = max_j;
		*pos_y = max_i;
		return;
	}
	
	//ランダム
	check = 0;
	while( check == 0 )
	{
		tmp_d = (double)rand() / ((double)RAND_MAX + 1);
		*pos_x = (int)( tmp_d * 10.0 );

		tmp_d = (double)rand() / ((double)RAND_MAX + 1);
		*pos_y = (int)( tmp_d * 10.0 );

		if( board[*pos_y][*pos_x] == STONE_SPACE )
			check = 1;
	}
	return;
}

//-------------------------------------------------
// 4連確認
//-------------------------------------------------
void str9_len4check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1, -1};
    int dy[] = { 1, 0, 1, 1 };
    
    for (i = 0; i < 4; i++) {
		len_flag = 1;
        for (j = 1; j <= 3; j++) {
            if (board[y][x] != board[y+j*dy[i]][x+j*dx[i]]
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0)
            {
                len_flag = 0;
                break;
            }
        }
        if (len_flag == 1) {
			//縦４連のとき
			if (i == 0) {
				if (board[y-1][x] == STONE_SPACE && y - 1 >= 0 ) {
					*pos_y = y - 1;
					*pos_x = x;
					*pos_flag = 1;
					return;
				}
				else if (board[y+j*dy[i]][x] == STONE_SPACE && y + j * dy[i] < BOARD_SIZE) {
					*pos_y = y + j * dy[i];
					*pos_x = x;
					*pos_flag = 1;
					return;
				}
			}
			//横４連のとき
			else if (i == 1) {
				if (board[y][x-1] == STONE_SPACE && x - 1 >= 0) {
					*pos_x = x - 1;
					*pos_y = y;
					*pos_flag = 1;
					return;
				}
				else if (board[y][x+j*dx[i]] == STONE_SPACE && x + j * dx[i] < BOARD_SIZE) {
					*pos_x = x + j * dx[i];
					*pos_y = y;
					*pos_flag = 1;
					return;
				}
			}
			//右斜め下４連のとき
			else if (i == 2) {
				if (board[y-1][x-1] == STONE_SPACE && y - 1 >= 0 && x - 1 >= 0) {
					*pos_x = x - 1;
					*pos_y = y - 1;
					*pos_flag = 1;
					return;
				}
				else if (board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && y+j*dy[i] < BOARD_SIZE && x+j*dx[i] < BOARD_SIZE) {
					*pos_x = x + j * dx[i];
					*pos_y = y + j * dy[i];
					*pos_flag = 1;
					return;
				}
			}
			//左ななめ下４連のとき
			else {
				if (board[y-1][x+1] == STONE_SPACE && y - 1 >= 0 && x + 1 < BOARD_SIZE) {
					*pos_x = x + 1;
					*pos_y = y - 1;
					*pos_flag = 1;
					return;
				}
				else if (board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && y+j*dy[i] < BOARD_SIZE && x+j*dx[i] >= 0) {
					*pos_x = x + j * dx[i];
					*pos_y = y + j * dy[i];
					*pos_flag = 1;
					return;
				}
			}
		}
    }
}

//-------------------------------------------------
// 飛び4確認(3:1)
//-------------------------------------------------
void str9_tobi3_1check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag)
{
    int i, j, len_flag, color;
    int dx[] = { 0, 1, 1, -1};
    int dy[] = { 1, 0, 1, 1 };
    
	color = board[y][x];
	
    for (i = 0; i < 4; i++) {
		len_flag = 1;
        for (j = 1; j <= 2; j++) {
            if (board[y][x] != board[y+j*dy[i]][x+j*dx[i]]
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0)
            {
                len_flag = 0;
                break;
            }
        }
        if (len_flag == 1 ) {
			//縦飛び4のとき
			if (i == 0) {
				if (board[y-1][x] == STONE_SPACE && board[y-2][x] == color && y - 1 >= 0) {
					*pos_y = y - 1;
					*pos_x = x;
					*pos_flag = 1;
					return;
				}
				else if (board[y+j*dy[i]][x] == STONE_SPACE && board[y+j*dy[i]+1][x] == color && y + j * dy[i] < BOARD_SIZE) {
					*pos_y = y + j * dy[i];
					*pos_x = x;
					*pos_flag = 1;
					return;
				}
			}
			//横飛び4のとき
			else if (i == 1) {
				if (board[y][x-1] == STONE_SPACE && board[y][x-2] == color && x - 1 >= 0) {
					*pos_x = x - 1;
					*pos_y = y;
					*pos_flag = 1;
					return;
				}
				else if (board[y][x+j*dx[i]] == STONE_SPACE && board[y][x+j*dx[i]+1] == color && x + j * dx[i] < BOARD_SIZE) {
					*pos_x = x + j * dx[i];
					*pos_y = y;
					*pos_flag = 1;
					return;
				}
			}
			//右斜め下飛び4連のとき
			else if (i == 2) {
				if (board[y-1][x-1] == STONE_SPACE && board[y-2][x-2] == color && y - 1 >= 0 && x - 1 >= 0) {
					*pos_x = x - 1;
					*pos_y = y - 1;
					*pos_flag = 1;
					return;
				}
				else if (board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && board[y+j*dy[i]+1][x+j*dx[i]+1] == color && y+j*dy[i] < BOARD_SIZE && x+j*dx[i] < BOARD_SIZE) {
					*pos_x = x + j * dx[i];
					*pos_y = y + j * dy[i];
					*pos_flag = 1;
					return;
				}
			}
			//左ななめ下飛び4連のとき
			else {
				if (board[y-1][x+1] == STONE_SPACE  && board[x+2][y-2] == color && y - 1 >= 0 && x + 1 < BOARD_SIZE) {
					*pos_x = x + 1;
					*pos_y = y - 1;
					*pos_flag = 1;
					return;
				}
				else if (board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && board[y+j*dy[i]+1][x+j*dx[i]-1] == color && y+j*dy[i] < BOARD_SIZE && x+j*dx[i] >= 0) {
					*pos_x = x+j*dx[i];
					*pos_y = y+j*dy[i];
					*pos_flag = 1;
					return;
				}
			}
		}
    }
}

//-------------------------------------------------
// 飛び4確認(2:2)
//-------------------------------------------------
void str9_tobi2_2check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1, -1};
    int dy[] = { 1, 0, 1, 1 };
    
    for (i = 0; i < 4; i++) {
		len_flag = 1;
        for (j = 1; j <= 4; j++) {
            if (board[y][x] != board[y+j*dy[i]][x+j*dx[i]]
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0) 
            {
				if (j != 2 || (j == 2 && board[y+j*dy[i]][x+j*dx[i]] != STONE_SPACE)) {
              		len_flag = 0;
                	break;
                }
            }
        }
        if (len_flag == 1 ) {
			//縦飛び4のとき
			if (i == 0) {
				*pos_x = x;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//横飛び4のとき
			else if (i == 1) {
				*pos_x = x + 2;
				*pos_y = y;
				*pos_flag = 1;
				return;
			}
			//右斜め下飛び4連のとき
			else if (i == 2) {
				*pos_x = x + 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//左ななめ下飛び4連のとき
			else {
				*pos_x = x - 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
		}
    }
}

//とびさんを四連にする
void str9_tobi3check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1, -1};
    int dy[] = { 1, 0, 1, 1 };
    
	//1:2のとき
    for (i = 0; i < 4; i++) {
		len_flag = 1;
        for (j = 1; j <= 3; j++) {
            if (board[y][x] != board[y+j*dy[i]][x+j*dx[i]]
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0) 
            {
				if (j != 1 || (j == 1 && board[y+j*dy[i]][x+j*dx[i]] != STONE_SPACE)) {
              		len_flag = 0;
                	break;
                }
            }
        }
        if (len_flag == 1) {
	        //縦のとき
			if (i == 0 && (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE)) {
				*pos_y = y + 1;
				*pos_x = x;
				*pos_flag = 1;
				return;
			}
			//横のとき
			else if (i == 1 && (board[y][x-1] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE)) {
				*pos_y = y;
				*pos_x = x + 1;
				*pos_flag = 1;
				return;
			}
			//右斜め下のとき
			else if (i == 2 && (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x + 1;
				*pos_y = y + 1;
				*pos_flag = 1;
				return;
			}
			//左ななめ下のとき
			else if (i == 3 && (board[y-1][x+1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x - 1;
				*pos_y = y + 1;
				*pos_flag = 1;
				return;
			}
		}
    }
    
    //2:1のとき
    for (i = 0; i < 4; i++) {
		len_flag = 1;
        for (j = 1; j <= 3; j++) {
            if (board[y][x] != board[y+j*dy[i]][x+j*dx[i]]
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0) 
            {
				if (j != 2 || (j == 2 && board[y+j*dy[i]][x+j*dx[i]] != STONE_SPACE)) {
            		len_flag = 0;
                	break;
                }
            }
        }
        if (len_flag == 1) {
	        //縦のとき
			if (i == 0 && (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE)) {
				*pos_y = y + 2;
				*pos_x = x;
				*pos_flag = 1;
				return;
			}
			//横のとき
			else if (i == 1 && (board[y][x-1] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE)) {
				*pos_y = y;
				*pos_x = x + 2;
				*pos_flag = 1;
				return;
			}
			//右斜め下のとき
			else if (i == 2 && (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x + 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//左ななめ下のとき
			else if (i == 3 && (board[y-1][x+1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x - 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
		}
    }
}

//-------------------------------------------------
// 三三,四三確認
//-------------------------------------------------
void str9_sansanA_check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag, int color, int color2)
{
    int i, j, len_flag, ct = 0;
    int dx[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
    int dy[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
    
    for (i = 0; i < 8; i++) {
		len_flag = 1;
        for (j = 1; j <= 2; j++) {
            if (board[y+j*dy[i]][x+j*dx[i]] != color
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0)
            {
                len_flag = 0;
                break;
            }
        }
        if (len_flag == 1 && board[y+j*dy[i]][x+j*dx[i]] != color2 && board[y-dy[i]][x-dx[i]] != color2) {
			ct++;
		}
    }
    if (ct >= 2) {
		*pos_x = x;
		*pos_y = y;
		*pos_flag = 1;
	}
}

//-------------------------------------------------
// 三三確認（制限なし）
//-------------------------------------------------
void str9_sansanB_check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag, int color)
{
    int i, j, len_flag, ct = 0;
    int dx[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
    int dy[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
    
    for (i = 0; i < 8; i++) {
		len_flag = 1;
        for (j = 1; j <= 2; j++) {
            if (board[y+j*dy[i]][x+j*dx[i]] != color
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0)
            {
                len_flag = 0;
                break;
            }
        }
        if (len_flag == 1) {
			ct++;
		}
    }
    if (ct >= 2) {
		*pos_x = x;
		*pos_y = y;
		*pos_flag = 1;
	}
}


//-------------------------------------------------
// 3連確認
//-------------------------------------------------
void str9_len3check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1, -1};
    int dy[] = { 1, 0, 1, 1 };
    
    
    for (i = 0; i < 4; i++) {
		len_flag = 1;
        for (j = 1, len_flag = 1; j <= 2; j++) {
            if (board[y][x] != board[y+j*dy[i]][x+j*dx[i]]
                || y+j*dy[i] >= BOARD_SIZE
                || x+j*dx[i] >= BOARD_SIZE
            	|| y+j*dy[i] < 0
                || x+j*dx[i] < 0)
            {
                len_flag = 0;
                break;
            }
        }
        if (len_flag == 1 ) {
			//縦3連のとき
			if (i == 0) {
				if (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE && board[y-2][x] == STONE_SPACE && y - 1 >= 0) {
					*pos_y = y - 1;
					*pos_x = x;
					*pos_flag = 1;
					return;
				}
				else if (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE && board[y+j*dy[i]+1][x] == STONE_SPACE && y + j * dy[i] < BOARD_SIZE) {
					*pos_y = y + j * dy[i];
					*pos_x = x;
					*pos_flag = 1;
					return;
				}
			}
			//横3連のとき
			else if (i == 1) {
				if (board[y][x-1] == STONE_SPACE && board[y][x-2] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE &&  x - 1 >= 0) {
					*pos_x = x - 1;
					*pos_y = y;
					*pos_flag = 1;
					return;
				}
				else if (board[y][x-1] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE && board[y][x+j*dx[i]+1] == STONE_SPACE && x + j * dx[i] < BOARD_SIZE) {
					*pos_x = x + j * dx[i];
					*pos_y = y;
					*pos_flag = 1;
					return;
				}
			}
			//右斜め下3連のとき
			else if (i == 2) {
				if (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && board[y-2][x-2] == STONE_SPACE && y - 1 >= 0 && x - 1 >= 0) {
					*pos_x = x - 1;
					*pos_y = y - 1;
					*pos_flag = 1;
					return;
				}
				else if (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && board[y+j*dy[i]+1][x+j*dx[i]+1] == STONE_SPACE && y+j*dy[i] < BOARD_SIZE && x+j*dx[i] < BOARD_SIZE) {
					*pos_x = x + j * dx[i];
					*pos_y = y + j * dy[i];
					*pos_flag = 1;
					return;
				}
			}
			//左ななめ下3連のとき
			else {
				if (board[y-1][x+1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && board[x+2][y-2] == STONE_SPACE && y - 1 >= 0 && x + 1 < BOARD_SIZE) {
					*pos_x = x + 1;
					*pos_y = y - 1;
					*pos_flag = 1;
					return;
				}
				else if (board[y-1][x+1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && board[y+j*dy[i]+1][x+j*dx[i]-1] == STONE_SPACE && y+j*dy[i] < BOARD_SIZE && x+j*dx[i] >= 0) {
					*pos_x = x+j*dx[i];
					*pos_y = y+j*dy[i];
					*pos_flag = 1;
					return;
				}
			}
		}
    }
}

