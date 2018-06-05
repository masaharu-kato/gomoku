#include "groups.h"

//=======================================================
// Group 12
//=======================================================

void strategy12( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );
void str12_thinking(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int self_stone, const int rival_stone, const int count, const position *history);
int str12_eval_firstphase(int x, int y, const int board[][BOARD_SIZE]);
int str12_eval_secondphase(int x, int y, const int board[][BOARD_SIZE]);

//-------------------------------------------------
// �����A���S���Y���N��
//-------------------------------------------------
void strategy12(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int check;
	int phase = 0;

	check = 0;
	while( check == 0 )
	{
		//�����`�F�b�N
		if(count % 2 == 0){
			phase = STONE_BLACK;
		}else if(count % 2 == 1){
			phase = STONE_WHITE;
		}

		if(phase == STONE_BLACK){
			str12_thinking(board, pos_x, pos_y, STONE_BLACK, STONE_WHITE, count, history);
		}else{
			str12_thinking(board, pos_x, pos_y, STONE_WHITE, STONE_BLACK, count, history);
		}
		
		if( board[*pos_y][*pos_x] == STONE_SPACE )
			check = 1;
	}
	
	return;
}

//����A���S���Y��
void str12_thinking(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int self_stone, const int rival_stone, const int count, const position *history){

	int eval[BOARD_SIZE][BOARD_SIZE] = {0};
	int max_eval = -1;
	double max_pos_x, max_pos_y;

	//���ځE�O��ڔ���	
	//���ڂ������璆�̃A���S���Y�����N��
	if(count == 0){
		*pos_x = 5;
		*pos_y = 5;
		return;
	//�O��ڂ������璆�̃A���S���Y�����N��
	}else if(count == 2){
		
		//�Y���E�Ԍ�����

		printf("%d\n", history[count - 2].x + history[count - 2].y);
		printf("%d\n", history[count - 1].x + history[count - 1].y);
		
		if((history[count - 2].x + history[count - 2].y - history[count - 1].x + history[count - 1].y) % 2 == 1 ){
			printf("Kagetsu");
			//�Ԍ�
			*pos_x = history[count - 1].x - (history[count - 2].y - history[count - 1].y);
			*pos_y = history[count - 1].y - (history[count - 2].x - history[count - 1].x);
			
			return;
		}else if((history[count - 2].x + history[count - 2].y - history[count - 1].x + history[count - 1].y) % 2 == 0){
			printf("Fugetsu\n");
			//�Y��
			*pos_x = history[count - 2].x - (history[count - 2].x - history[count - 1].x);
			*pos_y = history[count - 2].y + (history[count - 2].y - history[count - 1].y);
			
			return;
		}

	//��A�O��ڂłȂ���Έȉ��̃v���O�������N��
	}else{
	//�Ֆʏ�̂��ׂĂ̋󂫃X�y�[�X�̕]��������//
		for(int i = 0; i < BOARD_SIZE; i++){
			for(int j = 0; j < BOARD_SIZE; j++){
				if(board[i][j] == STONE_SPACE){
					if(self_stone == STONE_BLACK){
						eval[i][j] += str12_eval_firstphase(i, j, board);
					}else{
						eval[i][j] += str12_eval_secondphase(i, j, board);
					}
				}else{
					eval[i][j] = -1;
				}
			}
		}

	//�ő�]���l�̌��o
		for(int i = 0; i < BOARD_SIZE; i++){
			for(int j = 0; j < BOARD_SIZE; j++ ){
				if(max_eval < eval[i][j]){
					max_eval = eval[i][j];
					printf("new max_eval => %d%d %d\n", i, j, max_eval);
					max_pos_x = j;
					max_pos_y = i;
				}else if(max_eval == eval[i][j]){
					if((max_pos_x + max_pos_y) - (10 - i + j) < 0 ){
						max_eval = eval[i][j];
						printf("new max_eval => %d%d %d\n", j, i, max_eval);
						max_pos_x = j;
						max_pos_y = i;
					}
				}
			}
		}
		
		printf("%d\n", eval[3][1]);

		*pos_x = max_pos_x;
		*pos_y = max_pos_y;

		return;
	}
	
	return;

}
//����A���S���Y���I���

