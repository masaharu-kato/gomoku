#include "group3.h"
using namespace Core;

//=======================================================
// Group 3
//=======================================================

Position Strategy3::getNextTurn(){

//	評価値の合計を収める
	BoardMap<int> number(1);

//	自分の石の色
	int turn = board->getKind();

//	1手目（先手）
	if(board->getCount() == 0) return {5, 5};

//	1手目（後攻）
	if(board->getCount() == 1){
		Position chist = board->getHistory(0);
		return {
			chist.x + (chist.x < 5 ? 1 : -1),
			chist.y + (chist.y < 5 ? 1 : -1),
		};
	}

//	自分の有利な選択
//	盤上のすべてのマス目
	board->doEachEmpty([&](const Position& p) {

	//	上左斜め4方向に関して処理
		for(Position p_diff : Directions::half){

			Stone::Number ns_me_or_empty = 1;
			Stone::Number ns_me = 0;
			Stone::Number ns_prev = 0;

		//	一つ前の石の色
			Stone::Kind pstone = turn;

		//	置いた位置から距離4以内に
			for(int di = 1; di <= 4; di++){

				Stone::Kind cstone = board->getStone(p + di * p_diff);

			//	現在の場所が相手、または壁でない場合
				if(cstone == turn || !cstone) ns_me_or_empty++;

			//	もし自分の石があり、そこが盤上であれば、
				if(cstone == turn) {
					ns_me++;
			
			//	1つ前に調べたやつが自分の石、または置くことを想定した石であり、現在調べている場所が空であれば
				} else if(!cstone && pstone == turn){
					ns_prev++;
					break;
				} else {
					break;
				}

			}

			//評価値の計算
			if(ns_me > 1) number[p] *= choiceValueForComputer(ns_me, ns_prev, turn);

		}
	});

	//初期化
	int max = -1111111110;
	int counter = 1;
	int picounter = 0;
	int stonecounter = 0;

	//相手の有利な選択
	board->doEachEmpty([&](const Position& p) {

	//	上左斜め4方向に関して処理
		for(Position p_diff : Directions::half){

		//	距離 1 ～ 4
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
		//評価値が一番大きい奴を選択
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
