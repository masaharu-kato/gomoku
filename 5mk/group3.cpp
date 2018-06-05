#include "group3.h"
using namespace Core;

//=======================================================
// Group 3
//=======================================================

int str3_choicevalueforplayer(int c, int pi, int turn);
int str3_choicevalueforcomputer(int c, int pi,int turn);

Position Strategy3::getNextTurn(){

	int counter=1, picounter=0, max=-111111111, ncounter=0, stonecounter=0;
	//�]���l�̍��v�����߂�
	BoardMap<int> number(1);

	int turn = board->getKind();

	//�P��ځi���j
	if(board->getCount() == 0) return {5, 5};

	//1��ځi��U�j
	if(board->getCount() == 1){
		Position chist = board->getHistory(0);
		return {
			chist.x + (chist.x < 5 ? 1 : -1),
			chist.y + (chist.y < 5 ? 1 : -1),
		};
	}

	//�����̗L���ȑI��
	//�Տ�̂��ׂẴ}�X��
	board->doEach([&](const Position& p) {
		if(baoar
	//�㉺���E�΂߂ɑ΂���
	for(Position p_diff : directions){
		//�u�����ʒu���狗��4�ȓ���
		for(j=1; j<=4; j++){
			//���݂̏ꏊ������A�܂��͕ǂłȂ��ꍇ
			if(board->getStone(p+j*p_diff) == turn	||	board[y+j*dy[i]][x+j*dx[i]] == 0){
	stonecounter++;
	//printf("%d, %d, %d\n",x+j*dx[i] , y+j*dy[i], stonecounter);
			}
			//���������̐΂�����A�������Տ�ł���΁A
			if((board[y+j*dy[i]][x+j*dx[i]] == turn) && ((y+j*dy[i]) >= 0) && ((y+j*dy[i]) < 10) && ((x+j*dx[i]) >= 0) && ((x+j*dx[i]) < 10))
	counter++;
			//1�O�ɒ��ׂ���������̐΁A�܂��͒u�����Ƃ�z�肵���΂ł���A���ݒ��ׂĂ���ꏊ����ł����
			else if(board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && (board[y+(j-1)*dy[i]][x+(j-1)*dx[i]] == turn || (y+(j-1)*dy[i] == y && x+(j-1)*dx[i] == x))){
	picounter++;
	break;
			}
			else
	break;
		}
		if(i % 2 != 0){
			//�]���l�̌v�Z
			if(counter > 1)
	number[y][x] *= str3_choicevalueforcomputer(counter, picounter, turn);
			//�J�E���^���Z�b�g
			counter = 1;
			picounter = 0;
			stonecounter = 0;
		}
	}
}
		}
	}
	//������
	max = -1111111110;
	counter = 1;
	picounter = 0;
	stonecounter = 0;
	//����̗L���ȑI��
if(!board->getStone(p)){
	for(i=0; i<8; i++){
		for(j=1; j<=4; j++){
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
}
		}
	}
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
