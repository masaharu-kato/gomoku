#include "group5.h"
using namespace Core;

//=======================================================
// Group 5
//=======================================================

Position Strategy5::getNextTurn() {
	
	clock_t end;

	printf( "Strategy 5\t" );

	auto count = board->getCount();
	switch(count) {
	case 0: return {5, 5};
	case 1:{
		Position hist0 = board->getHistory(0);
		return {
			hist0.x + (hist0.x <= 4) ? 1 : -1,
			hist0.y + (hist0.y <= 4) ? 1 : -1,
		};
	}
	case 2:{
		Position hist0 = board->getHistory(0);
		Position hist1 = board->getHistory(0);

		if( (hist0.x == hist1.x && abs(hist0.y - hist1.y) == 1)
		 || (hist0.y == hist1.y && abs(hist0.x - hist1.x) == 1)
		){
			if(hist1.x == 4 || hist1.y == 4) return {4, 4};
			if(hist1.x == 6 || hist1.y == 6) return {6, 6};
		}

		if(abs(hist0.x - hist1.x) == 1 && abs(hist0.y - hist1.y) == 1){
			if(hist1 == Position(4,4)) return {4, 6};
			if(hist1 == Position(4,6)) return {4, 4};
			if(hist1 == Position(6,4)) return {6, 6};
			if(hist1 == Position(6,6)) return {6, 4};
		}
	}
	}	
	
	int score = (count % 2 ==0 ) ? -99999999 : 99999999;
	
	board->doEach([&](const Position& p, Stone::Kind& k) {
		
		if(k == STONE_SPACE && kuu(p)){
			if(count % 2 == 0){
				cboard[p] = STONE_BLACK;
				int ss = hyouka1(p);
				if(ss >= 9999999) return p;

				int ww = 0;
				cboard.doEach([&](const Position& cp, Stone::Kind& k) {
					if(k == STONE_SPACE && kuu(cp)){
						k = STONE_WHITE;
						if(hyouka2(cp) <= -8888888) return cp;
						int s;
						if((s = hyouka2(cp)) <= ww) ww = s;
						k = STONE_SPACE;
					}
				});

				int subscore;
				if((subscore = ss + ww) > score){
					score = subscore;
					pos = p;
				}else if(subscore == score && (*pos_x - history[count-1].x)*(*pos_x - history[count-1].x) + (*pos_y - history[count-1].y)*(*pos_y - history[count-1].y) > (j - history[count-1].x)*(j - history[count-1].x) + (i - history[count-1].y)*(i - history[count-1].y)){
					pos = p;
				}
				cboard[p] = STONE_SPACE;
			}
			else{
				cboard[p] = STONE_WHITE;
				int ss = hyouka2(p);
				if(ss <= -9999999) return p;
				
				cboard->doEach([&](const Position& cp, Stone::Kind& k) {
					if(k == STONE_SPACE && kuu(p)){
						k = STONE_BLACK;
						if(hyouka1(cp) >= 8888888) return cp;
						if((bscore = hyouka1(cp)) >= bb) bb = bscore;	
						k = STONE_SPACE;
					}
				});
				if(  ( subscore = ss + bb ) < score){
						score = subscore;
						ret = p;
				}else if(subscore == score && (*pos_x - history[count-1].x)*(*pos_x - history[count-1].x) + (*pos_y - history[count-1].y)*(*pos_y - history[count-1].y) > (j - history[count-1].x)*(j - history[count-1].x) + (i - history[count-1].y)*(i - history[count-1].y)){
					ret = p;
				}
				k = STONE_SPACE;
			}
		}
	}
	return ret;
}

//5連かどうかチェック
int str5_golenCheck(int board[][BOARD_SIZE], int x, int y){
	int i, j, co = 1, kuu = 0;
	int dx[] = {0, 0, 1, -1, 1, -1, -1, 1};
	int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};
	
	for(i = 0; i < 8;i++){
		j = 1;
		while(1){
			if(0 > y+j*dy[i] && 0 > x+j*dx[i] && y+j*dy[i] >= BOARD_SIZE && x+j*dx[i] >= BOARD_SIZE){
				if(i % 2 == 1){
					if(co >= 5){
						return 1;
					}else{
						co = 1;
					}
				}
				break;
			}
			if(board[y][x] == board[y+j*dy[i]][x+j*dx[i]]){
				co++;
			}else{
				if(i % 2 == 1){
					if(co >= 5){
						return 1;
					}else{
						co = 1;
					}
				}
				break;
			}
			j++;
		}
	}
	return 0;
}


