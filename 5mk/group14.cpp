#include "groups.h"


//=======================================================
// Group 14
//=======================================================

void strategy14( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );
void str14_addpoint(int point[][BOARD_SIZE], int x, int y, int len, int empty_flug);

void strategy14( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
	int i, j, k, l, me, opponent;
	int len, point[BOARD_SIZE][BOARD_SIZE];
	int max = 0, cnt, pos;
	double tmp_d;
	position choices[BOARD_SIZE*BOARD_SIZE];
	int dx[] = { 0, 1, 1, -1 };
	int dy[] = { 1, 0, 1, 1 };

	printf( "Strategy 14\t" );

	
	//先行初手
	if (count == 0) { *pos_x = 5; *pos_y = 5; return; }
	
	//自分(me)と相手(opponent)の確認
	if (count % 2 == 0) {
		me = STONE_BLACK;
		opponent = STONE_WHITE;
	}
	else {
		me = STONE_WHITE;
		opponent = STONE_BLACK;
	}
	
	//点数(point)の初期化
	for (i = 0; i < BOARD_SIZE; i++) {
		for(j = 0; j < BOARD_SIZE; j++) {
			point[i][j] = 0;
		}
	}

	//連続チェックと点数加算
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == opponent) {
				
				for (k = 0; k < 4; k++) {
					if (board[i-dy[k]][j-dx[k]] == opponent
						&& i-dy[k] < BOARD_SIZE
						&& j-dx[k] < BOARD_SIZE
						&& i-dy[k] >= 0
						&& j-dx[k] >= 0)
					{
						continue;
					}
					for (l = 1, len = 1; l <= 4; l++) {
						if (board[i+l*dy[k]][j+l*dx[k]] == me					//進行方向のマスが別の石or盤外なら
							|| i+l*dy[k] >= BOARD_SIZE
							|| j+l*dx[k] >= BOARD_SIZE
							|| i+l*dy[k] < 0
							|| j+l*dx[k] < 0)
						{
							if (board[i-dy[k]][j-dx[k]] == STONE_SPACE			//進行方向の逆側のマスが空なら
								&& i-dy[k] < BOARD_SIZE
								&& j-dx[k] < BOARD_SIZE
								&& i-dy[k] >= 0
								&& j-dx[k] >= 0)
							{
								str14_addpoint(point, j-dx[k], i-dy[k], len, 0);			//進行方向の逆側のマスに加点
							}
							
							break;
						}
						else if (board[i+l*dy[k]][j+l*dx[k]] == opponent) {		//進行方向のマスが同じ石なら
							len++;
							continue;
						}
						else {													//進行方向のマスが空なら
							if (board[i-dy[k]][j-dx[k]] == STONE_SPACE				//進行方向の逆側のマスが空なら
								&& i-dy[k] < BOARD_SIZE
								&& j-dx[k] < BOARD_SIZE
								&& i-dy[k] >= 0
								&& j-dx[k] >= 0)
							{
								if (board[i + (l + 1)*dy[k]][j + (l + 1)*dx[k]] == opponent
									&& board[i + (l + 2)*dy[k]][j + (l + 2)*dx[k]] == opponent
									&& i + (l + 2)*dy[k] < BOARD_SIZE
									&& j + (l + 2)*dx[k] < BOARD_SIZE
									)
								{
									str14_addpoint(point, j + l*dx[k], i + l*dy[k], 6, 0);
								}
								else {
									str14_addpoint(point, j - dx[k], i - dy[k], len, 1);				//進行方向の逆側のマスに加点
									str14_addpoint(point, j + l*dx[k], i + l*dy[k], len, 1);			//進行方向のマスに加点
								}
							}
							else {													//逆側のマスが空でないなら
								str14_addpoint(point, j+l*dx[k], i+l*dy[k], len, 0);			//進行方向のマスに加点
							}
							break;
						}
					}
				}
			
			}
		}
	}





	
	//最高点のマスを(choices[]に)記録
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if ( point[i][j] > max) {
				max = point[i][j];
			}
		}
	}
	cnt = 0;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (point[i][j] == max) {
				choices[cnt].x = j;
				choices[cnt].y = i;
				cnt++;
			}
		}
	}

	//最高点のマス(choices[])からランダムに選択
	tmp_d = (double)rand() / ((double)RAND_MAX + 1);
	pos = tmp_d * cnt;
	*pos_x = choices[pos].x;
	*pos_y = choices[pos].y;

	return;
}

void str14_addpoint(int point[][BOARD_SIZE], int x, int y, int len, int empty_flug)		//empty_flugは、石の並びの両端が空→1, 片端が空→0
{
	if (empty_flug) {			//両空
		switch (len) {
			case 1: point[y][x] += 5; break;
			case 2: point[y][x] += 30; break;
			case 3: point[y][x] += 100; break;
			//case 4: point[y][x] += 1000; break;
		}
	}
	else {						//片空
		switch (len) {
			case 1: point[y][x] += 1; break;
			case 2: point[y][x] += 5; break;
			case 3: point[y][x] += 25; break;
			case 4: point[y][x] += 100; break;
			case 6: point[y][x] += 30; break;
		}
	}
	
	return;
}