//�A���΃`�F�b�N���
int str12_eval_firstphase(int x, int y, const int board[][BOARD_SIZE]){
	
	int seq[4][9];
	int count[4] = {0}, count_self = 0;
	int distance = 0;
	int seq_enemy = 0;
	int eval = 0;
	
	//�E��������
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y - 4 + i < 0 || y - 4 + i > 9){
			seq[0][i] = - 1;
		}else{
			seq[0][i] = board[x - 4 + i][y - 4 + i];
		}
	}

	//�E���オ��
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y + 4 - i < 0 || y + 4 - i > 9){
			seq[1][i] = - 1;
		}else{
			seq[1][i] = board[x - 4 + i][y + 4 - i];
		}
	}

	//���܂�����
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9){
			seq[2][i] = - 1;
		}else{
			seq[2][i] = board[x - 4 + i][y];
		}
	}

	//�c�܂�����
	for(int i = 0; i < 9; i++){
		if(y - 4 + i < 0 || y - 4 + i > 9){
			seq[3][i] = - 1;
		}else{
			seq[3][i] = board[x][y - 4 + i];
		}
	}


	
	//�]���v�Z
	//�W�����]��
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(seq[i][5+j] == STONE_BLACK){
				count[i]++;
				distance += 10 - (5 + j);
			}else if(seq[i][5+j] == STONE_WHITE || seq[i][5+j] == -1){
				break;
			}
		}
		for(int j = 0; j < 4; j++){
			if(seq[i][3-j] == STONE_BLACK){
				count[i]++;
				distance += 3 - j + 1;
			}else if(seq[i][3-j] == STONE_WHITE || seq[i][3-j] == -1){
				break;
			}
		}
		
		if(seq_enemy < count[i]){
			seq_enemy = count[i];
		}
		
		for(int j = 0; j < 4; j++){
			if(seq[i][5+j] == STONE_WHITE){
				count_self++;
			}else if(seq[i][5+j] == STONE_BLACK || seq[i][5+j] == -1){
				break;
			}
		}
		for(int j = 0; j < 4; j++){
			if(seq[i][3-j] == STONE_WHITE){
				count_self++;
			}else if(seq[i][3-j] == STONE_BLACK|| seq[i][3-j] == -1){
				break;
			}
		}
		
		if(count_self > 2){
			eval += 10;
			break;
		}else{
			count_self = 0;
		}
	}
	
	eval += seq_enemy * seq_enemy  + distance;

	return eval;

}

//�A���΃`�F�b�N���
int str12_eval_secondphase(int x, int y, const int board[][BOARD_SIZE]){
	
	int seq[4][9];
	int count[4] = {0}, count_self = 0;
	int distance = 0;
	int seq_enemy = 0;
	int eval = 0;
	
	//�E��������
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y - 4 + i < 0 || y - 4 + i > 9){
			seq[0][i] = - 1;
		}else{
			seq[0][i] = board[x - 4 + i][y - 4 + i];
		}
	}

	//�E���オ��
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y + 4 - i < 0 || y + 4 - i > 9){
			seq[1][i] = - 1;
		}else{
			seq[1][i] = board[x - 4 + i][y + 4 - i];
		}
	}

	//���܂�����
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9){
			seq[2][i] = - 1;
		}else{
			seq[2][i] = board[x - 4 + i][y];
		}
	}

	//�c�܂�����
	for(int i = 0; i < 9; i++){
		if(y - 4 + i < 0 || y - 4 + i > 9){
			seq[3][i] = - 1;
		}else{
			seq[3][i] = board[x][y - 4 + i];
		}
	}
	
	//�]���v�Z
	//�W�����]��
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(seq[i][5+j] == STONE_BLACK){
				count[i]++;
				distance += 10 - (5 + j);
			}else if(seq[i][5+j] == STONE_WHITE || seq[i][5+j] == -1){
				break;
			}
		}
		for(int j = 0; j < 4; j++){
			if(seq[i][3-j] == STONE_BLACK){
				count[i]++;
				distance += 3 - j + 1;
			}else if(seq[i][3-j] == STONE_WHITE || seq[i][3-j] == -1){
				break;
			}
		}
		
		printf("count[%d] = %d\n", i, count[i]);
		if(seq_enemy < count[i]){
			seq_enemy = count[i];
		}
		
		for(int j = 0; j < 4; j++){
			if(seq[i][5+j] == STONE_WHITE){
				count_self++;
			}else if(seq[i][5+j] == STONE_BLACK || seq[i][5+j] == -1){
				break;
			}
		}
		for(int j = 0; j < 4; j++){
			if(seq[i][3-j] == STONE_WHITE){
				count_self++;
			}else if(seq[i][3-j] == STONE_BLACK || seq[i][3-j] == -1){
				break;
			}
		}
		
		if(count_self > 3){
			eval += 10000;
			break;
		}else{
			count_self = 0;
		}
	}
	
	eval += seq_enemy * seq_enemy  + distance;

	return eval;

}
