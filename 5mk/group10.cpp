#include "groups.h"


//=======================================================
// Group 10
//=======================================================

void strategy10(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int score[BOARD_SIZE][BOARD_SIZE]; /* �]���l�L���p�̔z�� */
	int spacex[BOARD_SIZE], spacey[BOARD_SIZE];	
	
	int mystone, yourstone;
	
	int isi = -10000;
	
	int i, j, k;
	
	int me, you, nix;  /* nix means nothing */	
	
	int win = 15000, tobi = 1000, sanren = 500, three = 100, two = 10;
	int win2 = 12000, tobi2 = 800, sanren2 = 300, three2 = 80, two2 =10;
	
	int max;
	int setx[128], sety[128];

	for (i = 0; i < BOARD_SIZE; i++) { /* �Ֆʂ̕]���l�̃��Z�b�g */
		for (j = 0; j < BOARD_SIZE; j++) {
			score[j][i] = 0;
		}
	}
	
	for (i = 0; i < BOARD_SIZE; i++) { /* ���łɐ΂̒u���Ă���Ƃ���̔��� */
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[j][i] == STONE_BLACK || board[j][i] == STONE_WHITE) {
				score[j][i] += isi;
			}
		}
	}
	
//-------------------------------------------------------
// ���E���̔��f
//-------------------------------------------------------
	
	if (count % 2 == 0) {
		mystone = STONE_BLACK;
		yourstone = STONE_WHITE;
	}
	else if (count % 2 != 0) {
		mystone = STONE_WHITE;
		yourstone = STONE_BLACK;
	}

