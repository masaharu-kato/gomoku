#include "group2.h"
#include "position_map.h"
#include "directions.h"
using namespace Core;

//	
//	p: 基準位置
//	diff: 方向
//	kind: 色
int Strategy2::scan(const Position& p, const Position& diff, Stone::Kind kind){
	double f;
	
	int i1=0, i2=0;
	while(i1 < 4 && board->getStone(p + i1 * diff) != kind) i1++;
	while(i2 < 4 && board->getStone(p - i2 * diff) != kind) i2++;
	
	switch(i1+i2) {
	case 4: return 100000;
	case 3: return 5000;
	case 2: return 2000;
	case 1: return 1000;
	}

	return 0;
}

Position Strategy2::calcForce(const Position& p) {
	
	double f=0;
				
	for(int i = 0; i < 4; i++){
		f += 3/2* scan(p, directions[i], count%2+2);
		f +=      scan(p, directions[i], count%2+3)/2;
	}

	return f;
}

Position Strategy2::calc(){

//	先手の初手は (4, 5)
	if(board->getCount() == 0) return {4, 5};

//	後手の初手は先手の右斜め下
	if(board->getCount() == 1) return board->getHistory(0) + Position(1, 1);

//	先手の二番手は (5, 6)が埋まっていれば(6, 5)、そうでなければ(2, 5)
	if(board->getCount() == 2){
		if(board->getStone({5, 6})) return {6, 5}; else return {2, 5};
	}

	
	BoardMap<bool> search;
	Position max_p(0, 0);
	double max = 0;

	for(const Position& p : board->getHistory()){
		for(const Position& d : directions) {
			Position cpos = p + d;
			if(board->getStone(cpos)) {

				if(max<f){
					max_p = cpos;
					max=f;
				}
			}
		}
	}
	return max_p;
}