//フリー4連のチェック（飛び四は対象外)
int str5_yonlenCheck(int board[][BOARD_SIZE], int x, int y){
	int i, j, co = 1, kuu = 0;
	int dx[] = {0, 0, 1, -1, 1, -1, -1, 1};
	int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};
	
	for(i = 0; i < 8;i++){
		j = 1;
		while(1){
			if(0 > y+j*dy[i] && 0 > x+j*dx[i] && y+j*dy[i] >= BOARD_SIZE && x+j*dx[i] >= BOARD_SIZE){
				co = 1;
				break;
			}
			if(board[y][x] == board[y+j*dy[i]][x+j*dx[i]]){
				co++;
			}else if(board[y+j*dy[i]][x+j*dx[i]] == 0){
				if(i % 2 == 1){
					if(co >= 4) return 1;
					co = 1;
				}
				break;
			}else{
				if(i % 2 == 0){
					i++;
					co = 1;
					break;
				}else{
					co = 1;
					break;
				}
			}
			j++;
		}
	}
	return 0;
}

//三三以上になるかチェック
int str5_sslencheck(int board[][BOARD_SIZE] ,int x ,int y){
	int i, j ,co ,tui = 0, situi = 0,mk[8] = {0, 0, 0, 0, 0, 0, 0, 0} ,kuu =0 ,ok = 0;
	int dx[] = {0, 0, 1, -1, 1, -1, -1, 1};
	int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};
	
	for(i = 0;i < 8;i++){
		j = 1;
		while(1){
			if(0 > y+j*dy[i] || 0 > x+j*dx[i] || y+j*dy[i] >= BOARD_SIZE || x+j*dx[i] >= BOARD_SIZE){
				if(i % 2 == 1){
					if(co >=4){
						situi++;
					}
				}
				co = 1;
				break;
			}
			if(board[y][x] == board[y+j*dy[i]][x+j*dx[i]]){
				co++;
			}else if(board[y+j*dy[i]][x+j*dx[i]] == 0){
				if(i % 2 == 0){
					break;
				}else{
					if(co>=4){
						situi++;
					}
					if(co >= 3){
						tui++;
						mk[i - 1] = 1;
						mk[i] = 1;
						co = 1;
						break;
					}else{
						co = 1;
						break;
					}
				}
				break;
			}else{
				if(co>=4){
					tui++;
				}
				co = 1;
				break;
			}
			j++;
		}
	}
	if(situi >= 2){
		return 1;
	}else if(tui >= 2){
		for(i = 0;i < 8;i++){
			if(mk[i] == 1){
				j = 1;
				kuu = 0;
				while(1){
					if(0 > y+j*dy[i] || 0 > x+j*dx[i] || y+j*dy[i] >= BOARD_SIZE || x+j*dx[i] >= BOARD_SIZE || (board[y+j*dy[i]][x+j*dx[i]] != board[y][x] && board[y+j*dy[i]][x+j*dx[i]] != 0 ) ){
						if(kuu >= 2){
							ok++;
						}
						break;
					}else if(board[y+j*dy[i]][x+j*dx[i]] == 0){
						kuu++;
					}
					j++;
				}
			}
		}
		if(ok >= 2){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}
//仮に打つ場所の周りに石があるかどうか
int str5_kuu(int x, int y, int len[][BOARD_SIZE]){
	int i, j;
	for(i = y - 1;i <= y + 1;i++){
		for(j = x - 1;j <= x + 1;j++){
			if(i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE){
				if(len[i][j] != 0){
					return 1;
				}
			}
		}
	}
	return 0;
}

//黒の手の評価
int str5_hyouka1(int board[][BOARD_SIZE], int x, int y){
	int i, j ,co = 0 , l = 0;
	int b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, w1 = 0, w2 = 0, w3 = 0, w4 = 0 ,w5 = 0 ,w6 = 0;
	
	//打った手で5連、フリーの4連(結構弱い)ができるかどうか
	if(str5_golenCheck(board, x, y)){
		return 9999999;
	}else if(str5_yonlenCheck(board, x, y)){
		return 8888888;
	}else if(str5_sslencheck(board, x, y)){
		return 7777777;
	}
	
	for(i = y - 1;i <= y + 1;i++){
		for(j = x - 1;j <= x + 1;j++){
			if(i < 0 || j < 0 || i >= BOARD_SIZE || j >= BOARD_SIZE){
				break;
			}
			if(board[y][x] == STONE_WHITE){
				co = str5_tomecheck(board, x, y, j, i);
			}else if(board[y][x] == STONE_BLACK){
				l = str5_l_check(board, x, y, j, i);
			}else{
				l = 1;
			}
			if(l >= 5){
				b5++;
			}else if(l == 4){
				b4++;
			}else if(l == 3){
				b3++;
			}else if(l == 2){
				b2++;
			}else if(l == 1){
				b1++;
			}else if(co == 1){
				w1 += co;
			}else if(co == 2){
				w2 += co;
			}else if(co == 3){
				w3 += co;
			}else if(co == 4){
				w4 += co;
			}else if(co <= 5){
				w5 += co;
			}
		}
	}
	
	return b1 * 1 + b2 * 100 + b3 * 150 + b4 * 500 + b5 * 1000 + w1 * 10 + w2 * 1000 + w3 * 10000 + w4 * 15000 + w5 * 20000;
}

//白の手の評価
int str5_hyouka2(int board[][BOARD_SIZE], int x, int y){
	int i, j ,co = 0 ,l = 0;
	int b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, w1 = 0, w2 = 0, w3 = 0, w4 = 0 ,w5 = 0;
	
	//打った手で5連、フリーの4連ができるかどうか
	if(str5_golenCheck(board, x, y)){
		return -9999999;
	}else if(str5_yonlenCheck(board, x, y)){
		return -8888888;
	}else if(str5_sslencheck(board, x, y)){
		return -7777777;
	}
	
	for(i = y - 1;i <= y + 1;i++){
		for(j = x - 1;j <= x + 1;j++){
			if(i < 0 || j < 0 || i >= BOARD_SIZE || j >= BOARD_SIZE){
				break;
			}
			if(board[i][j] == STONE_BLACK){
				co = str5_tomecheck(board, x, y, j, i);
			}else if(board[i][j] == STONE_WHITE){
				l = str5_l_check(board, x, y, j, i);
			}else{
				l = 1;
			}
			
			if(co >= 5){
				b5 += co;
			}else if(co == 4){
				b4 += co;
			}else if(co == 3){
				b3 += co;
			}else if(co == 2){
				b2 += co;
			}else if(co == 1){
				b1 += co;
			}else if(l == 1){
				w1++;
			}else if(l == 2){
				w2++;
			}else if(l == 3){
				w3++;
			}else if(l == 4){
				w4++;
			}else if(l <= 5){
				w5++;
			}
		}
	}
	
	return -(b1 * 10 + b2 * 1000 + b3 * 10000 + b4 * 15000 + b5 * 20000 + w1 * 1 + w2 * 100 + w3 * 150 + w4 * 500 + w5 * 1000);
}

//自分の連鎖が作れているかどうか
int str5_l_check(int board[][BOARD_SIZE], int x, int y ,int s ,int t){
	int i, j = 1, k, co = 2,stop[] = {0, 0, 0, 0, 0, 0, 0, 0} ,kuu = 1;
	int dx[] = {0, 0, 1, -1, 1, -1, -1, 1};
	int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};
	
	if(board[t][s] == 0){
		return 0;
	}
	
	if(t == y - 1){
		if(s == x - 1){
			while(t + j*dy[5] >= 0 && s + j*dx[5] >= 0){
					if(board[t][s] == board[t+j*dy[5]][s+j*dx[5]]){
						co++;
					}else if(board[t+j*dy[5]][s+j*dx[5]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x){
			while(t + j*dy[1] >= 0){
					if(board[t][s] == board[t+j*dy[1]][s+j*dx[1]]){
						co++;
					}else if(board[t+j*dy[1]][s+j*dx[1]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x + 1){
			while(t + j*dy[7] >= 0 && s + j*dx[7] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[7]][s+j*dx[7]]){
						co++;
					}else if(board[t+j*dy[7]][s+j*dx[7]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}
	}else if(t == y){
		if(s == x - 1){
			while(s + j*dx[3] >= 0){
					if(board[t][s] == board[t+j*dy[3]][s+j*dx[3]]){
						co++;
					}else if(board[t+j*dy[3]][s+j*dx[3]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x + 1){
			while(s + j*dx[2] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[2]][s+j*dx[2]]){
						co++;
					}else if(board[t+j*dy[2]][s+j*dx[2]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}
	}else if(t == y + 1){
		if(s == x - 1){
			while(t + j*dy[6] < BOARD_SIZE && s + j*dx[6] >= 0){
					if(board[t][s] == board[t+j*dy[6]][s+j*dx[6]]){
						co++;
					}else if(board[t+j*dy[6]][s+j*dx[6]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x){
			while( t + j*dy[0] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[0]][s+j*dx[0]]){
						co++;
					}else if(board[t+j*dy[0]][s+j*dx[0]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x + 1){
			while(j*dy[4] < BOARD_SIZE && s + j*dx[4] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[4]][s+j*dx[4]]){
						co++;
					}else if(board[t+j*dy[4]][s+j*dx[4]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}
	}
	
	return co;
}
	

//相手の連鎖を止められているかどうか
int str5_tomecheck(int board[][BOARD_SIZE], int x, int y ,int s ,int t){
	int i, j = 1, k, co = 1 ,stop[] = {0, 0, 0, 0, 0, 0, 0, 0} ,kuu = 0;
	int dx[] = {0, 0, 1, -1, 1, -1, -1, 1};
	int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};
	
	if(board[t][s] == 0){
		return 0;
	}
	
	if(t == y - 1){
		if(s == x - 1){
			while(t + j*dy[5] >= 0 && s + j*dx[5] >= 0){
					if(board[t][s] == board[t+j*dy[5]][s+j*dx[5]]){
						co++;
					}else if(board[t+j*dy[5]][s+j*dx[5]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x){
			while(t + j*dy[1] >= 0){
					if(board[t][s] == board[t+j*dy[1]][s+j*dx[1]]){
						co++;
					}else if(board[t+j*dy[1]][s+j*dx[1]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x + 1){
			while(t + j*dy[7] >= 0 && s + j*dx[7] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[7]][s+j*dx[7]]){
						co++;
					}else if(board[t+j*dy[7]][s+j*dx[7]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}
	}else if(t == y){
		if(s == x - 1){
			while(s + j*dx[3] >= 0){
					if(board[t][s] == board[t+j*dy[3]][s+j*dx[3]]){
						co++;
					}else if(board[t+j*dy[3]][s+j*dx[3]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x + 1){
			while(s + j*dx[2] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[2]][s+j*dx[2]]){
						co++;
					}else if(board[t+j*dy[2]][s+j*dx[2]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}
	}else if(t == y + 1){
		if(s == x - 1){
			while(t + j*dy[6] < BOARD_SIZE && s + j*dx[6] >= 0){
					if(board[t][s] == board[t+j*dy[6]][s+j*dx[6]]){
						co++;
					}else if(board[t+j*dy[6]][s+j*dx[6]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x){
			while( t + j*dy[0] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[0]][s+j*dx[0]]){
						co++;
					}else if(board[t+j*dy[0]][s+j*dx[0]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}else if(s == x + 1){
			while(j*dy[4] < BOARD_SIZE && s + j*dx[4] < BOARD_SIZE){
					if(board[t][s] == board[t+j*dy[4]][s+j*dx[4]]){
						co++;
					}else if(board[t+j*dy[4]][s+j*dx[4]] == 0){
						if(kuu == 0){
							kuu++;
						}else{
						return co+1;
						}
					}else{
						return co;
					}
					j++;
			}
		}
	}
	
	return co;
}