//-------------------------------------------------------
// �]���J�n
//-------------------------------------------------------

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == mystone) {
				score[i-1][j-1] += 1;
				score[i-1][j] += 1;
				score[i-1][j+1] += 1;
				score[i][j-1] += 1;
				score[i][j+1] += 1;
				score[i+1][j-1] += 1;
				score[i+1][j] += 1;
				score[i+1][j+1] += 1;
			}
		}
	}
	
	if (count == 0) { /* ���̏��� */
		score[4][4] += 10;
		score[4][5] += 10;
		score[5][4] += 10;
		score[5][5] += 10;
	}
	
	if (count == 2 || count == 4) { /* ����2��ځA3��� */
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == mystone) {
					score[i-1][j-1] += 30;
					score[i-1][j] += 20;
					score[i-1][j+1] += 30;
					score[i][j-1] += 20;
					score[i][j+1] += 20;
					score[i+1][j-1] += 30;
					score[i+1][j] += 20;
					score[i+1][j+1] += 30;
				}
				if (board[i][j] == yourstone) {
					score[i-2][j-2] -= 30;
					score[i-2][j+2] -= 30;
					score[i+2][j-2] -= 30;
					score[i+2][j+2] -= 30;
				}
			}
		}
	}
	
	if (count == 1) { /* ����1��� */
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == yourstone) {
					score[i-1][j-1] += 10;
					score[i-1][j] += 30;
					score[i-1][j+1] += 10;
					score[i][j-1] += 30;
					score[i][j+1] += 30;
					score[i+1][j-1] += 10;
					score[i+1][j] += 30;
					score[i+1][j+1] += 10;
				}
			}
		}
	}
	
	if (count > 2) {
		
		for (i = 0; i < BOARD_SIZE; i++) { /* �c�����ɑ��� */
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) { /* 5������ */
					
					if (board[i][k] == mystone) { /* �����̐� */
						me++;
					}
					if (board[i][k] == yourstone) { /* ����̐� */
						you++;
					}
					if (board[i][k] == STONE_SPACE) { /* �󔒂��J�E���g���Ĉʒu���L�� */
						spacex[nix] = i;
						spacey[nix] = k;
						nix++;
					}
				}
			
				if (me == 4 && nix == 1) { /* �l�A�i���̎�ŏ����j */
					score[spacex[0]][spacey[0]] += win;
				}
				
				if (me == 3 && nix == 2) { /* �O�A�i�l�A�𐶐��j */
					if (board[i][j] == STONE_SPACE && board[i][j + 4] == STONE_SPACE) {
						score[i][j] += sanren;
						score[i][j + 4] += sanren;
					}
					else { /* �O�A�ł͂Ȃ�3�� */
						score[spacex[0]][spacey[0]] += three;
						score[spacex[1]][spacey[1]] += three;
					}
				}
				
				if (me == 2 && nix == 3) {
					score[spacex[0]][spacey[0]] += two;
					score[spacex[1]][spacey[1]] += two;
					score[spacex[2]][spacey[2]] += two;
				}
				
				if (you == 4 && nix == 1) {
					score[spacex[0]][spacey[0]] += win2;
				}
				
				if (you == 3 && nix == 2) {
					if (board[i][j] == STONE_SPACE && board[i][j + 4] == STONE_SPACE) {
						score[i][j] += sanren2;
						score[i][j + 4] += sanren2;
					}
					else {
						score[spacex[0]][spacey[0]] += three2;
						score[spacex[1]][spacey[1]] += three2;
					}
				}
				
				if (you == 2 && nix == 3) {
					score[spacex[0]][spacey[0]] += two2;
					score[spacex[1]][spacey[1]] += two2;
					score[spacex[2]][spacey[2]] += two2;
				}
			}
			
			for (j = 0; j < BOARD_SIZE - 5; j++) { /* ��юO�A�̕]�� */
					
				me = 0;
				you = 0;
				nix = 0;
					
				for (k = j; k < j + 6; k++) {
						
					if (board[i][k] == mystone) {
						me++;
					}
					if (board[i][k] == yourstone) {
						you++;
					}
					if (board[i][k] == STONE_SPACE) {
						spacex[nix] = i;
						spacey[nix] = k;
						nix++;
					}
				}
					
				if (me == 3 && nix == 3) {
					if (board[i][k] == STONE_SPACE && board[i][k + 5] == STONE_SPACE) { /* ��юO�A�Ȃ� */
						score[spacex[1]][spacey[1]] += tobi; /* �l�A�����ʒu�ɉ��_ */
					}
					else {
						score[spacex[0]][spacey[0]] += 3;
						score[spacex[1]][spacey[1]] += 3;
						score[spacex[2]][spacey[2]] += 3;
					}
				}
			}
		}
		
		for (i = 0; i < BOARD_SIZE; i++) { /* �������ɑ��� */
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) { /* 5������ */
					
					if (board[k][i] == mystone) { /* �����̐� */
						me++;
					}
					if (board[k][i] == yourstone) { /* ����̐� */
						you++;
					}
					if (board[k][i] == STONE_SPACE) { /* �󔒂��J�E���g���Ĉʒu���L�� */
						spacex[nix] = k;
						spacey[nix] = i;
						nix++;
					}
				}
				
				if (me == 4 && nix == 1) { /* �l�A�i���̎�ŏ����j */
					score[spacex[0]][spacey[0]] += win;
				}
				
				if (me == 3 && nix == 2) { /* �O�A�i�l�A�𐶐��j */
					if (board[j][i] == STONE_SPACE && board[j + 4][i] == STONE_SPACE) {
						score[j][i] += sanren;
						score[j + 4][i] += sanren;
					}
					else { /* �O�A�ł͂Ȃ�3�� */
						score[spacex[0]][spacey[0]] += three;
						score[spacex[1]][spacey[1]] += three;
					}
				}
				
				if (me == 2 && nix == 3) {
					score[spacex[0]][spacey[0]] += two;
					score[spacex[1]][spacey[1]] += two;
					score[spacex[2]][spacey[2]] += two;
				}
				
				if (you == 4 && nix == 1) {
					score[spacex[0]][spacey[0]] += win2;
				}
				
				if (you == 3 && nix == 2) {
					if (board[j][i] == STONE_SPACE && board[j + 4][i] == STONE_SPACE) {
						score[j][i] += sanren2;
						score[j + 4][i] += sanren2;
					}
					else {
						score[spacex[0]][spacey[0]] += three2;
						score[spacex[1]][spacey[1]] += three2;
					}
				}
				
				if (you == 2 && nix == 3) {
					score[spacex[0]][spacey[0]] += two2;
					score[spacex[1]][spacey[1]] += two2;
					score[spacex[2]][spacey[2]] += two2;
				}
			}
			for (j = 0; j < BOARD_SIZE - 5; j++) { /* ��юO�A�̕]�� */
					
				me = 0;
				you = 0;
				nix = 0;
					
				for (k = j; k < j + 6; k++) {
						
					if (board[k][i] == mystone) {
						me++;
					}
					if (board[k][i] == yourstone) {
						you++;
					}
					if (board[k][i] == STONE_SPACE) {
						spacex[nix] = k;
						spacey[nix] = i;
						nix++;
					}
				}
					
				if (me == 3 && nix == 3) {
					if (board[k][i] == STONE_SPACE && board[k + 5][i] == STONE_SPACE) { /* ��юO�A�Ȃ� */
						score[spacex[1]][spacey[1]] += tobi; /* �l�A�����ʒu�ɉ��_ */
					}
					else {
						score[spacex[0]][spacey[0]] += 3;
						score[spacex[1]][spacey[1]] += 3;
						score[spacex[2]][spacey[2]] += 3;
					}
				}
			}
		}
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* �E�΂߉����������ɑ��� */
			
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) {
					
					if (i + k < BOARD_SIZE) {
						
						if (board[i + k][k] == mystone) {
							me++;
						}
						if (board[i + k][k] == yourstone) {
							you++;
						}
						if (board[i + k][k] == STONE_SPACE) {
							spacex[nix] = i + k;
							spacey[nix] = k;
							nix++;
						}
					}
					
					if (me == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win;
					}
					
					if (me == 3 && nix == 2) {
						if (spacex[0] == i + j && spacex[1] == i + j + 4) {
							score[spacex[0]][spacey[0]] += sanren;
							score[spacex[1]][spacey[1]] += sanren;
						}
						else {
							score[spacex[0]][spacey[0]] += three;
							score[spacex[0]][spacey[0]] += three;
						}
					}
					
					if (me == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two;
						score[spacex[1]][spacey[1]] += two;
						score[spacex[2]][spacey[2]] += two;
					}
					
					if (you == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win2;
					}
					
					if (you == 3 && nix == 2) {
						if (spacex[0] == i + j && spacex[1] == i + j + 4) {
							score[spacex[0]][spacey[0]] += sanren2;
							score[spacex[1]][spacey[1]] += sanren2;
						}
						else {
							score[spacex[0]][spacey[0]] += three2;
							score[spacex[0]][spacey[0]] += three2;
						}
					}
					
					if (you == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two2;
						score[spacex[1]][spacey[1]] += two2;
						score[spacex[2]][spacey[2]] += two2;
					}
				}
			}
			for (j = 0; j < BOARD_SIZE - 5; j++) {
					
				me = 0;
				you = 0;
				nix = 0;

				for (k = j; k < j + 6; k++) {
					
					if (i + k < BOARD_SIZE) {
						
						if (board[i + k][k] == mystone) {
							me++;
						}
						if (board[i + k][k] == yourstone) {
							you++;
						}
						if (board[i + k][k] == STONE_SPACE) {
							spacex[nix] = i + k;
							spacey[nix] = k;
							nix++;
						}
					}
						
					if (me == 3 && nix == 3) {
								
						if (spacex[0] == i + j && spacex[2] == i + j + 5) {
							score[spacex[1]][spacey[1]] += tobi;
						}
						else {
							score[spacex[0]][spacey[0]] += 3;
							score[spacex[1]][spacey[1]] += 3;
							score[spacex[2]][spacey[2]] += 3;
						}
					}
				}
			}
		}
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* �E�΂߉��������c�ɑ��� */
			
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) {
					
					if (i + k < BOARD_SIZE) {
						
						if (board[k][i + k] == mystone) {
							me++;
						}
						if (board[k][i + k] == yourstone) {
							you++;
						}
						if (board[k][i + k] == STONE_SPACE) {
							spacex[nix] = k;
							spacey[nix] = i + k;
							nix++;
						}
					}
				
					
					if (me == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win;
					}
					
					if (me == 3 && nix == 2) {
						if (spacey[0] == i + j && spacey[1] == i + j + 4) {
							score[spacex[0]][spacey[0]] += sanren;
							score[spacex[1]][spacey[1]] += sanren;
						}
						else {
							score[spacex[0]][spacey[0]] += three;
							score[spacex[0]][spacey[0]] += three;
						}
					}
					
					if (me == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two;
						score[spacex[1]][spacey[1]] += two;
						score[spacex[2]][spacey[2]] += two;
					}
					
					if (you == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win2;
					}
					
					if (you == 3 && nix == 2) {
						if (spacey[0] == i + j && spacey[1] == i + j + 4) {
							score[spacex[0]][spacey[0]] += sanren2;
							score[spacex[1]][spacey[1]] += sanren2;
						}
						else {
							score[spacex[0]][spacey[0]] += three2;
							score[spacex[0]][spacey[0]] += three2;
						}
					}
					
					if (you == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two2;
						score[spacex[1]][spacey[1]] += two2;
						score[spacex[2]][spacey[2]] += two2;
					}
				}
			}
			for (j = 0; j < BOARD_SIZE - 5; j++) {
					
				me = 0;
				you = 0;
				nix = 0;
					
				for (k = j; k < j + 6; k++) {
					
					if (i + k < BOARD_SIZE) {
						
						if (board[k][i + k] == mystone) {
							me++;
						}
						if (board[k][i + k] == yourstone) {
							you++;
						}
						if (board[k][i + k] == STONE_SPACE) {
							spacex[nix] = k;
							spacey[nix] = i + k;
							nix++;
						}
					}
				}
						
				if (me == 3 && nix == 3) {
					if (spacey[0] == i + j && spacey[2] == i + j + 5) {
						score[spacex[1]][spacey[1]] += tobi;
					}
					else {
						score[spacex[0]][spacey[0]] += 3;
						score[spacex[1]][spacey[1]] += 3;
						score[spacex[2]][spacey[2]] += 3;
					}
				}
			}
		}
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* ���΂߉����������ɑ��� */
			
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) {
					
					if (i + k < BOARD_SIZE) {
						
						if (board[BOARD_SIZE - 1 - i - k][k] == mystone) {
							me++;
						}
						if (board[BOARD_SIZE - 1 - i - k][k] == yourstone) {
							you++;
						}
						if (board[BOARD_SIZE - 1 - i - k][k] == STONE_SPACE) {
							spacex[nix] = BOARD_SIZE - 1 - i - k;
							spacey[nix] = k;
							nix++;
						}
					}
					
					if (me == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win;
					}
					
					if (me == 3 && nix == 2) {
						if (spacex[0] == BOARD_SIZE - 1 - i - j && spacex[1] == BOARD_SIZE - 5 - i - j) {
							score[spacex[0]][spacey[0]] += sanren;
							score[spacex[1]][spacey[1]] += sanren;
						}
						else {
							score[spacex[0]][spacey[0]] += three;
							score[spacex[0]][spacey[0]] += three;
						}
					}
					
					if (me == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two;
						score[spacex[1]][spacey[1]] += two;
						score[spacex[2]][spacey[2]] += two;
					}
					
					if (you == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win2;
					}
					
					if (you == 3 && nix == 2) {
						if (spacex[0] == BOARD_SIZE - 1 - i - j && spacex[1] == BOARD_SIZE - 5 - i - j) {
							score[spacex[0]][spacey[0]] += sanren2;
							score[spacex[1]][spacey[1]] += sanren2;
						}
						else {
							score[spacex[0]][spacey[0]] += three2;
							score[spacex[0]][spacey[0]] += three2;
						}
					}
					
					if (you == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two2;
						score[spacex[1]][spacey[1]] += two2;
						score[spacex[2]][spacey[2]] += two2;
					}
				}
			}
			for (j = 0; j < BOARD_SIZE - 5; j++) {
					
				me = 0;
				you = 0;
				nix = 0;
				
					
				for (k = j; k < j + 6; k++) {
					
					if (i + k < BOARD_SIZE) {
						
						if (board[i + k][k] == mystone) {
							me++;
						}
						if (board[i + k][k] == yourstone) {
							you++;
						}
						if (board[i + k][k] == STONE_SPACE) {
							spacex[nix] = i + k;
							spacey[nix] = k;
							nix++;
						}
					}
				}
						
				if (me == 3 && nix == 3) {
					if (i + k < BOARD_SIZE) {
						if (spacex[0] == BOARD_SIZE - 1 - i - j && spacex[2] == BOARD_SIZE - 6 - i - j) {
							score[spacex[1]][spacey[1]] += tobi;
						}
						else {
							score[spacex[0]][spacey[0]] += 3;
							score[spacex[1]][spacey[1]] += 3;
							score[spacex[2]][spacey[2]] += 3;
						}
					}
				}
			}
		}
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* ���΂߉��������c�ɑ��� */
			
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) {
					
					if (i + k < BOARD_SIZE) {
						
						if (board[BOARD_SIZE - 1 - k][i + k] == mystone) {
							me++;
						}
						if (board[BOARD_SIZE - 1 - k][i + k] == yourstone) {
							you++;
						}
						if (board[BOARD_SIZE - 1 - k][i + k] == STONE_SPACE) {
							spacex[nix] = BOARD_SIZE - 1 - k;
							spacey[nix] = i + k;
							nix++;
						}
					}
					
					if (me == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win;
					}
					
					if (me == 3 && nix == 2) {
						if (spacex[0] == BOARD_SIZE - 1 - j && spacex[1] == BOARD_SIZE - 5 - j) {
							score[spacex[0]][spacey[0]] += sanren;
							score[spacex[1]][spacey[1]] += sanren;
						}
						else {
							score[spacex[0]][spacey[0]] += three;
							score[spacex[0]][spacey[0]] += three;
						}
					}
					
					if (me == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two;
						score[spacex[1]][spacey[1]] += two;
						score[spacex[2]][spacey[2]] += two;
					}
					
					if (you == 4 && nix == 1) {
						score[spacex[0]][spacey[0]] += win2;
					}
					
					if (you == 3 && nix == 2) {
						if (spacex[0] == BOARD_SIZE - 1 - j && spacex[1] == BOARD_SIZE - 5 - j) {
							score[spacex[0]][spacey[0]] += sanren2;
							score[spacex[1]][spacey[1]] += sanren2;
						}
						else {
							score[spacex[0]][spacey[0]] += three2;
							score[spacex[0]][spacey[0]] += three2;
						}
					}
					
					if (you == 2 && nix == 3) {
						score[spacex[0]][spacey[0]] += two2;
						score[spacex[1]][spacey[1]] += two2;
						score[spacex[2]][spacey[2]] += two2;
					}
				}
			}
			for (j = 0; j < BOARD_SIZE - 5; j++) {
					
				me = 0;
				you = 0;
				nix = 0;

				for (k = j; k < j + 6; k++) {
					if (i + k < BOARD_SIZE) {	
						if (board[BOARD_SIZE - 1 - k][i + k] == mystone) {
							me++;
						}
						if (board[BOARD_SIZE - 1 - k][i + k] == yourstone) {
							you++;
						}
						if (board[BOARD_SIZE - 1 - k][i + k] == STONE_SPACE) {
							spacex[nix] = BOARD_SIZE - 1 - k;
							spacey[nix] = i + k;
							nix++;
						}
					}
				}
				if (me == 3 && nix == 3) {
					if (spacex[0] == BOARD_SIZE - 1 - i - j && spacex[2] == BOARD_SIZE - 6 - i - j) {
						score[spacex[1]][spacey[1]] += tobi;
					}
					else {
						score[spacex[0]][spacey[0]] += 3;
						score[spacex[1]][spacey[1]] += 3;
						score[spacex[2]][spacey[2]] += 3;
					}
				}
			}
		}
	}
	
//-----------------------------------------------------
// �]��
//-----------------------------------------------------
	max = 0;
	k = 0;
	
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (score[j][i] > max) {
				max = score[j][i];
			}
		}
	}
	
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (score[j][i] == max) {
				setx[k] = i;
				sety[k] = j;
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
