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

	
	//��s����
	if (count == 0) { *pos_x = 5; *pos_y = 5; return; }
	
	//����(me)�Ƒ���(opponent)�̊m�F
	if (count % 2 == 0) {
		me = STONE_BLACK;
		opponent = STONE_WHITE;
	}
	else {
		me = STONE_WHITE;
		opponent = STONE_BLACK;
	}
	
	//�_��(point)�̏�����
	for (i = 0; i < BOARD_SIZE; i++) {
		for(j = 0; j < BOARD_SIZE; j++) {
			point[i][j] = 0;
		}
	}

	//�A���`�F�b�N�Ɠ_�����Z
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
						if (board[i+l*dy[k]][j+l*dx[k]] == me					//�i�s�����̃}�X���ʂ̐�or�ՊO�Ȃ�
							|| i+l*dy[k] >= BOARD_SIZE
							|| j+l*dx[k] >= BOARD_SIZE
							|| i+l*dy[k] < 0
							|| j+l*dx[k] < 0)
						{
							if (board[i-dy[k]][j-dx[k]] == STONE_SPACE			//�i�s�����̋t���̃}�X����Ȃ�
								&& i-dy[k] < BOARD_SIZE
								&& j-dx[k] < BOARD_SIZE
								&& i-dy[k] >= 0
								&& j-dx[k] >= 0)
							{
								str14_addpoint(point, j-dx[k], i-dy[k], len, 0);			//�i�s�����̋t���̃}�X�ɉ��_
							}
							
							break;
						}
						else if (board[i+l*dy[k]][j+l*dx[k]] == opponent) {		//�i�s�����̃}�X�������΂Ȃ�
							len++;
							continue;
						}
						else {													//�i�s�����̃}�X����Ȃ�
							if (board[i-dy[k]][j-dx[k]] == STONE_SPACE				//�i�s�����̋t���̃}�X����Ȃ�
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
									str14_addpoint(point, j - dx[k], i - dy[k], len, 1);				//�i�s�����̋t���̃}�X�ɉ��_
									str14_addpoint(point, j + l*dx[k], i + l*dy[k], len, 1);			//�i�s�����̃}�X�ɉ��_
								}
							}
							else {													//�t���̃}�X����łȂ��Ȃ�
								str14_addpoint(point, j+l*dx[k], i+l*dy[k], len, 0);			//�i�s�����̃}�X�ɉ��_
							}
							break;
						}
					}
				}
			
			}
		}
	}





	
	//�ō��_�̃}�X��(choices[]��)�L�^
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

	//�ō��_�̃}�X(choices[])���烉���_���ɑI��
	tmp_d = (double)rand() / ((double)RAND_MAX + 1);
	pos = tmp_d * cnt;
	*pos_x = choices[pos].x;
	*pos_y = choices[pos].y;

	return;
}

void str14_addpoint(int point[][BOARD_SIZE], int x, int y, int len, int empty_flug)		//empty_flug�́A�΂̕��т̗��[����1, �В[����0
{
	if (empty_flug) {			//����
		switch (len) {
			case 1: point[y][x] += 5; break;
			case 2: point[y][x] += 30; break;
			case 3: point[y][x] += 100; break;
			//case 4: point[y][x] += 1000; break;
		}
	}
	else {						//�Ћ�
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

