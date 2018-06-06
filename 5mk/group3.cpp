#include "group3.h"
using namespace Core;

//=======================================================
// Group 3
//=======================================================

Position Strategy3::getNextTurn(){

//	�]���l�̍��v�����߂�
	BoardMap<int> number(1);

//	�����̐΂̐F
	int turn = board->getKind();

//	1��ځi���j
	if(board->getCount() == 0) return {5, 5};

//	1��ځi��U�j
	if(board->getCount() == 1){
		Position chist = board->getHistory(0);
		return {
			chist.x + (chist.x < 5 ? 1 : -1),
			chist.y + (chist.y < 5 ? 1 : -1),
		};
	}

//	�����̗L���ȑI��
//	�Տ�̂��ׂẴ}�X��
	board->doEachEmpty([&](const Position& p) {

	//	�㍶�΂�4�����Ɋւ��ď���
		for(Position p_diff : Directions::half){

			Stone::Number ns_me_or_empty = 1;
			Stone::Number ns_me = 0;
			Stone::Number ns_prev = 0;

		//	��O�̐΂̐F
			Stone::Kind pstone = turn;

		//	�u�����ʒu���狗��4�ȓ���
			for(int di = 1; di <= 4; di++){

				Stone::Kind cstone = board->getStone(p + di * p_diff);

			//	���݂̏ꏊ������A�܂��͕ǂłȂ��ꍇ
				if(cstone == turn || !cstone) ns_me_or_empty++;

			//	���������̐΂�����A�������Տ�ł���΁A
				if(cstone == turn) {
					ns_me++;
			
			//	1�O�ɒ��ׂ���������̐΁A�܂��͒u�����Ƃ�z�肵���΂ł���A���ݒ��ׂĂ���ꏊ����ł����
				} else if(!cstone && pstone == turn){
					ns_prev++;
					break;
				} else {
					break;
				}

			}

			//�]���l�̌v�Z
			if(ns_me > 1) number[p] *= choiceValueForComputer(ns_me, ns_prev, turn);

		}
	});

	//������
	int max = -1111111110;
	int counter = 1;
	int picounter = 0;
	int stonecounter = 0;

	//����̗L���ȑI��
	board->doEachEmpty([&](const Position& p) {

	//	�㍶�΂�4�����Ɋւ��ď���
		for(Position p_diff : Directions::half){

		//	���� 1 �` 4
			for(int di = 1; di <= 4; di++){
				if(board[y+j*dy[i]][x+j*dx[i]] != turn	&&	((y+j*dy[i]) >= 0) && ((y+j*dy[i]) < 10) && ((x+j*dx[i]) >= 0) && ((x+j*dx[i]) < 10))
		stonecounter++;
				if(board[y+j*dy[i]][x+j*dx[i]] != turn && board[y+j*dy[i]][x+j*dx[i]] != 0 && ((y+j*dy[i]) >= 0) && ((y+j*dy[i]) < 10) && ((x+j*dx[i]) >= 0) && ((x+j*dx[i]) < 10))
		counter++;
				else if(board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && ( (board[y+(j-1)*dy[i]][x+(j-1)*dx[i]] != turn && board[y+(j-1)*dy[i]][x+(j-1)*dx[i]] != 0) || (y+(j-1)*dy[i] == y && x+(j-1)*dx[i] == x))){
		picounter++;
		break;
				}
				else
		break;
			}
			if(i % 2 != 0){
				//printf("stonecounter = %d\n", stonecounter);
				if(counter > 1){
		//printf("counter=%d, picounter=%d\n", counter, picounter);
		number[y][x] *= str3_choicevalueforplayer(counter, picounter, turn);
				}
				counter = 1;
				picounter = 0;
				stonecounter = 0;
				if(i == 7){
		if(y == 0 || y == 9 || x == 0 || x == 9)
			number[y][x] -= 5;
		else if(y == 1 || y == 8 || x == 1 || x == 8)
			number[y][x] -= 3;
		//printf("x = %d, y = %d, number = %d\n",x, y, number[y][x]);
		//�]���l����ԑ傫���z��I��
		if(max < number[y][x]){
			max = number[y][x];
			pos = 
		}
				}
			}
		}
	});
}


int str3_choicevalueforcomputer(int c, int pi,int turn){
	if( c>=5 )
		return 10000000;
	else if( c==4 && pi==2 ){
		if(turn == 1)
			return 500000;
		else
			return 400000;
	}
	else if( c==4 && pi==1 ){
		if(turn == 1)
			return 12;
		else
			return 10;
	}
	else if( c==3 && pi==2 )
		return 30;
	else if( c==3 && pi==1 ){
		if(turn == 1)
			return 6;
		else
			return 4;
	}
	else if( c==2 && pi==2 ){
		if(turn == 1)
			return 10;
		else
			return 8;
	}
	else
		return 1;
}

int str3_choicevalueforplayer(int c, int pi, int turn){
	if( c>=5 )
		return 5000000;
	else if( c==4 && pi==2 )
		return 30000;
	else if( c==4 && pi==1 ){
		if(turn == 1)
			return 3;
		else
			return 5;
	}
	else if( c==3 && pi==2 ){
		if(turn == 1)
			return 8;
		else
			return 12;
	}
	else if( c==3 && pi==1 )
		return 2;
	else
		return 1;
}
