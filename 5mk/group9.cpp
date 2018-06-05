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

	//���̏ꍇ�̂P���
	if (count == 0) {
		*pos_x = 5;
		*pos_y = 5;
		return;
	}
	//���̏ꍇ�̂Q���
	else if (count == 1) {
		if (history[count-1].y > 1 && history[count-1].y < BOARD_SIZE - 2 && history[count-1].x > 1 && history[count-1].x < BOARD_SIZE - 2) {
			//����̍��E�ǂ���ɒu���������_���Ɍ��߂�
			tmp_d = (double)rand() / ((double)RAND_MAX + 1);
			tateyoko_check = (int)( tmp_d * 2.0 );
			if (tateyoko_check == 0) {
				*pos_x = history[count-1].x - 1;
			}
			else {
				*pos_x = history[count-1].x + 1;
			}
			//����̏㉺�ǂ���ɒu���������_���Ɍ��߂�
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
	
	//�R��ڈȍ~
	else {
		//�����̂T�A�������T��
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
			color = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len4check(board, j, i, pos_x, pos_y, &pos_flag); //4�A������
					str9_tobi3_1check(board, j, i, pos_x, pos_y, &pos_flag); //���4(3:1)������
					str9_tobi2_2check(board, j, i, pos_x, pos_y, &pos_flag); //���4(2:2)������
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		
		//����̂T�A��j�~������T��
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len4check(board, j, i, pos_x, pos_y, &pos_flag); //4�A����̂T�A��h��
					str9_tobi3_1check(board, j, i, pos_x, pos_y, &pos_flag); //���4(3:1)��5�A��h��
					str9_tobi2_2check(board, j, i, pos_x, pos_y, &pos_flag); //���4(2:2)��5�A��h��
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//�����̂R�A���m�F���āA�S�A�i���[�t���[�j�������T��
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len3check(board, j, i, pos_x, pos_y, &pos_flag); //3�A����
					str9_tobi3check(board, j, i, pos_x, pos_y, &pos_flag); //��юO����
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//����̂R�A���m�F���āA�S�A�i���[�t���[�j��j�~������T��
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
			color = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len3check(board, j, i, pos_x, pos_y, &pos_flag); //3�A�����4�A��h��
					str9_tobi3check(board, j, i, pos_x, pos_y, &pos_flag); //��тR�����4�A��h��
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//�O�O�A�O�l�����
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
		
		//�O�O�A�O�l��j�~����
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
		
		//�O�O�i�����Ȃ��j�����
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
		
		//�O�O�i�����Ȃ��j��j�~����
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
		
		//�t���[�̂Q�A�����(��葽���̂ڂ������~��)
		int max = 0, max_i, max_j;
		int botti_flag = 0; //�ڂ��������邩���Ȃ���
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
					//����ƉE��
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
					//�E��ƍ���
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
	
	//�t���[�̂Q�A��j�~����
	int max = 0, max_i, max_j;
	int botti_flag = 0; //�ڂ��������邩���Ȃ���
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
				//����ƉE��
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
				//�E��ƍ���
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
	
	//�����_��
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
// 4�A�m�F
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
			//�c�S�A�̂Ƃ�
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
			//���S�A�̂Ƃ�
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
			//�E�΂߉��S�A�̂Ƃ�
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
			//���ȂȂ߉��S�A�̂Ƃ�
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
// ���4�m�F(3:1)
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
			//�c���4�̂Ƃ�
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
			//�����4�̂Ƃ�
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
			//�E�΂߉����4�A�̂Ƃ�
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
			//���ȂȂ߉����4�A�̂Ƃ�
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
// ���4�m�F(2:2)
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
			//�c���4�̂Ƃ�
			if (i == 0) {
				*pos_x = x;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//�����4�̂Ƃ�
			else if (i == 1) {
				*pos_x = x + 2;
				*pos_y = y;
				*pos_flag = 1;
				return;
			}
			//�E�΂߉����4�A�̂Ƃ�
			else if (i == 2) {
				*pos_x = x + 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//���ȂȂ߉����4�A�̂Ƃ�
			else {
				*pos_x = x - 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
		}
    }
}

//�Ƃт�����l�A�ɂ���
void str9_tobi3check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1, -1};
    int dy[] = { 1, 0, 1, 1 };
    
	//1:2�̂Ƃ�
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
	        //�c�̂Ƃ�
			if (i == 0 && (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE)) {
				*pos_y = y + 1;
				*pos_x = x;
				*pos_flag = 1;
				return;
			}
			//���̂Ƃ�
			else if (i == 1 && (board[y][x-1] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE)) {
				*pos_y = y;
				*pos_x = x + 1;
				*pos_flag = 1;
				return;
			}
			//�E�΂߉��̂Ƃ�
			else if (i == 2 && (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x + 1;
				*pos_y = y + 1;
				*pos_flag = 1;
				return;
			}
			//���ȂȂ߉��̂Ƃ�
			else if (i == 3 && (board[y-1][x+1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x - 1;
				*pos_y = y + 1;
				*pos_flag = 1;
				return;
			}
		}
    }
    
    //2:1�̂Ƃ�
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
	        //�c�̂Ƃ�
			if (i == 0 && (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE)) {
				*pos_y = y + 2;
				*pos_x = x;
				*pos_flag = 1;
				return;
			}
			//���̂Ƃ�
			else if (i == 1 && (board[y][x-1] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE)) {
				*pos_y = y;
				*pos_x = x + 2;
				*pos_flag = 1;
				return;
			}
			//�E�΂߉��̂Ƃ�
			else if (i == 2 && (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x + 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//���ȂȂ߉��̂Ƃ�
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
// �O�O,�l�O�m�F
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
// �O�O�m�F�i�����Ȃ��j
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
// 3�A�m�F
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
			//�c3�A�̂Ƃ�
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
			//��3�A�̂Ƃ�
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
			//�E�΂߉�3�A�̂Ƃ�
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
			//���ȂȂ߉�3�A�̂Ƃ�
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

