#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10      // 盤面サイズ 10 * 10
#define STONE_SPACE 0      // 盤面にある石 なし
#define STONE_BLACK 1      // 盤面にある石 黒
#define STONE_WHITE 2      // 盤面にある石 白


typedef struct{
	int x;
	int y;
} position;

void inputPutPos(int board[][BOARD_SIZE], const int which, const int count, position *history, const int *group );
void changeTurn(int *which_turn);
int checkOutPos(int x, int y);
void gameInit(int board[][BOARD_SIZE], int *which_turn);
void boardInit(int board[][BOARD_SIZE]);
void boardPrint(int board[][BOARD_SIZE]);
int gameEndProcess(int board[][BOARD_SIZE]);
int lenCheck(int board[][BOARD_SIZE], int x, int y);
void randomStrategy( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );
void keyboardInput( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );

typedef void (*strategy)( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );





#include <functional>

	double str1_ef5[32][5] = {
		/* 00000 */ {   0,   0,   0,   0,   0},
		/* 00001 */ { 0.1, 0.2, 0.5,   1,   0},
		/* 00010 */ { 0.2, 0.5,   1,   0,   1},
		/* 00011 */ { 0.5,   1,   2,   0,   0},
		/* 00100 */ { 0.5,   1,   0,   1, 0.5},
		/* 00101 */ {   1,   2,   0,   5,   0},
		/* 00110 */ {   1,   2,   0,   0,   2},
		/* 00111 */ {  16,  32,   0,   0,   0},

		/* 01000 */ {   1,   0,   1, 0.5, 0.2},
		/* 01001 */ {   1,   0,   2,   2,   0},
		/* 01010 */ {   2,   0,   5,   0,   1},
		/* 01011 */ {   2,   0,   4,   0,   0},
		/* 01100 */ {   4,   0,   0,   4,   2},
		/* 01101 */ {   2,   0,   0,   4,   0},
		/* 01110 */ { 100,   0,   0,   0,  80},
		/* 01111 */ { 100,   0,   0,   0,   0},
		

		/* 10000 */ {   0,   1, 0.5, 0.2, 0.1},
		/* 10001 */ {   0, 1.5, 0.6, 1.5,   0},
		/* 10010 */ {   0,   4,   4,   0,   2},
		/* 10011 */ {   0,   4,   4,   0,   0},
		/* 10100 */ {   0,   5,   0,   2,   0},
		/* 10101 */ {   0,   6,   0,   6,   0},
		/* 10110 */ {   0, 100,   0,   0,  80},
		/* 10111 */ {   0, 100,   0,   0,   0},

		/* 11000 */ {   0,   0,   2,   1, 0.5},
		/* 11001 */ {   0,   0,   4,   4,   0},
		/* 11010 */ {   0,   0, 100,   0,  80},
		/* 11011 */ {   0,   0, 100,   0,   0},
		/* 11100 */ {   0,   0,   0, 100,  80},
		/* 11101 */ {   0,   0,   0, 100,   0},
		/* 11110 */ {   0,   0,   0,   0, 100},
		/* 11111 */ {   0,   0,   0,   0,   0},
	};


	void str1_calcScore(int i1len,
		std::function<int(int)> getI2Length,
		std::function<int(int, int)> getStone,
		std::function<double*(int, int)> getScore,
		int mystone,
		double rate
	) {
		
		for(int i1=0; i1<i1len; i1++) {

			int len_beg = -1;
			unsigned long pattern = 0;

			int i2len = getI2Length(i1);
			for(int i2=0; i2<i2len; i2++) {
				int cstone = getStone(i1, i2);

			//	自分の石か、空白なら
				if(!cstone || cstone == mystone) {
				//	連続開始の場合、連続開始位置を設定
					if(len_beg < 0) len_beg = i2;
				//	石の有無を記録
					pattern = (pattern << 1) | (!!cstone);
				}else{
				//	連続を終了
					len_beg = -1;
				}

			//	五連続になれば
				if(i2 - len_beg + 1 == 5) {
					double* cef5 = str1_ef5[pattern & 31];
					for(int i=0; i<5; i++) *getScore(i1, len_beg + i) += rate * cef5[i];
				}

			}
		}

	}

	void str1_calcStoneAll(const int board[][BOARD_SIZE], double score[][BOARD_SIZE], int mystone, double rate) {
		
	//	X方向
		str1_calcScore(BOARD_SIZE,
			[](int i1){ return BOARD_SIZE; },
			[&](int i1, int i2){ return  board[i1][i2]; },
			[&](int i1, int i2){ return &score[i1][i2]; },
			mystone, rate
		);
		
	//	Y方向
		str1_calcScore(BOARD_SIZE,
			[](int i1){ return BOARD_SIZE; },
			[&](int i1, int i2){ return  board[i2][i1]; },
			[&](int i1, int i2){ return &score[i2][i1]; },
			mystone, rate
		);
		
	//	右下がり斜め方向
		str1_calcScore(BOARD_SIZE*2-1,
			[](int i1){ return (i1 <= 9) ? i1+1 : 19-i1; },
			[&](int i1, int i2){ return  board[i1 < 9 ? 9-i1+i2 : i2][i1 < 9 ? i2 : i1-9+i2]; },
			[&](int i1, int i2){ return &score[i1 < 9 ? 9-i1+i2 : i2][i1 < 9 ? i2 : i1-9+i2]; },
			mystone, rate
		);
		
	//	左下がり斜め方向
		str1_calcScore(BOARD_SIZE*2-1,
			[](int i1){ return (i1 <= 9) ? i1+1 : 19-i1; },
			[&](int i1, int i2){ return  board[i1 < 9 ? i2 : i2+i1-9][i1 < 9 ? i1-i2 : 9-i2]; },
			[&](int i1, int i2){ return &score[i1 < 9 ? i2 : i2+i1-9][i1 < 9 ? i1-i2 : 9-i2]; },
			mystone, rate
		);

	}


	void strategy1(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history ) {
	
	//	今の色
		int mystone =  (count % 2) ? STONE_BLACK : STONE_WHITE;
		int enstone =  (mystone == STONE_WHITE) ? STONE_BLACK : STONE_WHITE;

	//	評価値
		double score[BOARD_SIZE][BOARD_SIZE] = {0};

	//	評価値を計算
		str1_calcStoneAll(board, score, enstone, 2.0);
	//	str1_calcStoneAll(board, score, mystone, 1.0);

	//	
		double cmax = 0;
		int xmax = -1;
		int ymax = -1;

		for(int iy=0; iy<BOARD_SIZE; iy++) {	
			for(int ix=0; ix<BOARD_SIZE; ix++) {
				if(board[iy][ix]) continue;
				if(xmax < 0 || ymax < 0){ xmax = ix; ymax = iy; }
				double cscore = score[iy][ix];
				if(cmax < cscore){
					cmax = cscore;
					xmax = ix;
					ymax = iy;
				}
			}
		}

		*pos_x = xmax;
		*pos_y = ymax;
	}















//=======================================================
// Group 3
//=======================================================

int str3_choicevalueforplayer(int c, int pi, int turn);
int str3_choicevalueforcomputer(int c, int pi,int turn);

void strategy3( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int counting, const position *history ){
  //上、下、左、右、左上、右下、右上、左下
  int dy[]={-1, 1, 0, 0, -1, 1, -1, 1};
  int dx[]={0, 0, -1, 1, -1, 1, 1, -1};
  int i, j, y, x, counter=1, picounter=0, max=-111111111, ncounter=0, stonecounter=0;
  //評価値の合計を収める
  int number[BOARD_SIZE][BOARD_SIZE]={0};

	int turn = counting % 2 == 0 ? STONE_BLACK : STONE_WHITE;

	printf( "Strategy 3\t" );

  for(i=0; i<BOARD_SIZE; i++){
    for(j=0; j<BOARD_SIZE; j++)
      number[i][j] = 1;
  }

  //１手目（先手）
  if(counting == 0){
    *pos_x = 5;
    *pos_y = 5;
  }
  //1手目（後攻）
  else if(counting == 1){
    if(history[0].x < 5)
      *pos_x = history[0].x + 1;
    else
      *pos_x = history[0].x - 1;
    if(history[0].y < 5)
      *pos_y = history[0].y + 1;
    else
      *pos_y = history[0].y - 1;
  }
  //２手目以降
  else{
    //自分の有利な選択
    //盤上のすべてのマス目
    for(y=0; y<BOARD_SIZE; y++){
      for(x=0; x<BOARD_SIZE; x++){
	if(board[y][x] == STONE_SPACE){
	  //上下左右斜めに対して
	  for(i=0; i<8; i++){
	    //置いた位置から距離4以内に
	    for(j=1; j<=4; j++){
	      //現在の場所が相手、または壁でない場合
	      if((board[y+j*dy[i]][x+j*dx[i]] == turn  ||  board[y+j*dy[i]][x+j*dx[i]] == 0) && ((y+j*dy[i]) >= 0) && ((y+j*dy[i]) < 10) && ((x+j*dx[i]) >= 0) && ((x+j*dx[i]) < 10)){
		stonecounter++;
		//printf("%d, %d, %d\n",x+j*dx[i] , y+j*dy[i], stonecounter);
	      }
	      //もし自分の石があり、そこが盤上であれば、
	      if((board[y+j*dy[i]][x+j*dx[i]] == turn) && ((y+j*dy[i]) >= 0) && ((y+j*dy[i]) < 10) && ((x+j*dx[i]) >= 0) && ((x+j*dx[i]) < 10))
		counter++;
	      //1つ前に調べたやつが自分の石、または置くことを想定した石であり、現在調べている場所が空であれば
	      else if(board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE && (board[y+(j-1)*dy[i]][x+(j-1)*dx[i]] == turn || (y+(j-1)*dy[i] == y && x+(j-1)*dx[i] == x))){
		picounter++;
		break;
	      }
	      else
		break;
	    }
	    if(i % 2 != 0){
	      //評価値の計算
	      if(counter > 1)
		number[y][x] *= str3_choicevalueforcomputer(counter, picounter, turn);
	      //カウンタリセット
	      counter = 1;
	      picounter = 0;
	      stonecounter = 0;
	    }
	  }
	}
      }
    }
    //初期化
    max = -1111111110;
    counter = 1;
    picounter = 0;
    stonecounter = 0;
    //相手の有利な選択
    for(y=0; y<BOARD_SIZE; y++){
      for(x=0; x<BOARD_SIZE; x++){
	if(board[y][x] == STONE_SPACE){
	  for(i=0; i<8; i++){
	    for(j=1; j<=4; j++){
	      if(board[y+j*dy[i]][x+j*dx[i]] != turn  &&  ((y+j*dy[i]) >= 0) && ((y+j*dy[i]) < 10) && ((x+j*dx[i]) >= 0) && ((x+j*dx[i]) < 10))
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
		  *pos_x = x;
		  *pos_y = y;
		}
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

//=======================================================
// Group 4
//=======================================================

void strategy4( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );

void strategy4( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{

  int myturn,enemy;//自身の黒白を判定
  
  printf( "Strategy 4\t" );
  
  if(count%2==0){
    myturn = STONE_BLACK;
    enemy = STONE_WHITE;
  }else if(count%2==1){
    myturn = STONE_WHITE;
    enemy = STONE_BLACK;
  }
    double tmp_d;
    int check;
  if(count==0||count==1){//初手
 
      check = 0;
      while( check == 0 ){//真ん中付近に打つ
	tmp_d = (double)rand() / ((double)RAND_MAX + 1);
	*pos_x = (int)( tmp_d * 3.0 + 3.0 );

	tmp_d = (double)rand() / ((double)RAND_MAX + 1);
	*pos_y = (int)( tmp_d * 3.0 + 3.0 );

	if( board[*pos_y][*pos_x] == STONE_SPACE ){
	  check = 1;
	  return;
	}
      }

  }

  if(count>=2){

    int x,y,i,j,k,temp_x,temp_y,max_sum,fault,success_m,success_e,sw_e;
    int check;
    int dx[] = { 0, 1, 1, -1, 0, -1, -1, 1 };
    int dy[] = { 1, 0, 1, 1, -1, 0, -1, -1 };
    int my_sum,enemy_sum;

      max_sum = 0;
   

      for(y = 0; y < BOARD_SIZE; y++){
	for(x = 0; x < BOARD_SIZE; x++){
	  if(board[y][x] != STONE_SPACE){continue;}
	  my_sum = 0;
	  enemy_sum = 0;
	  success_m = 0;
	  for (k = 4; k > 0; k--) {//並んでいる個数---my_stone
	    for(i = 0; i < 8 ; i++){//方向を定める(８方向)
	      fault = 0;
	      for (j = 1; j <= k; j++) {
		if (   y+j*dy[i] >= BOARD_SIZE
                    || x+j*dx[i] >= BOARD_SIZE
                    || y+j*dy[i] < 0
                    || x+j*dx[i] < 0
                    ||board[y+j*dy[i]][x+j*dx[i]] != myturn){
		  fault = 1;
		  break;
		}	
	      }
	      if(fault == 0&&success_m==0){
		if(k==4){
		  *pos_x = x;
		  *pos_y = y;
		  // printf("mystone:4\n");
		  return ;
		}
		if(k==3){
		  if(board[y-dy[i]][x-dx[i]]==myturn&&y-dy[i]>=0&&y-dy[i]<10&&x-dx[i]>=0&&x-dx[i]<10){
		    my_sum = my_sum + 10000;
		  }
		}
		/*	if(k==2){
		  if(board[y-dy[i]][x-dx[i]]==myturn&&y-dy[i]>=0&&y-dy[i]<10&&x-dx[i]>=0&&x-dx[i]<10){
		    if(board[y-2*dy[i]][x-2*dx[i]]==myturn){
		      my_sum = my_sum + 10000;
		    }else{
		      my_sum = my_sum + 1000;
		    }
		  }
		  }*/
		my_sum = my_sum + k;
		my_sum++;
		success_m = 1;
	      }
	    }
	  }


	  if(max_sum<my_sum){
	    temp_x = x;
	    temp_y = y;
	    max_sum = my_sum;
	  }
	  success_e = 0;
	  sw_e = 0;
	  for (k = 4; k > 1; k--) {//並んでいる個数---enemy
	    for(i = 0; i < 8 ; i++){//方向を定める(８方向)
	      fault = 0;
	      for (j = 1; j <= k; j++) {
		if (   y+j*dy[i] >= BOARD_SIZE
                    || x+j*dx[i] >= BOARD_SIZE
                    || y+j*dy[i] < 0
                    || x+j*dx[i] < 0
                    ||board[y+j*dy[i]][x+j*dx[i]] != enemy){
		  fault = 1;
		  break;
		}	
	      }
	      if(fault == 0&&success_e==0){
		if(k==4){
		  enemy_sum = enemy_sum + 1000;
		  // printf("enemy:4\n");
		}
		if(k==3){
		  if(board[y+(j+1)*dy[i]][x+(j+1)*dx[i]]==STONE_SPACE){
		    enemy_sum = enemy_sum + 100;
		    //  printf("enemy:3\n");
		  }
		  if(board[y-dy[i]][x-dx[i]]==enemy){
		    enemy_sum = enemy_sum + 1000;
		    // printf("enemy:jump 4\n");
		  }
		}
		if(k==2){
		  if(board[y-dy[i]][x-dx[i]]==enemy){
		    if(board[y-2*dy[i]][x-2*dx[i]]==enemy){
		      enemy_sum = enemy_sum + 1000;
		    }else{
		      enemy_sum = enemy_sum + 100;
		      // printf("enemy:jump 3\n");
		    }
		  }
		}
		enemy_sum = enemy_sum + k;
		enemy_sum++;
		success_e = 1;
	      }
	    }
	  }

	  if(max_sum<enemy_sum){
	    temp_x = x;
	    temp_y = y;
	    max_sum = enemy_sum;
	  }
	}
      }

      if(max_sum>0){
	*pos_x = temp_x;
	*pos_y = temp_y;
	// printf("choice\n");
	return;
      }
    
      check = 0;
      while( check == 0 ){
	tmp_d = (double)rand() / ((double)RAND_MAX + 1);
	*pos_x = (int)( tmp_d * 10.0 );

	tmp_d = (double)rand() / ((double)RAND_MAX + 1);
	*pos_y = (int)( tmp_d * 10.0 );

	if( board[*pos_y][*pos_x] == STONE_SPACE ){
	  check = 1;
	  //  printf("random\n");
	  return;
	}
      }
  }

}


//=======================================================
// Group 5
//=======================================================

int str5_tomecheck(int board[][BOARD_SIZE], int x, int y ,int s ,int t);
int str5_l_check(int board[][BOARD_SIZE], int x, int y ,int s ,int t);
int str5_hyouka2(int board[][BOARD_SIZE], int x, int y);
int str5_hyouka1(int board[][BOARD_SIZE], int x, int y);
int str5_kuu(int x, int y, int len[][BOARD_SIZE]);
int str5_sslencheck(int board[][BOARD_SIZE] ,int x ,int y);
int str5_yonlenCheck(int board[][BOARD_SIZE], int x, int y);
int str5_golenCheck(int board[][BOARD_SIZE], int x, int y);

void strategy5( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
	
	int cboard[BOARD_SIZE][BOARD_SIZE] ,bscore = 0, wscore = 0 ,bb = 0 ,ww = 0;
	int a = 0, b = 0, c = 0, d = 0, i = 0, j = 0 ,k = 0 ,l = 0, m = 0, n = 0, score = 0 ,subscore = 0 ,ss;
	
	printf( "Strategy 5\t" );

	for(i = 0;i < BOARD_SIZE;i++){
		for(j = 0;j < BOARD_SIZE;j++){
			cboard[i][j] = board[i][j];
		}
	}
	
	if(count == 0){
		*pos_x = 5;
		*pos_y = 5;
		return;
	}else if(count == 1){
		if(history[0].x <= 4){
			*pos_x = history[0].x + 1;
		}else{
			*pos_x = history[0].x - 1;
		}
		if(history[0].y <= 4){
			*pos_y = history[0].y + 1;
		}else{
			*pos_y = history[0].y - 1;
		}
		return;
	}else if(count == 2){
		if( (history[0].x == history[1].x && abs(history[0].y - history[1].y) == 1) || (history[0].y == history[1].y && abs(history[0].x - history[1].x) == 1) ){
			if(history[1].x == 4 || history[1].y == 4){
				*pos_x = 4;
				*pos_y = 4;
			}else if(history[1].x == 6 ||history[1].y == 6){
				*pos_x = 6;
				*pos_y = 6;
			}
		}else if( abs(history[0].x - history[1].x) == 1 && abs(history[0].y - history[1].y) == 1){
			if(history[1].x == 4){
				if(history[1].y == 4){
					*pos_x = 4;
					*pos_y = 6;
				}else if(history[1].y == 6){
					*pos_x = 4;
					*pos_y = 4;
				}
			}else if(history[1].x == 6){
				if(history[1].y == 4){
					*pos_x = 6;
					*pos_y = 6;
				}else if(history[1].y == 6){
					*pos_x = 6;
					*pos_y = 4;
				}
			}
		}
		return;
	} 
		
	
	if(count % 2 ==0 ){
		score = -99999999;
	}else{
		score = 99999999;
	}
	
	for(i = 0;i < BOARD_SIZE;i++){
		for(j = 0;j < BOARD_SIZE;j++){
			if(board[i][j] == STONE_SPACE && str5_kuu(j, i, cboard)){
				if(count % 2 == 0){
					cboard[i][j] = STONE_BLACK;
					ss = str5_hyouka1(cboard, j, i);
					if(ss >= 9999999){
						*pos_x = j;
						*pos_y = i;
						return;
					}
					ww = 0;
					for(a = 0;a < BOARD_SIZE;a++){
						for(b = 0;b < BOARD_SIZE;b++){
							if(cboard[a][b] == STONE_SPACE && str5_kuu(b, a, cboard)){
								cboard[a][b] = STONE_WHITE;
								if(str5_hyouka2(cboard, b, a) <= -8888888){
									*pos_x = b;
									*pos_y = a;
									return;
								}
								if( (wscore = str5_hyouka2(cboard, b, a)) <= ww){
									ww = wscore;
								}
								cboard[a][b] = STONE_SPACE;
							}
						}
					}
					if(  ( subscore = ss + ww ) > score){
							score = subscore;
							*pos_x = j;
							*pos_y = i;
					}else if(subscore == score && (*pos_x - history[count-1].x)*(*pos_x - history[count-1].x) + (*pos_y - history[count-1].y)*(*pos_y - history[count-1].y) > (j - history[count-1].x)*(j - history[count-1].x) + (i - history[count-1].y)*(i - history[count-1].y)){
						*pos_x = j;
						*pos_y = i;
					}
					cboard[i][j] = STONE_SPACE;
				}else{
					cboard[i][j] = STONE_WHITE;
					ss = str5_hyouka2(cboard, j, i);
					if(ss <= -9999999){
						*pos_x = j;
						*pos_y = i;
						return;
					}
					for(a = 0;a < BOARD_SIZE;a++){
						for(b = 0;b < BOARD_SIZE;b++){
							if(cboard[a][b] == STONE_SPACE && str5_kuu(b, a, cboard)){
								cboard[a][b] = STONE_BLACK;
								if(str5_hyouka1(cboard, b, a) >= 8888888){
									*pos_x = b;
									*pos_y = a;
									return;
								}
									if( (bscore = str5_hyouka1(cboard, b, a)) >= bb){
										bb = bscore;
									}
								cboard[a][b] = STONE_SPACE;
							}
						}
					}
					if(  ( subscore = ss + bb ) < score){
							score = subscore;
							*pos_x = j;
							*pos_y = i;
					}else if(subscore == score && (*pos_x - history[count-1].x)*(*pos_x - history[count-1].x) + (*pos_y - history[count-1].y)*(*pos_y - history[count-1].y) > (j - history[count-1].x)*(j - history[count-1].x) + (i - history[count-1].y)*(i - history[count-1].y)){
						*pos_x = j;
						*pos_y = i;
					}
					cboard[i][j] = STONE_SPACE;
				}
			}
		}
	}
	return;
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
					if(co >= 4){
						return 1;
					}else{
						co = 1;
					}
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
	int j = 1, co = 2,stop[] = {0, 0, 0, 0, 0, 0, 0, 0} ,kuu = 1;
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

//=======================================================
// Group 6
//=======================================================

void strategy6(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int i, j, k, cnt = 0, _cnt;
	int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int point[BOARD_SIZE][BOARD_SIZE] = { 0 };

	printf( "Strategy 6\t" );

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) cnt++;
		}
	}
	if (cnt == BOARD_SIZE * BOARD_SIZE) {
		*pos_x = BOARD_SIZE / 2 - (rand() % 2);
		*pos_y = BOARD_SIZE / 2 - (rand() % 2);
	}

	else {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE) {
					int a = 0, b = 0;
					if (i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) point[i][j] -= 100;
					if (!(i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1)) {
						_cnt = 0;
						for (k = 0; k < 8; k++) {
							if (board[i + dx[k]][j + dy[k]] != STONE_SPACE) _cnt++;
						}
						if (_cnt >= 6) point[i][j] -= 100;
					}
					for (k = 0; k < 8; k++) {
						if (i + dx[k] >= 0 && j + dy[k] >= 0 && i + dx[k] < BOARD_SIZE && j + dy[k] < BOARD_SIZE) { //1個隣だけ見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK) {
									point[i][j] += 10;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 100;
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 1;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE) {
									point[i][j] += 20;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 50;
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 1;
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE) {
									point[i][j] += 10;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 100;
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 1;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK) {
									point[i][j] += 20;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 50;
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 1;
									}
								}
							}
						}
						if ((i + 2 * dx[k] >= 0 && j + 2 * dy[k] >= 0) && (i + 2 * dx[k] < BOARD_SIZE && j + 2 * dy[k] < BOARD_SIZE)) { //2個隣まで見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 5;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 10;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10000;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 30;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10;
											}
										}
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 80;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 50;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 200;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 1300;
											}
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] -= 40;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 5;
											if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 50;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) {
											point[i][j] += 50;
											a++;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 100;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) {
											point[i][j] += 100;
											b++;
										}
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 5;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 10;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10000;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 30;
												else if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 10;
											}
										}
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 80;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 50;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 200;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) point[i][j] += 1300;
											}
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] -= 40;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 5;
											if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) point[i][j] += 10;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE) {
									point[i][j] += 50;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE) {
											point[i][j] += 50;
											a++;
										}
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_SPACE
									|| board[i + dx[k]][j + dy[k]] == STONE_SPACE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK) {
									point[i][j] += 100;
									if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE
											|| board[i - dx[k]][j - dy[k]] == STONE_SPACE && board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK) {
											point[i][j] += 100;
											b++;
										}
									}
								}
							}
						}
						if ((i + 3 * dx[k] >= 0 && j + 3 * dy[k] >= 0) && (i + 3 * dx[k] < BOARD_SIZE && j + 3 * dy[k] < BOARD_SIZE)) { //3個隣まで見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									point[i][j] += 100;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 500; //反対側に1個あった時
										else if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 200;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									point[i][j] += 130;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 700; //反対側に1個あった時
									}
								}
								int l, cnt1 = 0, cnt2 = 0;
								for (l = 1; l <= 3; l++) {
									if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
										cnt1++;
										cnt2 -= 2;
									}
									else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
										cnt1 -= 2;
										cnt2++;
									}
								}
								if (cnt1 >= 2) {
									a++;
									point[i][j] += a * 120;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK)
										{
											point[i][j] += 20; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_BLACK || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
											}
										}
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 10; //反対側に1個あった時
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 100;
									}
								}
								if (cnt2 >= 2) {
									b++;
									b += a;
									point[i][j] += b * 140;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 5; //反対側に1個あった時
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 200; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
										}
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 30;
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									point[i][j] += 100;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] += 500; //反対側に1個あった時
										else if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 200;
									}
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK
									&& board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									point[i][j] += 130;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 700; //反対側に1個あった時
									}
								}
								int l, cnt1 = 0, cnt2 = 0;
								for (l = 1; l <= 3; l++) {
									if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
										cnt1++;
										cnt2 -= 2;
									}
									else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
										cnt1 -= 2;
										cnt2++;
									}
								}
								if (cnt1 >= 2) {
									a++;
									point[i][j] += a * 120;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
											point[i][j] += 20; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
											if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
												if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
											}
										}
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //反対側に1個あった時
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 100;
									}
								}
								if (cnt2 >= 2) {
									b++;
									b += a;
									point[i][j] += b * 140;
									if (board[i + dx[k]][j + dy[k]] == STONE_SPACE) point[i][j] -= 30;
									if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
										if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 5; //反対側に1個あった時
										if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) {
											point[i][j] += 200; //反対側に1個あった時
											if (i - dx[k] == 0 && j - dy[k] == 0 && i - dx[k] == BOARD_SIZE - 1 && j - dy[k] == BOARD_SIZE - 1) point[i][j] -= 30;
										}
									}
									if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) {
										if (board[i + 4 * dx[k]][i + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 30;
									}
								}
							}
						}
						if (i + 4 * dx[k] >= 0 && j + 4 * dy[k] >= 0 && i + 4 * dx[k] < BOARD_SIZE && j + 4 * dy[k] < BOARD_SIZE) { //4個隣まで見たとき
							if (count % 2 == 0) {
								if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) {
										point[i][j] += 10000;
										if ((i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) && (i + 4 * dx[k] == 0 || i + 4 * dx[k] == BOARD_SIZE - 1 || j + 4 * dy[k] == 0 || j + 4 * dy[k] == BOARD_SIZE - 1)) point[i][j] -= 10000;
									}
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) point[i][j] += 2000;
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 700;
								}
								if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) point[i][j] -= 70;
								else {
									int l, cnt1 = 0, cnt2 = 0;
									for (l = 1; l <= 4; l++) {
										if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
											cnt1++;
											cnt2 -= 2;
										}
										else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
											cnt1 -= 2;
											cnt2++;
										}
									}
									if (cnt1 >= 3) {
										point[i][j] += 90;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
												point[i][j] += 20; //反対側に1個あった時
												if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
													if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
												}
											}
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //反対側に1個あった時
										}
									}
									if (cnt2 >= 3) {
										point[i][j] += 150;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 20; //反対側に1個あった時
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 20; //反対側に1個あった時
										}
									}
								}
							}
							else {
								if (board[i + dx[k]][j + dy[k]] == STONE_WHITE && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_WHITE && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_WHITE) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_WHITE) {
										point[i][j] += 10000;
										if ((i == 0 || i == BOARD_SIZE - 1 || j == 0 || j == BOARD_SIZE - 1) && (i + 4 * dx[k] == 0 || i + 4 * dx[k] == BOARD_SIZE - 1 || j + 4 * dy[k] == 0 || j + 4 * dy[k] == BOARD_SIZE - 1)) point[i][j] -= 10000;
									}
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
								}
								else if (board[i + dx[k]][j + dy[k]] == STONE_BLACK && board[i + 2 * dx[k]][j + 2 * dy[k]] == STONE_BLACK && board[i + 3 * dx[k]][j + 3 * dy[k]] == STONE_BLACK) {
									if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) point[i][j] += 2000;
									else if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_SPACE) point[i][j] += 700;
								}
								if (board[i + 4 * dx[k]][j + 4 * dy[k]] == STONE_BLACK) point[i][j] -= 70;
								else {
									int l, cnt1 = 0, cnt2 = 0;
									for (l = 1; l <= 4; l++) {
										if (board[i + l * dx[k]][j + l * dy[k]] == STONE_WHITE) {
											cnt1++;
											cnt2 -= 2;
										}
										else if (board[i + l * dx[k]][j + l * dy[k]] == STONE_BLACK) {
											cnt1 -= 2;
											cnt2++;
										}
									}
									if (cnt1 >= 3) {
										point[i][j] += 90;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) {
												point[i][j] += 20; //反対側に1個あった時
												if (i - 2 * dx[k] >= 0 && j - 2 * dy[k] >= 0 && i - 2 * dx[k] < BOARD_SIZE && j - 2 * dy[k] < BOARD_SIZE) {
													if (board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_WHITE || board[i - 2 * dx[k]][j - 2 * dy[k]] == STONE_SPACE) point[i][j] += 1000;
												}
											}
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] -= 10; //反対側に1個あった時
										}
									}
									if (cnt2 >= 3) {
										point[i][j] += 150;
										if (i - dx[k] >= 0 && j - dy[k] >= 0 && i - dx[k] < BOARD_SIZE && j - dy[k] < BOARD_SIZE) {
											if (board[i - dx[k]][j - dy[k]] == STONE_WHITE) point[i][j] -= 20; //反対側に1個あった時
											if (board[i - dx[k]][j - dy[k]] == STONE_BLACK) point[i][j] += 20; //反対側に1個あった時
										}
									}
								}
							}
						}
					}
				}
			}
		}
		int p_max = -10000;
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (p_max < point[i][j] && board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) p_max = point[i][j];
			}
		}

		int b_flag = 0;
		while (1) {
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					if (point[i][j] == p_max && board[i][j] != STONE_BLACK && board[i][j] != STONE_WHITE) {
						*pos_x = j;
						*pos_y = i;
						if (rand() % 3 == 0) b_flag = 1;
					}
					if (b_flag == 1) break;
				}
				if (b_flag == 1) break;
			}
			if (b_flag == 1) break;
		}
	}

	return;
}

//=======================================================
// Group 7
//=======================================================

#include<set>
#include<algorithm>
#include<iostream>
using namespace std;

class str7_Gomoku{
public:
	str7_Gomoku();
	~str7_Gomoku();
	bool hit(int y, int x, int color);//置いた場所の判定で勝敗が決まるか判定
	bool hit(int y, int x);
	bool checkHit(int y, int x)const; //おけなければfalse
	void print()const;
	int getColor(int y, int x);
	int getNowColor()const;
private:
	int color;
	bool judge(int y, int x);//置いた場所の判定で勝敗が決まるか判定
	int table[10][10]; //y x
};

class str7_AI{
public:
	str7_AI();
	str7_AI(int color);
	~str7_AI();
	std::pair<int, int> getHit(str7_Gomoku gomoku);
	double eval(str7_Gomoku &gomoku);
	double eval(str7_Gomoku &gomoku, int color);
//private:
        int dfsab(str7_Gomoku &g,int color,int depth,int alph,int beta);
  //int AI::dfsab(Gomoku &g,int color,int depth,int alph,int beta);
	int dfs(str7_Gomoku &g, int color, int depth);
	int getConnectN(str7_Gomoku &gomoku, int color);
	std::set<std::pair<int, int> > getPos(str7_Gomoku &gomoku, int color);
	std::set<std::pair<int, int> > getPriorityPos(str7_Gomoku &gomoku, int color);
	std::set<std::pair<int, int> > getAround(str7_Gomoku &gomoku);
	int getEnemyColor();
	int getEnemyColor(int color);
	int myColor;
};




str7_Gomoku::str7_Gomoku(){
	color = 1;
	for (int i = 0; i<10; ++i){
		for (int j = 0; j<10; ++j){
			table[i][j] = 0;
		}
	}
}

str7_Gomoku::~str7_Gomoku(){

}

bool str7_Gomoku::hit(int y, int x, int color){
	table[y][x] = color;
	return judge(y, x);
}

bool str7_Gomoku::hit(int y, int x){
	table[y][x] = color;
	//	cout << table[y][x] <<" color:"<<color <<endl;
	if (color == 1) color = 2;
	else color = 1;
	return judge(y, x);
}

bool str7_Gomoku::judge(int y, int x){
	const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int dy[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int co = table[y][x];
	for (int i = 0; i<4; ++i){
		int c = 1;
		int ny = y;
		int nx = x;
		for (int j = 0; j<4; ++j){
			ny = ny + dy[i];
			nx = nx + dx[i];
			if (0 <= ny && ny <= 9 && 0 <= nx && nx <= 9){
				if (table[ny][nx] == co) c++;
				else break;
			}
		}
		ny = y;
		nx = x;
		for (int j = 0; j<4; ++j){
			ny = ny + dy[i + 4];
			nx = nx + dx[i + 4];
			if (0 <= ny && ny <= 9 && 0 <= nx && nx <= 9){
				if (table[ny][nx] == co) c++;
				else break;
			}

		}
		//		cout << c <<endl;
		if (c >= 5) return true;
	}
	return false;
}

bool str7_Gomoku::checkHit(int y, int x)const{
	if (y< 0 || 9< y || x< 0 || 9< x) return false;
	if (table[y][x] != 0) return false;
	return true;
}

void str7_Gomoku::print()const{
	cout << "  ";
	for (int i = 0; i<10; ++i) cout << i << " ";
	cout << endl;
	for (int i = 0; i<10; ++i){
		cout << i << " ";
		for (int j = 0; j<10; ++j){
			if (table[i][j] == 0)cout << "  ";
			else if (table[i][j] == 1) cout << "Ｘ ";
			else  cout << "◯ ";
			//		else cout << table[i][j] << "  ";
		}cout << endl;
	}
}

int str7_Gomoku::getColor(int y, int x){
	return table[y][x];
}

int str7_Gomoku::getNowColor()const{
	return color;
}





bool str7_onBoard(int y, int x){
	if (0 <= y && y < 10 && 0 <= x && x< 10)
		return true;

	return false;
}

str7_AI::str7_AI(){}

str7_AI::str7_AI(int color){
	myColor = color;
}

str7_AI::~str7_AI(){}


set<pair<int, int> > str7_AI::getPos(str7_Gomoku &gomoku, int color){
	set<pair<int, int> > res;
	res = getPriorityPos(gomoku, color);
	if (!res.empty()) return res;
	res = getAround(gomoku);
	if (!res.empty()) return res;
	res.insert(pair<int, int>(4, 4));
	return res;
}

set<pair<int, int> > str7_AI::getAround(str7_Gomoku &gomoku){
	set<pair<int, int> > se;

	const int dy[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	for (int i = 0; i<10; ++i){
		for (int j = 0; j<10; ++j){
			if (gomoku.getColor(i, j) != 0){
				for (int ddx = 0; ddx<8; ++ddx){
					for (int ddy = 0; ddy<8; ++ddy){
						int xx = j + dx[ddx];
						int yy = i + dy[ddy];
						if (gomoku.checkHit(yy, xx) == true) se.insert(pair<int, int>(yy, xx));
	/*					xx = j + dx[ddx] *2;
						yy = i + dy[ddy] *2;
						if(gomoku.checkHit(yy,xx) == true) se.insert(pair<int,int>(yy,xx));
						*/
					}
				}
			}
		}
	}

	return se;
}


//置いたら勝ち
//置かれたら負け
//自分が4のフリー作れる
//相手が４のフリー作れる

//置いたら勝ち(4つで一つ空き	0
//相手が4						1
//自分が3						2
//相手が3						3 
set<pair<int, int> > str7_AI::getPriorityPos(str7_Gomoku &gomoku, int myCol){
	set<pair<int, int> > res;
	int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	set<pair<int, int> > tmp[4];
	for (int y = 0; y<10; ++y){
		for (int x = 0; x<10; ++x){
			int color = gomoku.getColor(y, x);
			if (color == 0){
				for (int c = 1; c<3; ++c){
					if (gomoku.hit(y, x, c)){
						int index = 1;
						if (c == myCol) index = 0;
						tmp[index].insert(pair<int, int>(y, x));
						//		break;
					}
					else{
						for (int d = 0; d<4; ++d){
							int deathPoint = 0;
							int connect = 1;
							int ny = y + dy[d];
							int nx = x + dx[d];

							while (str7_onBoard(ny, nx) &&
								gomoku.getColor(ny, nx) == gomoku.getColor(y, x)){
								connect++;
								ny += dy[d];
								nx += dx[d];
							}
							if (!(str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == 0))
								deathPoint++;

							ny = y + dy[d + 4];
							nx = x + dx[d + 4];
							while (str7_onBoard(ny, nx) &&
								gomoku.getColor(ny, nx) == gomoku.getColor(y, x)){
								ny += dy[d + 4];
								nx += dx[d + 4];
								connect++;
							}

							if (!(str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == 0))
								deathPoint++;

							if (connect == 4 && deathPoint == 0){
								int index = 3;
								if (c == myCol) index = 2;
								tmp[index].insert(pair<int, int>(y, x));
								//				break;
							}
							//				cout << "(" << y << "," << x << ","<< connect<<"," << deathPoint<< ")" << endl;

						}
					}
					gomoku.hit(y, x, 0);
				}
			}

		}
	}
	for (int i = 0; i<4; ++i){
		if (!res.empty()) break;
		set<pair<int, int> >::iterator it = tmp[i].begin();
		while (it != tmp[i].end()){
			res.insert(*it);
			//		cout << i << endl;
			it++;
		}
	}
	return res;
}
	
int str7_AI::dfsab(str7_Gomoku &g,int color,int depth,int alph,int beta){
	if(depth == 4) return (int)eval(g,myColor);
	set<pair<int,int> > possible = getPos(g,getEnemyColor(color));
	set<pair<int,int> >::iterator it = possible.begin();
	int res = alph;
//	if(color != myColor) res  = beta;
	while(it != possible.end()){
		int turnColor = getEnemyColor(color);
		bool f = g.hit(it->first,it->second,turnColor);
		int tmp;
		if(f == true){
			g.hit(it->first,it->second,0);
			if(turnColor == myColor) return 100000;
			else return -100000;
		} 
		tmp = dfsab(g,turnColor,depth+1,alph,beta);
		g.hit(it->first,it->second,0);
		
		if(color != myColor){
			alph = max(alph,tmp);
			res = alph;
			if(res >= beta) return beta;
/*			if(res < tmp){
				res = tmp;
			}*/
		}else{
			beta = min(beta,tmp);
			res = beta;
			if(beta <= alph) return alph;
//			if(res > tmp) res = tmp;
		
		}
		it++;
	}
	return res;		

}


int str7_AI::dfs(str7_Gomoku &g, int color, int depth){
	if (depth == 3) return (int)eval(g, myColor);
	set<pair<int, int> > possible = getPos(g, getEnemyColor(color));
	//	g.print();	
	set<pair<int, int> >::iterator it = possible.begin();
	int res = 1000000;
	if (color != myColor) res = -100000000;
	while (it != possible.end()){
		int turnColor = getEnemyColor(color);
		bool f = g.hit(it->first, it->second, turnColor);
		int tmp;
		if (f == true){
			g.hit(it->first, it->second, 0);
			if (turnColor == myColor) return 100000;
			else return -100000;
			/*			if(enemyColor == myColor) tmp = 100000;
			else tmp =  -100000;
			*/
		}
		tmp = dfs(g, turnColor, depth + 1);
		g.hit(it->first, it->second, 0);
		//		if(depth == 0 ) cout << "(" <<it->first<< "," << it->second << ")"<< tmp << " ";
		if (color != myColor){
			if (res < tmp){
				res = tmp;
			}
		}
		else{
			if (res > tmp) res = tmp;

		}
		it++;
	}
	return res;
}


pair<int, int> str7_AI::getHit(str7_Gomoku g){
	set<pair<int,int> > possible = getPos(g,myColor);
	set<pair<int,int> >::iterator it = possible.begin();
	int res = -10000009;
	pair<int,int> resP(0,0);
	
	while(it != possible.end()){
		
		bool f = g.hit(it->first,it->second,myColor);
		if(f){
			g.hit(it->first,it->second,0);
			return pair<int,int>(it->first,it->second);
		}
//		cout << "dfs" << it->first << "," << it->second << endl;
//		int tmp = dfs(g,myColor,0);
		int tmp = dfsab(g,myColor,0,res,9999999);
		g.hit(it->first,it->second,0);
		if(res < tmp){
			resP =  *it;
			res = tmp;
		}
	//	cout << "("<<it->first << "," <<it->second << ") value:" << tmp <<endl;
		it++;
	}
	return resP;
	return pair<int,int>(4,4);	
}

int str7_AI::getEnemyColor(int color){
	if (color == 1) return 2;
	else return 1;
}

int str7_AI::getEnemyColor(){
	if (myColor == 1) return 2;
	return 1;
}

int str7_AI::getConnectN(str7_Gomoku &gomoku, int color){
	int res = 0;
	int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	//	set<pair<int,int> > se;
	for (int y = 0; y<10; ++y){
		for (int x = 0; x<10; ++x){
			if (gomoku.getColor(y, x) != color) continue;
			for (int d = 0; d<4; ++d){
				int deathPointN = 0;
				int connectN = 1;
				int backY = y + dy[d + 4];
				int backX = x + dx[d + 4];
				if (str7_onBoard(backY, backX)){
					if (gomoku.getColor(backY, backX) == color){
						continue;
					}
					else if (gomoku.getColor(backY, backX) == getEnemyColor(color)){
						deathPointN++;
					}
				}
				else deathPointN++;
				int nx = x + dx[d];
				int ny = y + dy[d];

				if (!(str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == color))
					continue;
				//				se.insert(pair<int,int>(y,x));
				while (str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == color){
					connectN++;
					nx += dx[d]; ny += dy[d];
				}
				if (!str7_onBoard(ny, nx) ||
					gomoku.getColor(ny, nx) == getEnemyColor(color)){
					deathPointN++;
				}

				int mul = 1;
				if (deathPointN == 2) mul = -1;
				else if (deathPointN == 0) mul = 2;

				if (connectN == 3){
					if (deathPointN == 2){
						res += int((connectN + 2) * 0.8);
					}
					else
						res += (connectN + 2) * mul;
				}
				else if (connectN == 4){
					if (deathPointN == 2){
						res += int((connectN + 2) * 0.8);
					}
					else if (deathPointN == 1){
						res += (connectN + 2);
					}
					else
						res += 100 * mul;
				}
				else if (connectN == 5) res += 5000;
				else res += connectN * mul;
			}
		}
	}

	return res;
}
double str7_AI::eval(str7_Gomoku &gomoku, int color){
	int my = getConnectN(gomoku, color);
	int enemy = getConnectN(gomoku, getEnemyColor(color));
	int res = int(my - 1.3*enemy);
	//	cout << "eval:" << my << ","<<enemy <<","<<res <<endl;
	return res;
	//	return getConnectN(gomoku,color) -2* getConnectN(gomoku,getEnemyColor(color));
}

double str7_AI::eval(str7_Gomoku &gomoku){
	return eval(gomoku, myColor);

}

	//------------------------------------------------
// ランダムに手を打つ戦略
//------------------------------------------------
void strategy7(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	static str7_Gomoku g;
	static int myColor = 2;
	if (count == 0) myColor = 1;
	static str7_AI ai(myColor);
//	AI(myColor);

	printf( "Strategy 7\t" );

	if (count != 0){
		int y = history[count - 1].y;
		int x = history[count - 1].x;
		g.hit(y, x);
	}

	pair<int,int> res = ai.getHit(g);
	g.hit(res.first, res.second);
	*pos_y = res.first;
	*pos_x = res.second;
	
	
	
	return;
}


//=======================================================
// Group 8
//=======================================================

int str8_gote(const int odp[100], const int count);
int str8_sente(const int odp[100], const int count);

void strategy8( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
  int i, j; //繰返し用
  int play_num; //手番の判定用
  int best_pos = 0; //最善手の一次元配列上での番号。この番号が一次元配列の添え字になる。
  int odp[100];
  int odp1[100];

	printf( "Strategy 8\t" );

  //手番の判定  
  if(count % 2 == 0){
    play_num = 0; //先手
  } else {
    play_num = 1; //後手
  }


  //盤面情報のコピー
  for(i=0; i<BOARD_SIZE; i++){ 
    for(j=0; j<BOARD_SIZE; j++){ 
      odp[j + 10 * i] = board[i][j]; //one dimension's position。盤面情報
    }
  }

  //先手でも自陣の石を2、敵陣の石を1とみなせるように書き換え
  if(play_num == 0){
    for(i=0; i<BOARD_SIZE; i++){ 
      for(j=0; j<BOARD_SIZE; j++){ 
	if(odp[j+i*10] == 1) odp1[j+i*10] = 2;
	if(odp[j+i*10] == 2) odp1[j+i*10] = 1;
	if(odp[j+i*10] == 0) odp1[j+i*10] = 0;
      }
    }
  
    for(i=0; i<BOARD_SIZE; i++){ 
      for(j=0; j<BOARD_SIZE; j++){ 
	odp[j+i*10] = odp1[j+i*10];
      }
    }
  }

   

  

  //石を置く場所の選択。今回は先手、後手で別々の関数を用いる。
  if(play_num == 0){
    //先手の時
    best_pos = str8_sente(odp, count);
  } else {
    //後手の時
    best_pos = str8_gote(odp, count);
  }

  *pos_x = best_pos % 10; 
  *pos_y = (best_pos - (best_pos % 10)) / 10;
}











//////////////////////////
////後手
//////////////////////////
int str8_gote(const int odp[100], const int count){



  int best_pos = 0; //0~99までの位置を記憶
  int i, j, k, k2, l; //ループ用
  
  int point_board[100][8]; //その手番での各マスの各方向に対する得点,右：[0]右下：[1]下：[2]左下：[3]左：[4]左上：[5]上：[6]右上：[7]
  int point_board2[100][8];
  int point[100]; //そのマスの総合得点
  int max_point; //その手番での得点の最大値

  int right_array[6]; //現在の検索位置を含めて、右方向に対する石の状況,3進数表記。でもこれもしかして現在の位置を含めて６個か７個にせんとむりか？それでもむりか？
  int right_array2[6];
  int right_arrays[100];//10進数表示
  int right_arrays2[100];
	
  int down_right_array[6]; //右下方向
  int down_right_array2[6];
  int down_right_arrays[100];
  int down_right_arrays2[100];
	
  int down_array[6]; //下方向
  int down_array2[6];
  int down_arrays[100];
  int down_arrays2[100];

  int down_left_array[6]; //左下方向
  int down_left_array2[6]; //左下方向
  int down_left_arrays[100];
  int down_left_arrays2[100];

  int left_array[6]; //左
  int left_array2[6]; //左
  int left_arrays[100];
  int left_arrays2[100];

  int up_left_array[6]; //左上
  int up_left_array2[6]; //左上
  int up_left_arrays[100];
  int up_left_arrays2[100];

  int up_array[6]; //上
  int up_array2[6]; //上
  int up_arrays[100];
  int up_arrays2[100];

  int up_right_array[6]; //右上
  int up_right_array2[6]; //右上
  int up_right_arrays[100];
  int up_right_arrays2[100];

  
  for(i=0; i<100; i++){
    right_arrays[i] = 0;
    down_right_arrays[i] = 0;
    down_arrays[i] = 0;
    down_left_arrays[i] = 0;
    left_arrays[i] = 0;
    up_left_arrays[i] = 0;
    up_arrays[i] = 0;
    up_right_arrays[i] = 0;
    right_arrays2[i] = 0;
    down_right_arrays2[i] = 0;
    down_arrays2[i] = 0;
    down_left_arrays2[i] = 0;
    left_arrays2[i] = 0;
    up_left_arrays2[i] = 0;
    up_arrays2[i] = 0;
    up_right_arrays2[i] = 0;
  }

  

  //各マスの得点を初期化
  for(i=0; i<100; i++){
    for(j=0; j<8; j++){
      point_board[i][j] = 0;
      point_board2[i][j] = 0;
    }
    point[i] = 0;
  }

  ////////////////////////////////////////////////////
  //各マスの右、右下、下、左下の並びを取得
  for(i=0; i<100; i++){
    if(odp[i] == STONE_WHITE){
      point_board[i][0] = -200;
      point_board[i][1] = -200;
      point_board[i][2] = -200;
      point_board[i][3] = -200;
      point_board[i][4] = -200;
      point_board[i][5] = -200;
      point_board[i][6] = -200;
      point_board[i][7] = -200;
      continue; //空欄でないなら選ばないようにする
    } else if(odp[i] == STONE_BLACK){
      point_board[i][0] = -300;
      point_board[i][1] = -300;
      point_board[i][2] = -300;
      point_board[i][3] = -300;
      point_board[i][4] = -300;
      point_board[i][5] = -300;
      point_board[i][6] = -300;
      point_board[i][7] = -300;
      continue;
    } else {
      //各方向の並び
      for(j=0; j<6; j++){
	right_array[j] = 0;
	down_right_array[j] = 0;
	down_array[j] = 0;
	down_left_array[j] = 0;
	left_array[j] = 0;
	up_left_array[j] = 0;
	up_array[j] = 0;
	up_right_array[j] = 0;
	right_array2[j] = 0;
	down_right_array2[j] = 0;
	down_array2[j] = 0;
	down_left_array2[j] = 0;
	left_array2[j] = 0;
	up_left_array2[j] = 0;
	up_array2[j] = 0;
	up_right_array2[j] = 0;
      }
    }



      //////////////////////////////////////////////
      //右方向に考える
      for(j=0; j<6; j++){
		k = i + j;
		k2 = i + (j-2);
	//もし次の列に行ってしまっても今回は止まらないのでそれ対策
	if(  (i - (i % 10)) == (k - (k % 10)) ){
	  right_array[j] = odp[k];
	} else {
	  //もし別の列に行ってしまう場合は、そこから右に5連はできないので0点
		if(j==5){
			point_board[i][0] = -1;
		} else {
			point_board[i][0] = -2; //そのマスの右方向は無視
		}
	  //continue;
	}

    if(  ((i - (i % 10)) == (k2 - (k2 % 10))) && (k2>=0)){
	  right_array2[j] = odp[k2];
      	} else{
	  //もし別の列に行ってしまう場合は、そこから右に5連はできないので0点
		if(j==5 || j==0){
			point_board2[i][0] = -1;
		} else {
			point_board2[i][0] = -2; //そのマスの右方向は無視
		}
	  //continue;
	}

      }
            
      ////////////////////////////////////////////////
      //右下方向
      for(j=0; j<6; j++){
	k = i + 11 * j;
      	k2 = i + 11 * (j-2);
	//行き過ぎて左側の行に行ってしまったとき対策
	if( (( k % 10) >= (i % 10)) && (k < 100) ){
	  down_right_array[j] = odp[k];
	} else {
		if(j==5){
			point_board[i][1] = -1;
		} else {
			point_board[i][1] = -2;
		}
	  //continue;
	}
	if( ((( k2 % 10) >= (i % 10)) && (k2 < 100) && (i-k2 < 0)) || ((( k2 % 10) <= (i % 10)) && (k2 >= 0) && (i-k2 >0)) ){
	  down_right_array2[j] = odp[k2];
	} else {
		if(j==5 || j==0){
			point_board2[i][1] = -1;
		} else {
			point_board2[i][1] = -2;
		}
	  //continue;
	}
      }

      ////////////////////////////////////////////////
      //下方向
      for(j=0; j<6; j++){
	k = i + 10 * j;
      	k2 = i + 10 * (j-2);
	//行き過ぎて99を超えてしまったとき対策
	if( k < 100 ){
	  down_array[j] = odp[k];
	} else {
		if(j==5){
			point_board[i][2] = -1;
		} else {
			point_board[i][2] = -2;
		}
	  //continue;
	}
	if( k2 < 100 && (k2 >= 0)){
	  down_array2[j] = odp[k2];
	} else {
		if(j==5 || j==0){
			point_board2[i][2] = -1;
		} else {
			point_board2[i][2] = -2;
		}
	  //continue;
	}
      }
      ///////////////////////////////////////////////
      //左下方向
      for(j=0; j<6; j++){
	k = i + 9 * j;
      	k2 = i + 9 * (j-2);
	//行き過ぎてしまったとき対策
	if( ( (k % 10) <= (i % 10) ) && (k < 100) ){
	  down_left_array[j] = odp[k];
	} else {
		if(j==5){
			point_board[i][3] = -1;
		} else {
			point_board[i][3] = -2;
		}
	  //continue;
	}
	if( ((( k2 % 10) <= (i % 10)) && (k2 < 100) && (i-k2 < 0)) || ((( k2 % 10) >= (i % 10)) && (k2 >= 0) && (i-k2 >0)) ){
	  down_left_array2[j] = odp[k2];
	} else {
		if(j==5|| j==0){
			point_board2[i][3] = -1;
		} else {
			point_board2[i][3] = -2;
		}
	  //continue;
	}
      }
      ///////////////////////////////////
      //左方向
      for(j=0; j<6; j++){
	k = i - j;
      	k2 = i - (j-2);
	//行き過ぎてしまったとき対策
	if( ((i - (i % 10))) == ((k - (k % 10))) && (k >= 0) ){
	  left_array[j] = odp[k];
	} else {
		if(j==5){
			point_board[i][4] = -1;
		} else {
			point_board[i][4] = -2;
		}
	  //continue;
	}
	if( ((i - (i % 10))) == ((k2 - (k2 % 10))) && (k2 >= 0) ){
	  left_array2[j] = odp[k2];
	} else {
		if(j==5 || j==0){
			point_board2[i][4] = -1;
		} else {
			point_board2[i][4] = -2;
		}
	  //continue;
	}
      }
      /////////////////////////////////
      //左上方向
      for(j=0; j<6; j++){
	k = i - 11 * j;
      	k2 = i - 11 * (j-2);
	//行き過ぎてしまったとき対策
	if(  ((k % 10) <= (i % 10)) && (k >= 0) ){
	  up_left_array[j] = odp[k];
	} else {
		if(j==5){
			point_board[i][5] = -1;
		} else {
			point_board[i][5] = -2;
		}
	  //continue;
	}
	if( ((( k2 % 10) <= (i % 10)) && (k2 > 0) && (i-k2 > 0)) || ((( k2 % 10) <= (i % 10)) && (k2 < 100) && (i-k2 < 0)) ){
	  up_left_array2[j] = odp[k2];
	} else {
		if(j==5 || j==0){
			point_board2[i][5] = -1;
		} else {
			point_board2[i][5] = -2;
		}
	  //continue;
	}
      }
      ///////////////////////////////
      //上
      for(j=0; j<6; j++){
	k = i - 10 * j;
      	k2 = i - 10 * (j-2);
	//行き過ぎてしまったとき対策
	if( k >= 0 ){
	  up_array[j] = odp[k];
	} else {
		if(j==5){
			point_board[i][6] = -1;
		} else {
			point_board[i][6] = -2;
		}
	  //continue;
	}
	if( k2 >= 0 && k2 <100){
	  up_array2[j] = odp[k2];
	} else {
		if(j==5 || j==0){
			point_board2[i][6] = -1;
		} else {
			point_board2[i][6] = -2;
		}
	  //continue;
	}
      }
      ////////////////////////////
      //右上
      for(j=0; j<6; j++){
	k = i - 9 * j;
      	k2 = i - 9 * (j-2);
	//行き過ぎてしまったとき対策
	if( ( (k % 10) >= (i % 10) ) && (k > 0) ){
	  up_right_array[j] = odp[k];
	} else {
	  //そこから左に5連はできないので0点
	  /*	  for(l=0; l<5; l++){
		  up_right_array[l] = 0;
		  }*/
		if(j==5){
			point_board[i][7] = -1;
		} else {
			point_board[i][7] = -2;
		}
	  //continue;
	}
	if( ((( k2 % 10) >= (i % 10)) && (k2 > 0) && (i-k2 > 0)) || ((( k2 % 10) <= (i % 10)) && (k2 <= 0) && (i-k2 <0)) ){
	  up_right_array2[j] = odp[k2];
	} else {
	  //そこから左に5連はできないので0点
	  /*	  for(l=0; l<5; l++){
		  up_right_array[l] = 0;
		  }*/
		if(j==5 || j==0){
			point_board2[i][7] = -1;
		} else {
			point_board2[i][7] = -2;
		}
	  //continue;
	}
      }

	


      /////////////////////////////////////////////////
      //各方向に対して連の状態とその点数を決める。3進数を10進数にする作業だと考える。
      for(j=0; j<6; j++){
	//スタート地点の計算(不要であると思われる)
	//スタート地点から1つ目の場所　3^4の位
	if(j == 0){
	  right_arrays[i] += right_array[0] * 243;
	  down_right_arrays[i] += down_right_array[0] * 243;
	  down_arrays[i] += down_array[0] * 243;
	  down_left_arrays[i] += down_left_array[0] * 243;
	  left_arrays[i] += left_array[0] * 243;
	  up_left_arrays[i] += up_left_array[0] * 243;
	  up_arrays[i] += up_array[0] * 243;
	  up_right_arrays[i] += up_right_array[0] * 243;
	  right_arrays2[i] += right_array2[0] * 243;
	  down_right_arrays2[i] += down_right_array2[0] * 243;
	  down_arrays2[i] += down_array2[0] * 243;
	  down_left_arrays2[i] += down_left_array2[0] * 243;
	  left_arrays2[i] += left_array2[0] * 243;
	  up_left_arrays2[i] += up_left_array2[0] * 243;
	  up_arrays2[i] += up_array2[0] * 243;
	  up_right_arrays2[i] += up_right_array2[0] * 243;
	}
		if(j == 1){
	  right_arrays[i] += right_array[1] * 81;
	  down_right_arrays[i] += down_right_array[1] * 81;
	  down_arrays[i] += down_array[1] * 81;
	  down_left_arrays[i] += down_left_array[1] * 81;
	  left_arrays[i] += left_array[1] * 81;
	  up_left_arrays[i] += up_left_array[1] * 81;
	  up_arrays[i] += up_array[1] * 81;
	  up_right_arrays[i] += up_right_array[1] * 81;
	  right_arrays2[i] += right_array2[1] * 81;
	  down_right_arrays2[i] += down_right_array2[1] * 81;
	  down_arrays2[i] += down_array2[1] * 81;
	  down_left_arrays2[i] += down_left_array2[1] * 81;
	  left_arrays2[i] += left_array2[1] * 81;
	  up_left_arrays2[i] += up_left_array2[1] * 81;
	  up_arrays2[i] += up_array2[1] * 81;
	  up_right_arrays2[i] += up_right_array2[1] * 81;
	}
	
	//スタート地点から2つ目の場所　3^3の位
	if(j == 2){
	  right_arrays[i] += right_array[2] * 27;
	  down_right_arrays[i] += down_right_array[2] * 27;
	  down_arrays[i] += down_array[2] * 27;
	  down_left_arrays[i] += down_left_array[2] * 27;
	  left_arrays[i] += left_array[2] * 27;
	  up_left_arrays[i] += up_left_array[2] * 27;
	  up_arrays[i] += up_array[2] * 27;
	  up_right_arrays[i] += up_right_array[2] * 27;
	  right_arrays2[i] += right_array2[2] * 27;
	  down_right_arrays2[i] += down_right_array2[2] * 27;
	  down_arrays2[i] += down_array2[2] * 27;
	  down_left_arrays2[i] += down_left_array2[2] * 27;
	  left_arrays2[i] += left_array2[2] * 27;
	  up_left_arrays2[i] += up_left_array2[2] * 27;
	  up_arrays2[i] += up_array2[2] * 27;
	  up_right_arrays2[i] += up_right_array2[2] * 27;
	}
	//スタート地点から3つ目の場所　3^2の位
	if(j == 3){
	  right_arrays[i] += right_array[3] * 9;
	  down_right_arrays[i] += down_right_array[3] * 9;
	  down_arrays[i] += down_array[3] * 9;
	  down_left_arrays[i] += down_left_array[3] * 9;
	  left_arrays[i] += left_array[3] * 9;
	  up_left_arrays[i] += up_left_array[3] * 9;
	  up_arrays[i] += up_array[3] * 9;
	  up_right_arrays[i] += up_right_array[3] * 9;
	  right_arrays2[i] += right_array2[3] * 9;
	  down_right_arrays2[i] += down_right_array2[3] * 9;
	  down_arrays2[i] += down_array2[3] * 9;
	  down_left_arrays2[i] += down_left_array2[3] * 9;
	  left_arrays2[i] += left_array2[3] * 9;
	  up_left_arrays2[i] += up_left_array2[3] * 9;
	  up_arrays2[i] += up_array2[3] * 9;
	  up_right_arrays2[i] += up_right_array2[3] * 9;
	}
	//スタート地点から4つ目の場所　3^1の位
	if(j == 4){
	  right_arrays[i] += right_array[4] * 3;
	  down_right_arrays[i] += down_right_array[4] * 3;
	  down_arrays[i] += down_array[4] * 3;
	  down_left_arrays[i] += down_left_array[4] * 3;
	  left_arrays[i] += left_array[4] * 3;
	  up_left_arrays[i] += up_left_array[4] * 3;
	  up_arrays[i] += up_array[4] * 3;
	  up_right_arrays[i] += up_right_array[4] * 3;
	  right_arrays2[i] += right_array2[4] * 3;
	  down_right_arrays2[i] += down_right_array2[4] * 3;
	  down_arrays2[i] += down_array2[4] * 3;
	  down_left_arrays2[i] += down_left_array2[4] * 3;
	  left_arrays2[i] += left_array2[4] * 3;
	  up_left_arrays2[i] += up_left_array2[4] * 3;
	  up_arrays2[i] += up_array2[4] * 3;
	  up_right_arrays2[i] += up_right_array2[4] * 3;
	}
	//スタート地点から5つ目の場所　3^0の位
	if(j == 5){
	  right_arrays[i] += right_array[5];
	  down_right_arrays[i] += down_right_array[5];
	  down_arrays[i] += down_array[5];
	  down_left_arrays[i] += down_left_array[5];
	  left_arrays[i] += left_array[5];
	  up_left_arrays[i] += up_left_array[5]; 
	  up_arrays[i] += up_array[5];
	  up_right_arrays[i] += up_right_array[5];	  
	  right_arrays2[i] += right_array2[5];
	  down_right_arrays2[i] += down_right_array2[5];
	  down_arrays2[i] += down_array2[5];
	  down_left_arrays2[i] += down_left_array2[5];
	  left_arrays2[i] += left_array2[5];
	  up_left_arrays2[i] += up_left_array2[5]; 
	  up_arrays2[i] += up_array2[5];
	  up_right_arrays2[i] += up_right_array2[5];	  
	}
      }
    

      //各配列について、それぞれの並びに関して得点を計算
      //3進数にしているのは、敵陣の石、自陣の石、空白の3種類全部の場合を考慮することを考えてのこと。
      //条件のifにより、途中で形によっては高い点数をつけられるように
      /////////////////右方向
      if(point_board[i][0] >= -1){
	switch(right_arrays[i]){
	case 188://020222
	  point_board[i+2][0] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i+3][0] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i+4][0] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][0] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][0] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i+2][0] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i+3][0] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i+4][0] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][0] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i+2][0] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i+3][0] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i+1][0] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i+2][0] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i+3][0] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i+1][0] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i+4][0] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][0] += 1000;//1000
	  break;
	/* case 201://021110
	  point_board[i+5][0] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i+3][0] += 100;//100
	  break;
	case 10://000101
	  point_board[i+4][0] += 100;//100
	  break;
	case 12://000110
	  point_board[i+2][0] += 100;//100
	  break;
	case 30://001010
	  point_board[i+3][0] += 100;//100
	  break;
	case 36://001100
	  point_board[i+1][0] += 100;//100
	  break;
	case 85://010011
	  point_board[i+2][0] += 100;//100
	  break;
	case 90://010100
	  point_board[i+2][0] += 100;//100
	  break;
	case 108://011000
	  point_board[i+3][0] += 100;//100
	  break;
	case 109://011001
	  point_board[i+3][0] += 100;//100
	  break;
	case 110://011002
	  point_board[i+3][0] += 100;//100
	  break;
	case 28://001001
	  point_board[i+3][0] += 10;//10
	  break;
	case 38://001102
	  point_board[i][0] += 10;//10
	  break;
	case 84://010010
	  point_board[i+2][0] += 10;//10
	  break;
	case 91://010101
	  point_board[i+2][0] += 10;//10
	  point_board[i+4][0] += 10;//10
	  break;
	case 95://010112
	  point_board[i][0] += 10;//10
	  point_board[i+2][0] += 10;//10
	  break;
	case 113://011012
	  point_board[i][0] += 10;//10
	  break;
	case 3://000010
	  point_board[i+3][0] += 1;//1
	  break;
	case 5://000012
	  point_board[i][0] += 1;//1
	  break;
	case 9://000100
	  point_board[i+2][0] += 1;//1
	  break;
	case 11://000102
	  point_board[i][0] += 1;//1
	  break;
	case 14://000112
	  point_board[i][0] += 1;//1
	  break;
	case 27://001000
	  point_board[i+1][0] += 1;//1
	  break;
	case 29://001002
	  point_board[i][0] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i+1][0] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][0] += 1;//1
	  break;
	case 81://010000
	  point_board[i][0] += 1;//1
	  break;
	case 82://010001
	  point_board[i][0] += 1;//1
	  point_board[i+2][0] += 1;//1
	  break;
	case 83://010002
	  point_board[i][0] += 1;//1
	  break;
	case 92://010102
	  point_board[i][0] += 1;//1
	  break;
	}
      }
      if(point_board2[i][0] >= -1){
	switch(right_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][0] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][0] += 50000;//5万
	  break;
	}
      }
      	

    	
    	///////////////右下方向
      if(point_board[i][1] >= -1){
	switch(down_right_arrays[i]){
	case 188://020222
	  point_board[i+22][1] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i+33][1] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i+44][1] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][1] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][1] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i+22][1] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i+33][1] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i+44][1] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][1] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i+22][1] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i+33][1] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i+11][1] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i+22][1] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i+33][1] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i+11][1] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i+44][1] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][1] += 1000;//1000
	  break;
	/* case 201:
	  point_board[i+55][1] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i+33][1] += 100;//100
	  break;
	case 10://000101
	  point_board[i+44][1] += 100;//100
	  break;
	case 12://000110
	  point_board[i+22][1] += 100;//100
	  break;
	case 30://001010
	  point_board[i+33][1] += 100;//100
	  break;
	case 36://001100
	  point_board[i+11][1] += 100;//100
	  break;
	case 85://010011
	  point_board[i+22][1] += 100;//100
	  break;
	case 90://010100
	  point_board[i+22][1] += 100;//100
	  break;
	case 108://011000
	  point_board[i+33][1] += 100;//100
	  break;
	case 109://011001
	  point_board[i+33][1] += 100;//100
	  break;
	case 110://011002
	  point_board[i+33][1] += 100;//100
	  break;
	case 28://001001
	  point_board[i+33][1] += 10;//10
	  break;
	case 38://001102
	  point_board[i][1] += 10;//10
	  break;
	case 84://010010
	  point_board[i+22][1] += 10;//10
	  break;
	case 91://010101
	  point_board[i+22][1] += 10;//10
	  point_board[i+44][1] += 10;//10
	  break;
	case 95://010112
	  point_board[i][1] += 10;//10
	  point_board[i+22][1] += 10;//10
	  break;
	case 113://011012
	  point_board[i][1] += 10;//10
	  break;
	case 3://000010
	  point_board[i+33][1] += 1;//1
	  break;
	case 5://000012
	  point_board[i][1] += 1;//1
	  break;
	case 9://000100
	  point_board[i+22][1] += 1;//1
	  break;
	case 11://000102
	  point_board[i][1] += 1;//1
	  break;
	case 14://000112
	  point_board[i][1] += 1;//1
	  break;
	case 27://001000
	  point_board[i+11][1] += 1;//1
	  break;
	case 29://001002
	  point_board[i][1] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i+11][1] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][1] += 1;//1
	  break;
	case 81://010000
	  point_board[i][1] += 1;//1
	  break;
	case 82://010001
	  point_board[i][1] += 1;//1
	  point_board[i+22][1] += 1;//1
	  break;
	case 83://010002
	  point_board[i][1] += 1;//1
	  break;
	case 92://010102
	  point_board[i][1] += 1;//1
	  break;
	}
      }
      if(point_board2[i][1] >= -1){
	switch(down_right_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][1] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][1] += 50000;//5万
	  break;
	}
      }

      ////////////////下方向
      if(point_board[i][2] >= -1){
	switch(down_arrays[i]){
	case 188://020222
	  point_board[i+20][2] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i+30][2] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i+40][2] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][2] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][2] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i+20][2] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i+30][2] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i+40][2] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][2] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i+20][2] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i+30][2] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i+10][2] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i+20][2] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i+30][2] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i+10][2] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i+40][2] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][2] += 1000;//1000
	  break;
	  /* case 201:
	  point_board[i+50][2] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i+30][2] += 100;//100
	  break;
	case 10://000101
	  point_board[i+40][2] += 100;//100
	  break;
	case 12://000110
	  point_board[i+20][2] += 100;//100
	  break;
	case 30://001010
	  point_board[i+30][2] += 100;//100
	  break;
	case 36://001100
	  point_board[i+10][2] += 100;//100
	  break;
	case 85://010011
	  point_board[i+20][2] += 100;//100
	  break;
	case 90://010100
	  point_board[i+20][2] += 100;//100
	  break;
	case 108://011000
	  point_board[i+20][2] += 100;//100
	  break;
	case 109://011001
	  point_board[i+30][2] += 100;//100
	  break;
	case 110://011002
	  point_board[i+30][2] += 100;//100
	  break;
	case 28://001001
	  point_board[i+30][2] += 10;//10
	  break;
	case 38://001102
	  point_board[i][2] += 10;//10
	  break;
	case 84://010010
	  point_board[i+20][2] += 10;//10
	  break;
	case 91://010101
	  point_board[i+20][2] += 10;//10
	  point_board[i+40][2] += 10;//10
	  break;
	case 95://010112
	  point_board[i][2] += 10;//10
	  point_board[i+20][2] += 10;//10
	  break;
	case 113://011012
	  point_board[i][2] += 10;//10
	  break;
	case 3://000010
	  point_board[i+30][2] += 1;//1
	  break;
	case 5://000012
	  point_board[i][2] += 1;//1
	  break;
	case 9://000100
	  point_board[i+20][2] += 1;//1
	  break;
	case 11://000102
	  point_board[i][2] += 1;//1
	  break;
	case 14://000112
	  point_board[i][2] += 1;//1
	  break;
	case 27://001000
	  point_board[i+10][2] += 1;//1
	  break;
	case 29://001002
	  point_board[i][2] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i+10][2] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][2] += 1;//1
	  break;
	case 81://010000
	  point_board[i][2] += 1;//1
	  break;
	case 82://010001
	  point_board[i][2] += 1;//1
	  point_board[i+20][2] += 1;//1
	  break;
	case 83://010002
	  point_board[i][2] += 1;//1
	  break;
	case 92://010102
	  point_board[i][2] += 1;//1
	  break;
	}
      }
      if(point_board2[i][2] >= -1){
	switch(down_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][2] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][2] += 50000;//5万
	  break;
	}
      }


      //////////////////////左下方向
      if(point_board[i][3] >= -1){
	switch(down_left_arrays[i]){
	case 188://020222
	  point_board[i+18][3] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i+27][3] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i+36][3] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][3] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][3] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i+18][3] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i+27][3] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i+36][3] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][3] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i+18][3] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i+27][3] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i+9][3] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i+18][3] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i+27][3] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i+9][3] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i+36][3] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][3] += 1000;//1000
	  break;
	  /* case 201:
	  point_board[i+45][3] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i+27][3] += 100;//100
	  break;
	case 10://000101
	  point_board[i+36][3] += 100;//100
	  break;
	case 12://000110
	  point_board[i+18][3] += 100;//100
	  break;
	case 30://001010
	  point_board[i+27][3] += 100;//100
	  break;
	case 36://001100
	  point_board[i+9][3] += 100;//100
	  break;
	case 85://010011
	  point_board[i+18][3] += 100;//100
	  break;
	case 90://010100
	  point_board[i+18][3] += 100;//100
	  break;
	case 108://011000
	  point_board[i+27][3] += 100;//100
	  break;
	case 109://011001
	  point_board[i+27][3] += 100;//100
	  break;
	case 110://011002
	  point_board[i+27][3] += 100;//100
	  break;
	case 28://001001
	  point_board[i+27][3] += 10;//10
	  break;
	case 38://001102
	  point_board[i][3] += 10;//10
	  break;
	case 84://010010
	  point_board[i+18][3] += 10;//10
	  break;
	case 91://010101
	  point_board[i+18][3] += 10;//10
	  point_board[i+36][3] += 10;//10
	  break;
	case 95://010112
	  point_board[i][3] += 10;//10
	  point_board[i+18][3] += 10;//10
	  break;
	case 113://011012
	  point_board[i][3] += 10;//10
	  break;
	case 3://000010
	  point_board[i+27][3] += 1;//1
	  break;
	case 5://000012
	  point_board[i][3] += 1;//1
	  break;
	case 9://000100
	  point_board[i+18][3] += 1;//1
	  break;
	case 11://000102
	  point_board[i][3] += 1;//1
	  break;
	case 14://000112
	  point_board[i][3] += 1;//1
	  break;
	case 27://001000
	  point_board[i+9][3] += 1;//1
	  break;
	case 29://001002
	  point_board[i][3] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i+9][3] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][3] += 1;//1
	  break;
	case 81://010000
	  point_board[i][3] += 1;//1
	  break;
	case 82://010001
	  point_board[i][3] += 1;//1
	  point_board[i+18][3] += 1;//1
	  break;
	case 83://010002
	  point_board[i][3] += 1;//1
	  break;
	case 92://010102
	  point_board[i][3] += 1;//1
	  break;
	}
      }
      if(point_board2[i][3] >= -1){
	switch(down_left_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][3] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][3] += 50000;//5万
	  break;
	}
      }

      /////////////////////左方向
      if(point_board[i][4] >= -1){
	switch(left_arrays[i]){
	case 188://020222
	  point_board[i-2][4] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i-3][4] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i-4][4] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][4] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][4] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i-2][4] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i-3][4] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i-4][4] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][4] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i-2][4] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i-3][4] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i-1][4] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i-2][4] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i-3][4] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i-1][4] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i-4][4] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][4] += 1000;//1000
	  break;
	  /* case 201:
	  point_board[i-5][4] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i-3][4] += 100;//100
	  break;
	case 10://000101
	  point_board[i-4][4] += 100;//100
	  break;
	case 12://000110
	  point_board[i-2][4] += 100;//100
	  break;
	case 30://001010
	  point_board[i-3][4] += 100;//100
	  break;
	case 36://001100
	  point_board[i-1][4] += 100;//100
	  break;
	case 85://010011
	  point_board[i-2][4] += 100;//100
	  break;
	case 90://010100
	  point_board[i-2][4] += 100;//100
	  break;
	case 108://011000
	  point_board[i-3][4] += 100;//100
	  break;
	case 109://011001
	  point_board[i-3][4] += 100;//100
	  break;
	case 110://011002
	  point_board[i-3][4] += 100;//100
	  break;
	case 28://001001
	  point_board[i-3][4] += 10;//10
	  break;
	case 38://001102
	  point_board[i][4] += 10;//10
	  break;
	case 84://010010
	  point_board[i-2][4] += 10;//10
	  break;
	case 91://010101
	  point_board[i-2][4] += 10;//10
	  point_board[i-4][4] += 10;//10
	  break;
	case 95://010112
	  point_board[i][4] += 10;//10
	  point_board[i-2][4] += 10;//10
	  break;
	case 113://011012
	  point_board[i][4] += 10;//10
	  break;
	case 3://000010
	  point_board[i-3][4] += 1;//1
	  break;
	case 5://000012
	  point_board[i][4] += 1;//1
	  break;
	case 9://000100
	  point_board[i-2][4] += 1;//1
	  break;
	case 11://000102
	  point_board[i][4] += 1;//1
	  break;
	case 14://000112
	  point_board[i][4] += 1;//1
	  break;
	case 27://001000
	  point_board[i-1][4] += 1;//1
	  break;
	case 29://001002
	  point_board[i][4] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i-1][4] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][4] += 1;//1
	  break;
	case 81://010000
	  point_board[i][4] += 1;//1
	  break;
	case 82://010001
	  point_board[i][4] += 1;//1
	  point_board[i-2][4] += 1;//1
	  break;
	case 83://010002
	  point_board[i][4] += 1;//1
	  break;
	case 92://010102
	  point_board[i][4] += 1;//1
	  break;
	}
      }
      if(point_board2[i][4] >= -1){
	switch(left_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][4] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][4] += 50000;//5万
	  break;
	}
      }

      //////////////////////左上方向
      if(point_board[i][5] >= -1){
	switch(up_left_arrays[i]){
	case 188://020222
	  point_board[i-22][5] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i-33][5] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i-44][5] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][5] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][5] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i-22][5] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i-33][5] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i-44][5] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][5] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i-22][5] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i-33][5] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i-11][5] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i-22][5] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i-33][5] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i-11][5] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i-44][5] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][5] += 1000;//1000
	  break;
	  /* case 201:
	  point_board[i-55][5] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i-33][5] += 100;//100
	  break;
	case 10://000101
	  point_board[i-44][5] += 100;//100
	  break;
	case 12://000110
	  point_board[i-22][5] += 100;//100
	  break;
	case 30://001010
	  point_board[i-33][5] += 100;//100
	  break;
	case 36://001100
	  point_board[i-11][5] += 100;//100
	  break;
	case 85://010011
	  point_board[i-22][5] += 100;//100
	  break;
	case 90://010100
	  point_board[i-22][5] += 100;//100
	  break;
	case 108://011000
	  point_board[i-33][5] += 100;//100
	  break;
	case 109://011001
	  point_board[i-33][5] += 100;//100
	  break;
	case 110://011002
	  point_board[i-33][5] += 100;//100
	  break;
	case 28://001001
	  point_board[i-33][5] += 10;//10
	  break;
	case 38://001102
	  point_board[i][5] += 10;//10
	  break;
	case 84://010010
	  point_board[i-22][5] += 10;//10
	  break;
	case 91://010101
	  point_board[i-22][5] += 10;//10
	  point_board[i-44][5] += 10;//10
	  break;
	case 95://010112
	  point_board[i][5] += 10;//10
	  point_board[i-22][5] += 10;//10
	  break;
	case 113://011012
	  point_board[i][5] += 10;//10
	  break;
	case 3://000010
	  point_board[i-33][5] += 1;//1
	  break;
	case 5://000012
	  point_board[i][5] += 1;//1
	  break;
	case 9://000100
	  point_board[i-22][5] += 1;//1
	  break;
	case 11://000102
	  point_board[i][5] += 1;//1
	  break;
	case 14://000112
	  point_board[i][5] += 1;//1
	  break;
	case 27://001000
	  point_board[i-11][5] += 1;//1
	  break;
	case 29://001002
	  point_board[i][5] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i-11][5] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][5] += 1;//1
	  break;
	case 81://010000
	  point_board[i][5] += 1;//1
	  break;
	case 82://010001
	  point_board[i][5] += 1;//1
	  point_board[i-22][5] += 1;//1
	  break;
	case 83://010002
	  point_board[i][5] += 1;//1
	  break;
	case 92://010102
	  point_board[i][5] += 1;//1
	  break;
	}
      }
      if(point_board2[i][5] >= -1){
	switch(up_left_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][5] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][5] += 50000;//5万
	  break;
	}
      }

      ///////////////////上方向
      if(point_board[i][6] >= -1){
	switch(up_arrays[i]){
	case 188://020222
	  point_board[i-20][6] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i-30][6] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i-40][6] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][6] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][6] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i-20][6] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i-30][6] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i-40][6] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][6] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i-20][6] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i-30][6] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i-10][6] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i-20][6] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i-30][6] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i-10][6] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i-40][6] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][6] += 1000;//1000
	  break;
	  /* case 201:///////021110
	  point_board[i-50][6] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i-30][6] += 100;//100
	  break;
	case 10://000101
	  point_board[i-40][6] += 100;//100
	  break;
	case 12://000110
	  point_board[i-20][6] += 100;//100
	  break;
	case 30://001010
	  point_board[i-30][6] += 100;//100
	  break;
	case 36://001100
	  point_board[i-10][6] += 100;//100
	  break;
	case 85://010011
	  point_board[i-20][6] += 100;//100
	  break;
	case 90://010100
	  point_board[i-20][6] += 100;//100
	  break;
	case 108://011000
	  point_board[i-30][6] += 100;//100
	  break;
	case 109://011001
	  point_board[i-30][6] += 100;//100
	  break;
	case 110://011002
	  point_board[i-30][6] += 100;//100
	  break;
	case 28://001001
	  point_board[i-30][6] += 10;//10
	  break;
	case 38://001102
	  point_board[i][6] += 10;//10
	  break;
	case 84://010010
	  point_board[i-20][6] += 10;//10
	  break;
	case 91://010101
	  point_board[i-20][6] += 10;//10
	  point_board[i-40][6] += 10;//10
	  break;
	case 95://010112
	  point_board[i][6] += 10;//10
	  point_board[i-20][6] += 10;//10
	  break;
	case 113://011012
	  point_board[i][6] += 10;//10
	  break;
	case 3://000010
	  point_board[i-30][6] += 1;//1
	  break;
	case 5://000012
	  point_board[i][6] += 1;//1
	  break;
	case 9://000100
	  point_board[i-20][6] += 1;//1
	  break;
	case 11://000102
	  point_board[i][6] += 1;//1
	  break;
	case 14://000112
	  point_board[i][6] += 1;//1
	  break;
	case 27://001000
	  point_board[i-10][6] += 1;//1
	  break;
	case 29://001002
	  point_board[i][6] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i-10][6] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][6] += 1;//1
	  break;
	case 81://010000
	  point_board[i][6] += 1;//1
	  break;
	case 82://010001
	  point_board[i][6] += 1;//1
	  point_board[i-20][6] += 1;//1
	  break;
	case 83://010002
	  point_board[i][6] += 1;//1
	  break;
	case 92://010102
	  point_board[i][6] += 1;//1
	  break;
	}
      }
      if(point_board2[i][6] >= -1){
	switch(up_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][6] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][6] += 50000;//5万
	  break;
	}
      }

      ///////////////////右上方向
      if(point_board[i][7] >= -1){
	switch(up_right_arrays[i]){
	case 188://020222
	  point_board[i-18][7] += 100000;//10万
	  break;
	case 220://022022
	  point_board[i-27][7] += 100000;//10万
	  break;
	case 227://022202
	  point_board[i-36][7] += 100000;//10万
	  break;
	case 240://022220
	  point_board[i][7] += 100000;//10万
	  break;
	case 241://022221
	  point_board[i][7] += 100000;//10万
	  break;
	case 94://010111
	  point_board[i-18][7] += 50000;//5万
	  break;
	case 112://011011
	  point_board[i-27][7] += 50000;//5万
	  break;
	case 118://011101
	  point_board[i-36][7] += 50000;//5万
	  break;
	case 122://011112
	  point_board[i][7] += 50000;//5万
	  break;
/*	case 13://000111
	  point_board[i-18][7] += 1000;//1000
	  break;

	case 31://001011
	  point_board[i-27][7] += 1000;//1000
	  break;
*/
	case 37://001101
	  point_board[i-9][7] += 2000;//2000
	  break;
	case 93://010110
	  point_board[i-18][7] += 20000;//2万
	  break;
	case 111://011010
	  point_board[i-27][7] += 20000;//2万
	  break;
	case 39://001110
	  point_board[i-9][7] += 20000;//2万
	  break;
	case 117://011100
	  point_board[i-36][7] += 20000;//2万
	  break;
	case 119://011102
	  point_board[i][7] += 1000;//1000
	  break;
	  /* case 201:
	  point_board[i-45][7] += 1000;//1000
	  break;
*/	case 4://000011
	  point_board[i-27][7] += 100;//100
	  break;
	case 10://000101
	  point_board[i-36][7] += 100;//100
	  break;
	case 12://000110
	  point_board[i-18][7] += 100;//100
	  break;
	case 30://001010
	  point_board[i-27][7] += 100;//100
	  break;
	case 36://001100
	  point_board[i-9][7] += 100;//100
	  break;
	case 85://010011
	  point_board[i-18][7] += 100;//100
	  break;
	case 90://010100
	  point_board[i-18][7] += 100;//100
	  break;
	case 108://011000
	  point_board[i-27][7] += 100;//100
	  break;
	case 109://011001
	  point_board[i-27][7] += 100;//100
	  break;
	case 110://011002
	  point_board[i-27][7] += 100;//100
	  break;
	case 28://001001
	  point_board[i-27][7] += 10;//10
	  break;
	case 38://001102
	  point_board[i][7] += 10;//10
	  break;
	case 84://010010
	  point_board[i-18][7] += 10;//10
	  break;
	case 91://010101
	  point_board[i-18][7] += 10;//10
	  point_board[i-36][7] += 10;//10
	  break;
	case 95://010112
	  point_board[i][7] += 10;//10
	  point_board[i-18][7] += 10;//10
	  break;
	case 113://011012
	  point_board[i][7] += 10;//10
	  break;
	case 3://000010
	  point_board[i-27][7] += 1;//1
	  break;
	case 5://000012
	  point_board[i][7] += 1;//1
	  break;
	case 9://000100
	  point_board[i-18][7] += 1;//1
	  break;
	case 11://000102
	  point_board[i][7] += 1;//1
	  break;
	case 14://000112
	  point_board[i][7] += 1;//1
	  break;
	case 27://001000
	  point_board[i-9][7] += 1;//1
	  break;
	case 29://001002
	  point_board[i][7] += 1;//1
	  break;
	case 41:////////////////001112
	  point_board[i-9][7] += 1;/////1
	  break;
	case 32://001012
	  point_board[i][7] += 1;//1
	  break;
	case 81://010000
	  point_board[i][7] += 1;//1
	  break;
	case 82://010001
	  point_board[i][7] += 1;//1
	  point_board[i-18][7] += 1;//1
	  break;
	case 83://010002
	  point_board[i][7] += 1;//1
	  break;
	case 92://010102
	  point_board[i][7] += 1;//1
	  break;
	}
      }  
	  if(point_board2[i][7] >= -1){
	switch(up_right_arrays2[i]){
	case 672://220220
	case 673://220221
	case 674://220222
	case 188://020222
	case 431://120222
	  point_board[i][7] += 100000;//10万
	  break;
	case 336://110110
	case 337://110111
	case 338://110112
	case 94 ://010111
	case 580://210111
	  point_board[i][7] += 50000;//5万
	  break;
	}
	  }

        
  }
/*  printf("\n");
  for(i=0; i<100; i++){
	if(i%10==0){printf("%2d\n",i);}
	for(j=0;j<8;j++){
    	printf("%6d", point_board[i][j]);
    	if(j==7){printf("\n");}
    }
    if(i%10 == 9) printf("\n", point_board[i][j]);
   
  }
*/   
  //各マスの総合得点を計算
  for(i=0; i<100; i++){
    for(j=0; j<8; j++){
      point[i] += point_board[i][j];
    }
  }
  int cnt = 2;
  max_point = -1000;
  //もっとも点数の高いポイントを計算
  for(i=0; i<100; i++){
    if(point[i] > max_point){
      cnt ++;
      best_pos = i;
      max_point = point[i];
    }else if(point[i] == max_point){
      if(rand()%cnt == 0){
	cnt++;
	best_pos = i;
	max_point = point[i];
      }
    }
  }

  
    
  printf("\n");
  for(i=0; i<100; i++){
    printf("%6d", point[i]);
    if(i%10 == 9) printf("\n");
   
  }
  
  printf("\n");
  return best_pos;
}











/////////////////////////////
/////////先手
//////////////////////////////

int str8_sente(const int odp[100], const int count){

  //１手目の作戦
  if(count == 0){
    int center[4] = {44, 45, 54, 55};
    int sel_first = rand()%4;
    return center[sel_first];
  } else {


	  int best_pos = 0; //0~99までの位置を記憶
	  int i, j, k, k2; //ループ用
	  
	  int point_board[100][8]; //その手番での各マスの各方向に対する得点,右：[0]右下：[1]下：[2]左下：[3]左：[4]左上：[5]上：[6]右上：[7]
	  int point_board2[100][8];
	  int point[100]; //そのマスの総合得点
	  int max_point; //その手番での得点の最大値

	  int right_array[6]; //現在の検索位置を含めて、右方向に対する石の状況,3進数表記。でもこれもしかして現在の位置を含めて６個か７個にせんとむりか？それでもむりか？
	  int right_array2[6];
	  int right_arrays[100];//10進数表示
	  int right_arrays2[100];
		
	  int down_right_array[6]; //右下方向
	  int down_right_array2[6];
	  int down_right_arrays[100];
	  int down_right_arrays2[100];
		
	  int down_array[6]; //下方向
	  int down_array2[6];
	  int down_arrays[100];
	  int down_arrays2[100];

	  int down_left_array[6]; //左下方向
	  int down_left_array2[6]; //左下方向
	  int down_left_arrays[100];
	  int down_left_arrays2[100];

	  int left_array[6]; //左
	  int left_array2[6]; //左
	  int left_arrays[100];
	  int left_arrays2[100];

	  int up_left_array[6]; //左上
	  int up_left_array2[6]; //左上
	  int up_left_arrays[100];
	  int up_left_arrays2[100];

	  int up_array[6]; //上
	  int up_array2[6]; //上
	  int up_arrays[100];
	  int up_arrays2[100];

	  int up_right_array[6]; //右上
	  int up_right_array2[6]; //右上
	  int up_right_arrays[100];
	  int up_right_arrays2[100];

	  
	  for(i=0; i<100; i++){
	    right_arrays[i] = 0;
	    down_right_arrays[i] = 0;
	    down_arrays[i] = 0;
	    down_left_arrays[i] = 0;
	    left_arrays[i] = 0;
	    up_left_arrays[i] = 0;
	    up_arrays[i] = 0;
	    up_right_arrays[i] = 0;
	    right_arrays2[i] = 0;
	    down_right_arrays2[i] = 0;
	    down_arrays2[i] = 0;
	    down_left_arrays2[i] = 0;
	    left_arrays2[i] = 0;
	    up_left_arrays2[i] = 0;
	    up_arrays2[i] = 0;
	    up_right_arrays2[i] = 0;
	  }

	  

	  //各マスの得点を初期化
	  for(i=0; i<100; i++){
	    for(j=0; j<8; j++){
	      point_board[i][j] = 0;
	      point_board2[i][j] = 0;
	    }
	    point[i] = 0;
	  }

	  ////////////////////////////////////////////////////
	  //各マスの右、右下、下、左下の並びを取得
	  for(i=0; i<100; i++){
	    if(odp[i] == STONE_WHITE){
	      point_board[i][0] = -200;
	      point_board[i][1] = -200;
	      point_board[i][2] = -200;
	      point_board[i][3] = -200;
	      point_board[i][4] = -200;
	      point_board[i][5] = -200;
	      point_board[i][6] = -200;
	      point_board[i][7] = -200;
	      continue; //空欄でないなら選ばないようにする
	    } else if(odp[i] == STONE_BLACK){
	      point_board[i][0] = -300;
	      point_board[i][1] = -300;
	      point_board[i][2] = -300;
	      point_board[i][3] = -300;
	      point_board[i][4] = -300;
	      point_board[i][5] = -300;
	      point_board[i][6] = -300;
	      point_board[i][7] = -300;
	      continue;
	    } else {
	      //各方向の並び
	      for(j=0; j<6; j++){
		right_array[j] = 0;
		down_right_array[j] = 0;
		down_array[j] = 0;
		down_left_array[j] = 0;
		left_array[j] = 0;
		up_left_array[j] = 0;
		up_array[j] = 0;
		up_right_array[j] = 0;
		right_array2[j] = 0;
		down_right_array2[j] = 0;
		down_array2[j] = 0;
		down_left_array2[j] = 0;
		left_array2[j] = 0;
		up_left_array2[j] = 0;
		up_array2[j] = 0;
		up_right_array2[j] = 0;
	      }
	    }



	      //////////////////////////////////////////////
	      //右方向に考える
	      for(j=0; j<6; j++){
			k = i + j;
			k2 = i + (j-2);
		//もし次の列に行ってしまっても今回は止まらないのでそれ対策
		if(  ((i - (i % 10)) == (k - (k % 10))) && (k<100)){
		  right_array[j] = odp[k];
		} else {
		  //もし別の列に行ってしまう場合は、そこから右に5連はできないので0点
			if(j==5){
				point_board[i][0] = -1;
			} else {
				point_board[i][0] = -2; //そのマスの右方向は無視
			}
		  //continue;
		}

	    if(  ((i - (i % 10)) == (k2 - (k2 % 10))) && (k2>=0) && (k2<100)){
		  right_array2[j] = odp[k2];
	      	} else{
		  //もし別の列に行ってしまう場合は、そこから右に5連はできないので0点
			if(j==5 || j==0){
				point_board2[i][0] = -1;
			} else {
				point_board2[i][0] = -2; //そのマスの右方向は無視
			}
		  //continue;
		}

	      }
	            
	      ////////////////////////////////////////////////
	      //右下方向
	      for(j=0; j<6; j++){
		k = i + 11 * j;
	      	k2 = i + 11 * (j-2);
		//行き過ぎて左側の行に行ってしまったとき対策
		if( (( k % 10) >= (i % 10)) && (k < 100) ){
		  down_right_array[j] = odp[k];
		} else {
			if(j==5){
				point_board[i][1] = -1;
			} else {
				point_board[i][1] = -2;
			}
		  //continue;
		}
		if( ((( k2 % 10) >= (i % 10)) && (k2 < 100) && (i-k2 < 0)) || ((( k2 % 10) <= (i % 10)) && (k2 >= 0) && (i-k2 >0)) ){
		  down_right_array2[j] = odp[k2];
		} else {
			if(j==5 || j==0){
				point_board2[i][1] = -1;
			} else {
				point_board2[i][1] = -2;
			}
		  //continue;
		}
	      }

	      ////////////////////////////////////////////////
	      //下方向
	      for(j=0; j<6; j++){
		k = i + 10 * j;
	      	k2 = i + 10 * (j-2);
		//行き過ぎて99を超えてしまったとき対策
		if( k < 100 ){
		  down_array[j] = odp[k];
		} else {
			if(j==5){
				point_board[i][2] = -1;
			} else {
				point_board[i][2] = -2;
			}
		  //continue;
		}
		if( k2 < 100 && (k2 >= 0)){
		  down_array2[j] = odp[k2];
		} else {
			if(j==5 || j==0){
				point_board2[i][2] = -1;
			} else {
				point_board2[i][2] = -2;
			}
		  //continue;
		}
	      }
	      ///////////////////////////////////////////////
	      //左下方向
	      for(j=0; j<6; j++){
		k = i + 9 * j;
	      	k2 = i + 9 * (j-2);
		//行き過ぎてしまったとき対策
		if( ( (k % 10) <= (i % 10) ) && (k < 100) ){
		  down_left_array[j] = odp[k];
		} else {
			if(j==5){
				point_board[i][3] = -1;
			} else {
				point_board[i][3] = -2;
			}
		  //continue;
		}
		if( ((( k2 % 10) <= (i % 10)) && (k2 < 100) && (i-k2 < 0)) || ((( k2 % 10) >= (i % 10)) && (k2 >= 0) && (i-k2 >0)) ){
		  down_left_array2[j] = odp[k2];
		} else {
			if(j==5 || j==0){
				point_board2[i][3] = -1;
			} else {
				point_board2[i][3] = -2;
			}
		  //continue;
		}
	      }
	      ///////////////////////////////////
	      //左方向
	      for(j=0; j<6; j++){
		k = i - j;
	      	k2 = i - (j-2);
		//行き過ぎてしまったとき対策
		if( ((i - (i % 10))) == ((k - (k % 10))) && (k >= 0) ){
		  left_array[j] = odp[k];
		} else {
			if(j==5){
				point_board[i][4] = -1;
			} else {
				point_board[i][4] = -2;
			}
		  //continue;
		}
		if( ((i - (i % 10))) == ((k2 - (k2 % 10))) && (k2 >= 0) ){
		  left_array2[j] = odp[k2];
		} else {
			if(j==5 || j==0){
				point_board2[i][4] = -1;
			} else {
				point_board2[i][4] = -2;
			}
		  //continue;
		}
	      }
	      /////////////////////////////////
	      //左上方向
	      for(j=0; j<6; j++){
		k = i - 11 * j;
	      	k2 = i - 11 * (j-2);
		//行き過ぎてしまったとき対策
		if(  ((k % 10) <= (i % 10)) && (k >= 0) ){
		  up_left_array[j] = odp[k];
		} else {
			if(j==5){
				point_board[i][5] = -1;
			} else {
				point_board[i][5] = -2;
			}
		  //continue;
		}
		if( ((( k2 % 10) <= (i % 10)) && (k2 > 0) && (i-k2 > 0)) || ((( k2 % 10) <= (i % 10)) && (k2 < 100) && (i-k2 < 0)) ){
		  up_left_array2[j] = odp[k2];
		} else {
			if(j==5 || j==0){
				point_board2[i][5] = -1;
			} else {
				point_board2[i][5] = -2;
			}
		  //continue;
		}
	      }
	      ///////////////////////////////
	      //上
	      for(j=0; j<6; j++){
		k = i - 10 * j;
	      	k2 = i - 10 * (j-2);
		//行き過ぎてしまったとき対策
		if( k >= 0 ){
		  up_array[j] = odp[k];
		} else {
			if(j==5){
				point_board[i][6] = -1;
			} else {
				point_board[i][6] = -2;
			}
		  //continue;
		}
		if( k2 >= 0 && k2 <100){
		  up_array2[j] = odp[k2];
		} else {
			if(j==5 || j==0){
				point_board2[i][6] = -1;
			} else {
				point_board2[i][6] = -2;
			}
		  //continue;
		}
	      }
	      ////////////////////////////
	      //右上
	      for(j=0; j<6; j++){
		k = i - 9 * j;
	      	k2 = i - 9 * (j-2);
		//行き過ぎてしまったとき対策
		if( ( (k % 10) >= (i % 10) ) && (k > 0) ){
		  up_right_array[j] = odp[k];
		} else {
		  //そこから左に5連はできないので0点
		  /*	  for(l=0; l<5; l++){
			  up_right_array[l] = 0;
			  }*/
			if(j==5){
				point_board[i][7] = -1;
			} else {
				point_board[i][7] = -2;
			}
		  //continue;
		}
		if( ((( k2 % 10) >= (i % 10)) && (k2 > 0) && (i-k2 > 0)) || ((( k2 % 10) <= (i % 10)) && (k2 <= 0) && (i-k2 <0)) ){
		  up_right_array2[j] = odp[k2];
		} else {
		  //そこから左に5連はできないので0点
		  /*	  for(l=0; l<5; l++){
			  up_right_array[l] = 0;
			  }*/
			if(j==5 || j==0){
				point_board2[i][7] = -1;
			} else {
				point_board2[i][7] = -2;
			}
		  //continue;
		}
	      }

		


	      /////////////////////////////////////////////////
	      //各方向に対して連の状態とその点数を決める。3進数を10進数にする作業だと考える。
	      for(j=0; j<6; j++){
		//スタート地点の計算(不要であると思われる)
		//スタート地点から1つ目の場所　3^4の位
		if(j == 0){
		  right_arrays[i] += right_array[0] * 243;
		  down_right_arrays[i] += down_right_array[0] * 243;
		  down_arrays[i] += down_array[0] * 243;
		  down_left_arrays[i] += down_left_array[0] * 243;
		  left_arrays[i] += left_array[0] * 243;
		  up_left_arrays[i] += up_left_array[0] * 243;
		  up_arrays[i] += up_array[0] * 243;
		  up_right_arrays[i] += up_right_array[0] * 243;
		  right_arrays2[i] += right_array2[0] * 243;
		  down_right_arrays2[i] += down_right_array2[0] * 243;
		  down_arrays2[i] += down_array2[0] * 243;
		  down_left_arrays2[i] += down_left_array2[0] * 243;
		  left_arrays2[i] += left_array2[0] * 243;
		  up_left_arrays2[i] += up_left_array2[0] * 243;
		  up_arrays2[i] += up_array2[0] * 243;
		  up_right_arrays2[i] += up_right_array2[0] * 243;
		}
		if(j == 1){
		  right_arrays[i] += right_array[1] * 81;
		  down_right_arrays[i] += down_right_array[1] * 81;
		  down_arrays[i] += down_array[1] * 81;
		  down_left_arrays[i] += down_left_array[1] * 81;
		  left_arrays[i] += left_array[1] * 81;
		  up_left_arrays[i] += up_left_array[1] * 81;
		  up_arrays[i] += up_array[1] * 81;
		  up_right_arrays[i] += up_right_array[1] * 81;
		  right_arrays2[i] += right_array2[1] * 81;
		  down_right_arrays2[i] += down_right_array2[1] * 81;
		  down_arrays2[i] += down_array2[1] * 81;
		  down_left_arrays2[i] += down_left_array2[1] * 81;
		  left_arrays2[i] += left_array2[1] * 81;
		  up_left_arrays2[i] += up_left_array2[1] * 81;
		  up_arrays2[i] += up_array2[1] * 81;
		  up_right_arrays2[i] += up_right_array2[1] * 81;
		}
		
		//スタート地点から2つ目の場所　3^3の位
		if(j == 2){
		  right_arrays[i] += right_array[2] * 27;
		  down_right_arrays[i] += down_right_array[2] * 27;
		  down_arrays[i] += down_array[2] * 27;
		  down_left_arrays[i] += down_left_array[2] * 27;
		  left_arrays[i] += left_array[2] * 27;
		  up_left_arrays[i] += up_left_array[2] * 27;
		  up_arrays[i] += up_array[2] * 27;
		  up_right_arrays[i] += up_right_array[2] * 27;
		  right_arrays2[i] += right_array2[2] * 27;
		  down_right_arrays2[i] += down_right_array2[2] * 27;
		  down_arrays2[i] += down_array2[2] * 27;
		  down_left_arrays2[i] += down_left_array2[2] * 27;
		  left_arrays2[i] += left_array2[2] * 27;
		  up_left_arrays2[i] += up_left_array2[2] * 27;
		  up_arrays2[i] += up_array2[2] * 27;
		  up_right_arrays2[i] += up_right_array2[2] * 27;
		}
		//スタート地点から3つ目の場所　3^2の位
		if(j == 3){
		  right_arrays[i] += right_array[3] * 9;
		  down_right_arrays[i] += down_right_array[3] * 9;
		  down_arrays[i] += down_array[3] * 9;
		  down_left_arrays[i] += down_left_array[3] * 9;
		  left_arrays[i] += left_array[3] * 9;
		  up_left_arrays[i] += up_left_array[3] * 9;
		  up_arrays[i] += up_array[3] * 9;
		  up_right_arrays[i] += up_right_array[3] * 9;
		  right_arrays2[i] += right_array2[3] * 9;
		  down_right_arrays2[i] += down_right_array2[3] * 9;
		  down_arrays2[i] += down_array2[3] * 9;
		  down_left_arrays2[i] += down_left_array2[3] * 9;
		  left_arrays2[i] += left_array2[3] * 9;
		  up_left_arrays2[i] += up_left_array2[3] * 9;
		  up_arrays2[i] += up_array2[3] * 9;
		  up_right_arrays2[i] += up_right_array2[3] * 9;
		}
		//スタート地点から4つ目の場所　3^1の位
		if(j == 4){
		  right_arrays[i] += right_array[4] * 3;
		  down_right_arrays[i] += down_right_array[4] * 3;
		  down_arrays[i] += down_array[4] * 3;
		  down_left_arrays[i] += down_left_array[4] * 3;
		  left_arrays[i] += left_array[4] * 3;
		  up_left_arrays[i] += up_left_array[4] * 3;
		  up_arrays[i] += up_array[4] * 3;
		  up_right_arrays[i] += up_right_array[4] * 3;
		  right_arrays2[i] += right_array2[4] * 3;
		  down_right_arrays2[i] += down_right_array2[4] * 3;
		  down_arrays2[i] += down_array2[4] * 3;
		  down_left_arrays2[i] += down_left_array2[4] * 3;
		  left_arrays2[i] += left_array2[4] * 3;
		  up_left_arrays2[i] += up_left_array2[4] * 3;
		  up_arrays2[i] += up_array2[4] * 3;
		  up_right_arrays2[i] += up_right_array2[4] * 3;
		}
		//スタート地点から5つ目の場所　3^0の位
		if(j == 5){
		  right_arrays[i] += right_array[5];
		  down_right_arrays[i] += down_right_array[5];
		  down_arrays[i] += down_array[5];
		  down_left_arrays[i] += down_left_array[5];
		  left_arrays[i] += left_array[5];
		  up_left_arrays[i] += up_left_array[5]; 
		  up_arrays[i] += up_array[5];
		  up_right_arrays[i] += up_right_array[5];	  
		  right_arrays2[i] += right_array2[5];
		  down_right_arrays2[i] += down_right_array2[5];
		  down_arrays2[i] += down_array2[5];
		  down_left_arrays2[i] += down_left_array2[5];
		  left_arrays2[i] += left_array2[5];
		  up_left_arrays2[i] += up_left_array2[5]; 
		  up_arrays2[i] += up_array2[5];
		  up_right_arrays2[i] += up_right_array2[5];	  
		}
	      }
	    

	      //各配列について、それぞれの並びに関して得点を計算
	      //3進数にしているのは、敵陣の石、自陣の石、空白の3種類全部の場合を考慮することを考えてのこと。
	      //条件のifにより、途中で形によっては高い点数をつけられるように
	      /////////////////右方向
	      if(point_board[i][0] >= -1){
		switch(right_arrays[i]){
		case 188://020222
		  point_board[i+2][0] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i+3][0] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i+4][0] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][0] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][0] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i+2][0] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i+3][0] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i+4][0] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][0] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i+2][0] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i+3][0] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i+1][0] += 22000;
			break;
		case 186://020220
			point_board[i+2][0] += 22000;
			break;
		case 222://022020
			point_board[i+3][0] += 22000;
			break;
		case 234://022200
			point_board[i+4][0] += 22000;
			break;

	/*	case 37://001101
		  point_board[i+1][0] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i+2][0] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i+3][0] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i+1][0] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i+4][0] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][0] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i+5][0] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i+1][0] += 100;//100
		  point_board[i+2][0] += 100;//100
		  break;
		case 60://002020
		  point_board[i+1][0] += 100;//100
		  point_board[i+3][0] += 100;//100
		  break;
		case 72://002200
		  point_board[i+1][0] += 100;//100
		  point_board[i+4][0] += 100;//100
		  break;
		case 73://002201
			point_board[i+1][0] += 50;
			break;
		case 168://020020
		  point_board[i+2][0] += 100;
		  point_board[i+3][0] += 100;//100
		  break;
		case 180://020200
		  point_board[i+2][0] += 100;//100
		  point_board[i+4][0] += 100;
		  break;
		case 187://020221
		  point_board[i][0] += 50;//100
		  point_board[i+2][0] += 50;
		  break;
		case 223://022021
		  point_board[i][0] += 50;//100
		  point_board[i+4][0] += 50;
		  break;
		case 216://022000
		  point_board[i+3][0] += 100;//100
		  point_board[i+4][0] += 100;
		  break;
		case 217://022001
		  point_board[i+3][0] += 50;//100
		  break;
		case 218://022002
		  point_board[i+3][0] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i+2][0] += 50;//100
		  point_board[i+3][0] += 10;
		  point_board[i+4][0] += 10;
		  break;
		case 182://020202
		  point_board[i+2][0] += 100;//10
		  point_board[i+4][0] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][0] += 5;//10
		  break;
		case 84://010010
		  point_board[i+2][0] += 10;//10
		  break;
		case 91://010101
		  point_board[i+2][0] += 10;//10
		  break;
		case 95://010112
		  point_board[i][0] += 5;//10
		  point_board[i+2][0] += 5;//10
		  break;
		case 113://011012
		  point_board[i][0] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i+3][0] += 1;//1
		  break;
		case 18://000200
		  point_board[i+1][0] += 30;//1
		  point_board[i+2][0] += 30;//1
		  break;
		case 19://000201
		  point_board[i+1][0] += 20;//1
		  break;
		case 25://000221
		  point_board[i+1][0] += 20;//1
		  break;
		case 54://002000
		  point_board[i+3][0] += 30;//1
		  point_board[i+4][0] += 30;//1
		  break;
		case 55://002001
		  point_board[i+1][0] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i+1][0] += 30;/////1
		  break;
		case 61://002021
		  point_board[i+1][0] += 30;//1
		  break;
		case 162://020000
		  point_board[i+2][0] += 30;//1
		  point_board[i+3][0] += 30;//1
		  break;
		case 163://020001
		  point_board[i+2][0] += 30;//1
		  break;
		case 181://020201
		  point_board[i+2][0] += 80;//1
		  break;
		
		}
	      }
	      if(point_board2[i][0] >= -1){
		switch(right_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][0] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][0] += 50000;//5万
		  break;
		}
	      }
	      	

	    	
	    	///////////////右下方向
	      if(point_board[i][1] >= -1){
		switch(down_right_arrays[i]){
		case 188://020222
		  point_board[i+22][1] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i+33][1] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i+44][1] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][1] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][1] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i+22][1] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i+33][1] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i+44][1] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][1] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i+22][1] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i+33][1] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i+11][1] += 22000;
			break;
		case 186://020220
			point_board[i+22][1] += 22000;
			break;
		case 222://022020
			point_board[i+33][1] += 22000;
			break;
		case 234://022200
			point_board[i+44][1] += 22000;
			break;

	/*	case 37://001101
		  point_board[i+11][1] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i+22][1] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i+33][1] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i+11][1] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i+44][1] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][1] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i+55][1] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i+11][1] += 100;//100
		  point_board[i+22][1] += 100;//100
		  break;
		case 60://002020
		  point_board[i+11][1] += 100;//100
		  point_board[i+33][1] += 100;//100
		  break;
		case 72://002200
		  point_board[i+11][1] += 100;//100
		  point_board[i+44][1] += 100;//100
		  break;
		case 73://002201
			point_board[i+11][1] += 50;
			break;
		case 168://020020
		  point_board[i+22][1] += 100;
		  point_board[i+33][1] += 100;//100
		  break;
		case 180://020200
		  point_board[i+22][1] += 100;//100
		  point_board[i+44][1] += 100;
		  break;
		case 187://020221
		  point_board[i][1] += 50;//100
		  point_board[i+22][1] += 50;
		  break;
		case 223://022021
		  point_board[i][1] += 50;//100
		  point_board[i+44][1] += 50;
		  break;
		case 216://022000
		  point_board[i+33][1] += 100;//100
		  point_board[i+44][1] += 100;
		  break;
		case 217://022001
		  point_board[i+33][1] += 50;//100
		  break;
		case 218://022002
		  point_board[i+33][1] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i+22][1] += 50;//100
		  point_board[i+33][1] += 10;
		  point_board[i+44][1] += 10;
		  break;
		case 182://020202
		  point_board[i+22][1] += 100;//10
		  point_board[i+44][1] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][1] += 5;//10
		  break;
		case 84://010010
		  point_board[i+22][1] += 10;//10
		  break;
		case 91://010101
		  point_board[i+22][1] += 10;//10
		  break;
		case 95://010112
		  point_board[i][1] += 5;//10
		  point_board[i+22][1] += 5;//10
		  break;
		case 113://011012
		  point_board[i][1] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i+33][1] += 1;//1
		  break;
		case 18://000200
		  point_board[i+11][1] += 30;//1
		  point_board[i+22][1] += 30;//1
		  break;
		case 19://000201
		  point_board[i+11][1] += 20;//1
		  break;
		case 25://000221
		  point_board[i+11][1] += 20;//1
		  break;
		case 54://002000
		  point_board[i+33][1] += 30;//1
		  point_board[i+44][1] += 30;//1
		  break;
		case 55://002001
		  point_board[i+11][1] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i+11][1] += 30;/////1
		  break;
		case 61://002021
		  point_board[i+11][1] += 30;//1
		  break;
		case 162://020000
		  point_board[i+22][1] += 30;//1
		  point_board[i+33][1] += 30;//1
		  break;
		case 163://020001
		  point_board[i+22][1] += 30;//1
		  break;
		case 181://020201
		  point_board[i+22][1] += 80;//1
		  break;
		
		}
	      }
	      if(point_board2[i][1] >= -1){
		switch(down_right_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][1] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][1] += 50000;//5万
		  break;
		}
	      }

	      ////////////////下方向
	      if(point_board[i][2] >= -1){
		switch(down_arrays[i]){
		case 188://020222
		  point_board[i+20][2] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i+30][2] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i+40][2] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][2] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][2] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i+20][2] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i+30][2] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i+40][2] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][2] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i+20][2] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i+30][2] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i+10][2] += 22000;
			break;
		case 186://020220
			point_board[i+20][2] += 22000;
			break;
		case 222://022020
			point_board[i+30][2] += 22000;
			break;
		case 234://022200
			point_board[i+40][2] += 22000;
			break;

	/*	case 37://001101
		  point_board[i+10][2] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i+20][2] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i+30][2] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i+10][2] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i+40][2] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][2] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i+50][2] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i+10][2] += 100;//100
		  point_board[i+20][2] += 100;//100
		  break;
		case 60://002020
		  point_board[i+10][2] += 100;//100
		  point_board[i+30][2] += 100;//100
		  break;
		case 72://002200
		  point_board[i+10][2] += 100;//100
		  point_board[i+40][2] += 100;//100
		  break;
		case 73://002201
			point_board[i+10][2] += 50;
			break;
		case 168://020020
		  point_board[i+20][2] += 100;
		  point_board[i+30][2] += 100;//100
		  break;
		case 180://020200
		  point_board[i+20][2] += 100;//100
		  point_board[i+40][2] += 100;
		  break;
		case 187://020221
		  point_board[i][2] += 50;//100
		  point_board[i+20][2] += 50;
		  break;
		case 223://022021
		  point_board[i][2] += 50;//100
		  point_board[i+40][2] += 50;
		  break;
		case 216://022000
		  point_board[i+30][2] += 100;//100
		  point_board[i+40][2] += 100;
		  break;
		case 217://022001
		  point_board[i+30][2] += 50;//100
		  break;
		case 218://022002
		  point_board[i+30][2] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i+20][2] += 50;//100
		  point_board[i+30][2] += 10;
		  point_board[i+40][2] += 10;
		  break;
		case 182://020202
		  point_board[i+20][2] += 100;//10
		  point_board[i+40][2] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][2] += 5;//10
		  break;
		case 84://010010
		  point_board[i+20][2] += 10;//10
		  break;
		case 91://010101
		  point_board[i+20][2] += 10;//10
		  break;
		case 95://010112
		  point_board[i][2] += 5;//10
		  point_board[i+20][2] += 5;//10
		  break;
		case 113://011012
		  point_board[i][2] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i+30][2] += 1;//1
		  break;
		case 18://000200
		  point_board[i+10][2] += 30;//1
		  point_board[i+20][2] += 30;//1
		  break;
		case 19://000201
		  point_board[i+10][2] += 20;//1
		  break;
		case 25://000221
		  point_board[i+10][2] += 20;//1
		  break;
		case 54://002000
		  point_board[i+30][2] += 30;//1
		  point_board[i+40][2] += 30;//1
		  break;
		case 55://002001
		  point_board[i+10][2] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i+10][2] += 30;/////1
		  break;
		case 61://002021
		  point_board[i+10][2] += 30;//1
		  break;
		case 162://020000
		  point_board[i+20][2] += 30;//1
		  point_board[i+30][2] += 30;//1
		  break;
		case 163://020001
		  point_board[i+20][2] += 30;//1
		  break;
		case 181://020201
		  point_board[i+20][2] += 80;//1
		  break;
		
		}
	      }
	      if(point_board2[i][2] >= -1){
		switch(down_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][2] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][2] += 50000;//5万
		  break;
		}
	      }


	      //////////////////////左下方向
	      if(point_board[i][3] >= -1){
		switch(down_left_arrays[i]){
		case 188://020222
		  point_board[i+18][3] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i+27][3] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i+36][3] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][3] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][3] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i+18][3] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i+27][3] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i+36][3] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][3] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i+18][3] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i+27][3] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i+9][3] += 22000;
			break;
		case 186://020220
			point_board[i+18][3] += 22000;
			break;
		case 222://022020
			point_board[i+27][3] += 22000;
			break;
		case 234://022200
			point_board[i+36][3] += 22000;
			break;

	/*	case 37://001101
		  point_board[i+9][3] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i+18][3] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i+27][3] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i+9][3] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i+36][3] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][3] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i+45][3] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i+9][3] += 100;//100
		  point_board[i+18][3] += 100;//100
		  break;
		case 60://002020
		  point_board[i+9][3] += 100;//100
		  point_board[i+27][3] += 100;//100
		  break;
		case 72://002200
		  point_board[i+9][3] += 100;//100
		  point_board[i+36][3] += 100;//100
		  break;
		case 73://002201
			point_board[i+9][3] += 50;
			break;
		case 168://020020
		  point_board[i+18][3] += 100;
		  point_board[i+27][3] += 100;//100
		  break;
		case 180://020200
		  point_board[i+18][3] += 100;//100
		  point_board[i+36][3] += 100;
		  break;
		case 187://020221
		  point_board[i][3] += 50;//100
		  point_board[i+18][3] += 50;
		  break;
		case 223://022021
		  point_board[i][3] += 50;//100
		  point_board[i+36][3] += 50;
		  break;
		case 216://022000
		  point_board[i+27][3] += 100;//100
		  point_board[i+36][3] += 100;
		  break;
		case 217://022001
		  point_board[i+27][3] += 50;//100
		  break;
		case 218://022002
		  point_board[i+27][3] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i+18][3] += 50;//100
		  point_board[i+27][3] += 10;
		  point_board[i+36][3] += 10;
		  break;
		case 182://020202
		  point_board[i+18][3] += 100;//10
		  point_board[i+36][3] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][3] += 5;//10
		  break;
		case 84://010010
		  point_board[i+18][3] += 10;//10
		  break;
		case 91://010101
		  point_board[i+18][3] += 10;//10
		  break;
		case 95://010112
		  point_board[i][3] += 5;//10
		  point_board[i+18][3] += 5;//10
		  break;
		case 113://011012
		  point_board[i][3] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i+27][3] += 1;//1
		  break;
		case 18://000200
		  point_board[i+9][3] += 30;//1
		  point_board[i+18][3] += 30;//1
		  break;
		case 19://000201
		  point_board[i+9][3] += 20;//1
		  break;
		case 25://000221
		  point_board[i+9][3] += 20;//1
		  break;
		case 54://002000
		  point_board[i+27][3] += 30;//1
		  point_board[i+36][3] += 30;//1
		  break;
		case 55://002001
		  point_board[i+9][3] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i+9][3] += 30;/////1
		  break;
		case 61://002021
		  point_board[i+9][3] += 30;//1
		  break;
		case 162://020000
		  point_board[i+18][3] += 30;//1
		  point_board[i+27][3] += 30;//1
		  break;
		case 163://020001
		  point_board[i+18][3] += 30;//1
		  break;
		case 181://020201
		  point_board[i+18][3] += 80;//1
		  break;
		
		}
	      }
	      if(point_board2[i][3] >= -1){
		switch(down_left_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][3] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][3] += 50000;//5万
		  break;
		}
	      }

	      /////////////////////左方向
	      if(point_board[i][4] >= -1){
		switch(left_arrays[i]){
		case 188://020222
		  point_board[i+2][4] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i-3][4] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i-4][4] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][4] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][4] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i-2][4] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i-3][4] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i-4][4] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][4] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i-2][4] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i-3][4] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i-1][4] += 22000;
			break;
		case 186://020220
			point_board[i-2][4] += 22000;
			break;
		case 222://022020
			point_board[i-3][4] += 22000;
			break;
		case 234://022200
			point_board[i-4][4] += 22000;
			break;

	/*	case 37://001101
		  point_board[i-1][4] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i-2][4] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i-3][4] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i-1][4] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i-4][4] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][4] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i-5][4] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i-1][4] += 100;//100
		  point_board[i-2][4] += 100;//100
		  break;
		case 60://002020
		  point_board[i-1][4] += 100;//100
		  point_board[i-3][4] += 100;//100
		  break;
		case 72://002200
		  point_board[i-1][4] += 100;//100
		  point_board[i-4][4] += 100;//100
		  break;
		case 73://002201
			point_board[i-1][4] += 50;
			break;
		case 168://020020
		  point_board[i-2][4] += 100;
		  point_board[i-3][4] += 100;//100
		  break;
		case 180://020200
		  point_board[i-2][4] += 100;//100
		  point_board[i-4][4] += 100;
		  break;
		case 187://020221
		  point_board[i][4] += 50;//100
		  point_board[i-2][4] += 50;
		  break;
		case 223://022021
		  point_board[i][4] += 50;//100
		  point_board[i-4][4] += 50;
		  break;
		case 216://022000
		  point_board[i-3][4] += 100;//100
		  point_board[i-4][4] += 100;
		  break;
		case 217://022001
		  point_board[i-3][4] += 50;//100
		  break;
		case 218://022002
		  point_board[i-3][4] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i-2][4] += 50;//100
		  point_board[i-3][4] += 10;
		  point_board[i-4][4] += 10;
		  break;
		case 182://020202
		  point_board[i-2][4] += 100;//10
		  point_board[i-4][4] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][4] += 5;//10
		  break;
		case 84://010010
		  point_board[i-2][4] += 10;//10
		  break;
		case 91://010101
		  point_board[i-2][4] += 10;//10
		  break;
		case 95://010112
		  point_board[i][4] += 5;//10
		  point_board[i-2][4] += 5;//10
		  break;
		case 113://011012
		  point_board[i][4] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i-3][4] += 1;//1
		  break;
		case 18://000200
		  point_board[i-1][4] += 30;//1
		  point_board[i-2][4] += 30;//1
		  break;
		case 19://000201
		  point_board[i-1][4] += 20;//1
		  break;
		case 25://000221
		  point_board[i-1][4] += 20;//1
		  break;
		case 54://002000
		  point_board[i-3][4] += 30;//1
		  point_board[i-4][4] += 30;//1
		  break;
		case 55://002001
		  point_board[i-1][4] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i-1][4] += 30;/////1
		  break;
		case 61://002021
		  point_board[i-1][4] += 30;//1
		  break;
		case 162://020000
		  point_board[i-2][4] += 30;//1
		  point_board[i-3][4] += 30;//1
		  break;
		case 163://020001
		  point_board[i-2][4] += 30;//1
		  break;
		case 181://020201
		  point_board[i-2][4] += 80;//1
		  break;
		
		}
	      }
	      if(point_board2[i][4] >= -1){
		switch(left_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][4] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][4] += 50000;//5万
		  break;
		}
	      }

	      //////////////////////左上方向
	      if(point_board[i][5] >= -1){
		switch(up_left_arrays[i]){
		case 188://020222
		  point_board[i-22][5] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i-33][5] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i-44][5] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][5] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][5] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i-22][5] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i-33][5] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i-44][5] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][5] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i-22][5] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i-33][5] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i-11][5] += 22000;
			break;
		case 186://020220
			point_board[i-22][5] += 22000;
			break;
		case 222://022020
			point_board[i-33][5] += 22000;
			break;
		case 234://022200
			point_board[i-44][5] += 22000;
			break;

	/*	case 37://001101
		  point_board[i-11][5] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i-22][5] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i-33][5] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i-11][5] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i-44][5] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][5] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i-55][5] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i-11][5] += 100;//100
		  point_board[i-22][5] += 100;//100
		  break;
		case 60://002020
		  point_board[i-11][5] += 100;//100
		  point_board[i-33][5] += 100;//100
		  break;
		case 72://002200
		  point_board[i-11][5] += 100;//100
		  point_board[i-44][5] += 100;//100
		  break;
		case 73://002201
			point_board[i-11][5] += 50;
			break;
		case 168://020020
		  point_board[i-22][5] += 100;
		  point_board[i-33][5] += 100;//100
		  break;
		case 180://020200
		  point_board[i-22][5] += 100;//100
		  point_board[i-44][5] += 100;
		  break;
		case 187://020221
		  point_board[i][5] += 50;//100
		  point_board[i-22][5] += 50;
		  break;
		case 223://022021
		  point_board[i][5] += 50;//100
		  point_board[i-44][5] += 50;
		  break;
		case 216://022000
		  point_board[i-33][5] += 100;//100
		  point_board[i-44][5] += 100;
		  break;
		case 217://022001
		  point_board[i-33][5] += 50;//100
		  break;
		case 218://022002
		  point_board[i-33][5] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i-22][5] += 50;//100
		  point_board[i-33][5] += 10;
		  point_board[i-44][5] += 10;
		  break;
		case 182://020202
		  point_board[i-22][5] += 100;//10
		  point_board[i-44][5] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][5] += 5;//10
		  break;
		case 84://010010
		  point_board[i-22][5] += 10;//10
		  break;
		case 91://010101
		  point_board[i-22][5] += 10;//10
		  break;
		case 95://010112
		  point_board[i][5] += 5;//10
		  point_board[i-22][5] += 5;//10
		  break;
		case 113://011012
		  point_board[i][5] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i-33][5] += 1;//1
		  break;
		case 18://000200
		  point_board[i-11][5] += 30;//1
		  point_board[i-22][5] += 30;//1
		  break;
		case 19://000201
		  point_board[i-11][5] += 20;//1
		  break;
		case 25://000221
		  point_board[i-11][5] += 20;//1
		  break;
		case 54://002000
		  point_board[i-33][5] += 30;//1
		  point_board[i-44][5] += 30;//1
		  break;
		case 55://002001
		  point_board[i-11][5] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i-11][5] += 30;/////1
		  break;
		case 61://002021
		  point_board[i-11][5] += 30;//1
		  break;
		case 162://020000
		  point_board[i-22][5] += 30;//1
		  point_board[i-33][5] += 30;//1
		  break;
		case 163://020001
		  point_board[i-22][5] += 30;//1
		  break;
		case 181://020201
		  point_board[i-22][5] += 80;//1
		  break;
		
		}
	      }
	      if(point_board2[i][5] >= -1){
		switch(up_left_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][5] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][5] += 50000;//5万
		  break;
		}
	      }

	      ///////////////////上方向
	      if(point_board[i][6] >= -1){
		switch(up_arrays[i]){
		case 188://020222
		  point_board[i-20][6] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i-30][6] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i-40][6] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][6] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][6] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i-20][6] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i-30][6] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i-40][6] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][6] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i-20][6] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i-30][6] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i-10][6] += 22000;
			break;
		case 186://020220
			point_board[i-20][6] += 22000;
			break;
		case 222://022020
			point_board[i-30][6] += 22000;
			break;
		case 234://022200
			point_board[i-40][6] += 22000;
			break;

	/*	case 37://001101
		  point_board[i-10][6] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i-20][6] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i-30][6] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i-10][6] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i-40][6] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][6] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i-50][6] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i-10][6] += 100;//100
		  point_board[i-20][6] += 100;//100
		  break;
		case 60://002020
		  point_board[i-10][6] += 100;//100
		  point_board[i-30][6] += 100;//100
		  break;
		case 72://002200
		  point_board[i-10][6] += 100;//100
		  point_board[i-40][6] += 100;//100
		  break;
		case 73://002201
			point_board[i-10][6] += 50;
			break;
		case 168://020020
		  point_board[i-20][6] += 100;
		  point_board[i-30][6] += 100;//100
		  break;
		case 180://020200
		  point_board[i-20][6] += 100;//100
		  point_board[i-40][6] += 100;
		  break;
		case 187://020221
		  point_board[i][6] += 50;//100
		  point_board[i-20][6] += 50;
		  break;
		case 223://022021
		  point_board[i][6] += 50;//100
		  point_board[i-40][6] += 50;
		  break;
		case 216://022000
		  point_board[i-30][6] += 100;//100
		  point_board[i-40][6] += 100;
		  break;
		case 217://022001
		  point_board[i-30][6] += 50;//100
		  break;
		case 218://022002
		  point_board[i-30][6] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i-20][6] += 50;//100
		  point_board[i-30][6] += 10;
		  point_board[i-40][6] += 10;
		  break;
		case 182://020202
		  point_board[i-20][6] += 100;//10
		  point_board[i-40][6] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][6] += 5;//10
		  break;
		case 84://010010
		  point_board[i-20][6] += 10;//10
		  break;
		case 91://010101
		  point_board[i-20][6] += 10;//10
		  break;
		case 95://010112
		  point_board[i][6] += 5;//10
		  point_board[i-20][6] += 5;//10
		  break;
		case 113://011012
		  point_board[i][6] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i-30][6] += 1;//1
		  break;
		case 18://000200
		  point_board[i-10][6] += 30;//1
		  point_board[i-20][6] += 30;//1
		  break;
		case 19://000201
		  point_board[i-10][6] += 20;//1
		  break;
		case 25://000221
		  point_board[i-10][6] += 20;//1
		  break;
		case 54://002000
		  point_board[i-30][6] += 30;//1
		  point_board[i-40][6] += 30;//1
		  break;
		case 55://002001
		  point_board[i-10][6] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i-10][6] += 30;/////1
		  break;
		case 61://002021
		  point_board[i-10][6] += 30;//1
		  break;
		case 162://020000
		  point_board[i-20][6] += 30;//1
		  point_board[i-30][6] += 30;//1
		  break;
		case 163://020001
		  point_board[i-20][6] += 30;//1
		  break;
		case 181://020201
		  point_board[i-20][6] += 80;//1
		  break;
		
		}
	      }
	      if(point_board2[i][6] >= -1){
		switch(up_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][6] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][6] += 50000;//5万
		  break;
		}
	      }

	      ///////////////////右上方向
	      if(point_board[i][7] >= -1){
		switch(up_right_arrays[i]){
		case 188://020222
		  point_board[i-18][7] += 100000;//10万
		  break;
		case 220://022022
		  point_board[i-27][7] += 100000;//10万
		  break;
		case 227://022202
		  point_board[i-36][7] += 100000;//10万
		  break;
		case 240://022220
		  point_board[i][7] += 100000;//10万
		  break;
		case 241://022221
		  point_board[i][7] += 100000;//10万
		  break;
		case 94://010111
		  point_board[i-18][7] += 50000;//5万
		  break;
		case 112://011011
		  point_board[i-27][7] += 50000;//5万
		  break;
		case 118://011101
		  point_board[i-36][7] += 50000;//5万
		  break;
		case 122://011112
		  point_board[i][7] += 50000;//5万
		  break;
	/*	case 13://000111
		  point_board[i-18][7] += 1000;//1000
		  break;

		case 31://001011
		  point_board[i-27][7] += 1000;//1000
		  break;
	*/
		case 78://002220
			point_board[i-9][7] += 22000;
			break;
		case 186://020220
			point_board[i-18][7] += 22000;
			break;
		case 222://022020
			point_board[i-27][7] += 22000;
			break;
		case 234://022200
			point_board[i-36][7] += 22000;
			break;

	/*	case 37://001101
		  point_board[i-9][7] += 2000;//2000
		  break;
	*/
		case 93://010110
		  point_board[i-18][7] += 20000;//2万
		  break;
		case 111://011010
		  point_board[i-27][7] += 20000;//2万
		  break;
		case 39://001110
		  point_board[i-9][7] += 20000;//2万
		  break;
		case 117://011100
		  point_board[i-36][7] += 20000;//2万
		  break;
	/*	case 119://011102
		  point_board[i][7] += 1000;//1000
		  break;
		case 201://021110
		  point_board[i-45][7] += 1000;//1000
		  break;
	*/
			
			
			
		case 24://000220
		  point_board[i-9][7] += 100;//100
		  point_board[i-18][7] += 100;//100
		  break;
		case 60://002020
		  point_board[i-9][7] += 100;//100
		  point_board[i-27][7] += 100;//100
		  break;
		case 72://002200
		  point_board[i-9][7] += 100;//100
		  point_board[i-36][7] += 100;//100
		  break;
		case 73://002201
			point_board[i-9][7] += 50;
			break;
		case 168://020020
		  point_board[i-18][7] += 100;
		  point_board[i-27][7] += 100;//100
		  break;
		case 180://020200
		  point_board[i-18][7] += 100;//100
		  point_board[i-36][7] += 100;
		  break;
		case 187://020221
		  point_board[i][7] += 50;//100
		  point_board[i-18][7] += 50;
		  break;
		case 223://022021
		  point_board[i][7] += 50;//100
		  point_board[i-36][7] += 50;
		  break;
		case 216://022000
		  point_board[i-27][7] += 100;//100
		  point_board[i-36][7] += 100;
		  break;
		case 217://022001
		  point_board[i-27][7] += 50;//100
		  break;
		case 218://022002
		  point_board[i-27][7] += 100;//100
		  break;
			
			
		case 164://020002
		  point_board[i-18][7] += 50;//100
		  point_board[i-27][7] += 10;
		  point_board[i-36][7] += 10;
		  break;
		case 182://020202
		  point_board[i-18][7] += 100;//10
		  point_board[i-36][7] += 100;//10
		  break;
			
		case 38://001102
		  point_board[i][7] += 5;//10
		  break;
		case 84://010010
		  point_board[i-18][7] += 10;//10
		  break;
		case 91://010101
		  point_board[i-18][7] += 10;//10
		  break;
		case 95://010112
		  point_board[i][7] += 5;//10
		  point_board[i-18][7] += 5;//10
		  break;
		case 113://011012
		  point_board[i][7] += 10;//10
		  break;
			
			
		case 6://000020
		  point_board[i-27][7] += 1;//1
		  break;
		case 18://000200
		  point_board[i-9][7] += 30;//1
		  point_board[i-18][7] += 30;//1
		  break;
		case 19://000201
		  point_board[i-9][7] += 20;//1
		  break;
		case 25://000221
		  point_board[i-9][7] += 20;//1
		  break;
		case 54://002000
		  point_board[i-27][7] += 30;//1
		  point_board[i-36][7] += 30;//1
		  break;
		case 55://002001
		  point_board[i-9][7] += 30;//1
		  break;
		case 79:////////////////002221
		  point_board[i-9][7] += 30;/////1
		  break;
		case 61://002021
		  point_board[i-9][7] += 30;//1
		  break;
		case 162://020000
		  point_board[i-18][7] += 30;//1
		  point_board[i-27][7] += 30;//1
		  break;
		case 163://020001
		  point_board[i-18][7] += 30;//1
		  break;
		case 181://020201
		  point_board[i-18][7] += 80;//1
		  break;
		
		}
	      }
		  if(point_board2[i][7] >= -1){
		switch(up_right_arrays2[i]){
		case 672://220220
		case 673://220221
		case 674://220222
		case 188://020222
		case 431://120222
		  point_board[i][7] += 100000;//10万
		  break;
		case 336://110110
		case 337://110111
		case 338://110112
		case 94 ://010111
		case 580://210111
		  point_board[i][7] += 50000;//5万
		  break;
		}
		  }

	        
	  }
/*	  printf("\n");
	  for(i=0; i<100; i++){
		if(i%10==0){printf("%2d\n",i);}
		for(j=0;j<8;j++){
	    	printf("%6d", point_board[i][j]);
	    	if(j==7){printf("\n");}
	    }
	    if(i%10 == 9) printf("\n", point_board[i][j]);
	   
	  }
*/
  	
	  //各マスの総合得点を計算
	  for(i=0; i<100; i++){
	    for(j=0; j<8; j++){
	      point[i] += point_board[i][j];
	    }
	  }
	  int cnt = 2;
	  max_point = -1000;
	  //もっとも点数の高いポイントを計算
	  for(i=0; i<100; i++){
	    if(point[i] > max_point){
	      cnt ++;
	      best_pos = i;
	      max_point = point[i];
	    }else if(point[i] == max_point){
	      if(rand()%cnt == 0){
		cnt++;
		best_pos = i;
		max_point = point[i];
	      }
	    }
	  }

	  
	    
	  printf("\n");
	  for(i=0; i<100; i++){
	    printf("%6d", point[i]);
	    if(i%10 == 9) printf("\n", point[i]);
	   
	  }
	  
	  printf("\n");
	  return best_pos;
  }
}


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

	//先手の場合の１手目
	if (count == 0) {
		*pos_x = 5;
		*pos_y = 5;
		return;
	}
	//後手の場合の２手目
	else if (count == 1) {
		if (history[count-1].y > 1 && history[count-1].y < BOARD_SIZE - 2 && history[count-1].x > 1 && history[count-1].x < BOARD_SIZE - 2) {
			//相手の左右どちらに置くかランダムに決める
			tmp_d = (double)rand() / ((double)RAND_MAX + 1);
			tateyoko_check = (int)( tmp_d * 2.0 );
			if (tateyoko_check == 0) {
				*pos_x = history[count-1].x - 1;
			}
			else {
				*pos_x = history[count-1].x + 1;
			}
			//相手の上下どちらに置くかランダムに決める
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
	
	//３手目以降
	else {
		//自分の５連を作る手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
			color = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len4check(board, j, i, pos_x, pos_y, &pos_flag); //4連から作る
					str9_tobi3_1check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(3:1)から作る
					str9_tobi2_2check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(2:2)から作る
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		
		//相手の５連を阻止する手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len4check(board, j, i, pos_x, pos_y, &pos_flag); //4連からの５連を防ぐ
					str9_tobi3_1check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(3:1)の5連を防ぐ
					str9_tobi2_2check(board, j, i, pos_x, pos_y, &pos_flag); //飛び4(2:2)の5連を防ぐ
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//自分の３連を確認して、４連（両端フリー）を作る手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_WHITE;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE) {
			color = STONE_BLACK;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len3check(board, j, i, pos_x, pos_y, &pos_flag); //3連から
					str9_tobi3check(board, j, i, pos_x, pos_y, &pos_flag); //飛び三から
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//相手の３連を確認して、４連（両端フリー）を阻止する手を探索
		if (board[history[count-1].y][history[count-1].x] == STONE_BLACK) {
			color = STONE_BLACK;
		}
		else if (board[history[count-1].y][history[count-1].x] == STONE_WHITE){
			color = STONE_WHITE;
		}
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == color) {
					str9_len3check(board, j, i, pos_x, pos_y, &pos_flag); //3連からの4連を防ぐ
					str9_tobi3check(board, j, i, pos_x, pos_y, &pos_flag); //飛び３からの4連を防ぐ
				}
				if (pos_flag == 1) {
					return;
				}
			}
		}
		
		//三三、三四を作る
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
		
		//三三、三四を阻止する
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
		
		//三三（制限なし）を作る
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
		
		//三三（制限なし）を阻止する
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
		
		//フリーの２連を作る(より多くのぼっちを救う)
		int max = 0, max_i, max_j;
		int botti_flag = 0; //ぼっちがいるかいないか
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
					//左上と右下
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
					//右上と左下
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
	
	//フリーの２連を阻止する
	int max = 0, max_i, max_j;
	int botti_flag = 0; //ぼっちがいるかいないか
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
				//左上と右下
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
				//右上と左下
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
	
	//ランダム
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
// 4連確認
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
			//縦４連のとき
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
			//横４連のとき
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
			//右斜め下４連のとき
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
			//左ななめ下４連のとき
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
// 飛び4確認(3:1)
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
			//縦飛び4のとき
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
			//横飛び4のとき
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
			//右斜め下飛び4連のとき
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
			//左ななめ下飛び4連のとき
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
// 飛び4確認(2:2)
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
			//縦飛び4のとき
			if (i == 0) {
				*pos_x = x;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//横飛び4のとき
			else if (i == 1) {
				*pos_x = x + 2;
				*pos_y = y;
				*pos_flag = 1;
				return;
			}
			//右斜め下飛び4連のとき
			else if (i == 2) {
				*pos_x = x + 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//左ななめ下飛び4連のとき
			else {
				*pos_x = x - 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
		}
    }
}

//とびさんを四連にする
void str9_tobi3check(const int board[][BOARD_SIZE], int x, int y, int *pos_x, int *pos_y, int *pos_flag)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1, -1};
    int dy[] = { 1, 0, 1, 1 };
    
	//1:2のとき
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
	        //縦のとき
			if (i == 0 && (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE)) {
				*pos_y = y + 1;
				*pos_x = x;
				*pos_flag = 1;
				return;
			}
			//横のとき
			else if (i == 1 && (board[y][x-1] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE)) {
				*pos_y = y;
				*pos_x = x + 1;
				*pos_flag = 1;
				return;
			}
			//右斜め下のとき
			else if (i == 2 && (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x + 1;
				*pos_y = y + 1;
				*pos_flag = 1;
				return;
			}
			//左ななめ下のとき
			else if (i == 3 && (board[y-1][x+1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x - 1;
				*pos_y = y + 1;
				*pos_flag = 1;
				return;
			}
		}
    }
    
    //2:1のとき
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
	        //縦のとき
			if (i == 0 && (board[y-1][x] == STONE_SPACE && board[y+j*dy[i]][x] == STONE_SPACE)) {
				*pos_y = y + 2;
				*pos_x = x;
				*pos_flag = 1;
				return;
			}
			//横のとき
			else if (i == 1 && (board[y][x-1] == STONE_SPACE && board[y][x+j*dx[i]] == STONE_SPACE)) {
				*pos_y = y;
				*pos_x = x + 2;
				*pos_flag = 1;
				return;
			}
			//右斜め下のとき
			else if (i == 2 && (board[y-1][x-1] == STONE_SPACE && board[y+j*dy[i]][x+j*dx[i]] == STONE_SPACE)) {
				*pos_x = x + 2;
				*pos_y = y + 2;
				*pos_flag = 1;
				return;
			}
			//左ななめ下のとき
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
// 三三,四三確認
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
// 三三確認（制限なし）
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
// 3連確認
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
			//縦3連のとき
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
			//横3連のとき
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
			//右斜め下3連のとき
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
			//左ななめ下3連のとき
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


//=======================================================
// Group 10
//=======================================================

void strategy10(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int score[BOARD_SIZE][BOARD_SIZE]; /* 評価値記憶用の配列 */
	int spacex[BOARD_SIZE], spacey[BOARD_SIZE];	
	
	int mystone, yourstone;
	
	int isi = -10000;
	
	int i, j, k;
	
	int me, you, nix;  /* nix means nothing */	
	
	int win = 15000, tobi = 1000, sanren = 500, three = 100, two = 10;
	int win2 = 12000, tobi2 = 800, sanren2 = 300, three2 = 80, two2 =10;
	
	int max;
	int setx[128], sety[128];
	

	printf( "Strategy 10\t" );

	for (i = 0; i < BOARD_SIZE; i++) { /* 盤面の評価値のリセット */
		for (j = 0; j < BOARD_SIZE; j++) {
			score[j][i] = 0;
		}
	}
	
	for (i = 0; i < BOARD_SIZE; i++) { /* すでに石の置いてあるところの判定 */
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[j][i] == STONE_BLACK || board[j][i] == STONE_WHITE) {
				score[j][i] += isi;
			}
		}
	}
	
//-------------------------------------------------------
// 先手・後手の判断
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
// 評価開始
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
	
	if (count == 0) { /* 先手の初手 */
		score[4][4] += 10;
		score[4][5] += 10;
		score[5][4] += 10;
		score[5][5] += 10;
	}
	
	if (count == 2 || count == 4) { /* 先手の2手目、3手目 */
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
	
	if (count == 1) { /* 後手の1手目 */
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
		
		for (i = 0; i < BOARD_SIZE; i++) { /* 縦方向に走査 */
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) { /* 5個ずつ走査 */
					
					if (board[i][k] == mystone) { /* 自分の石 */
						me++;
					}
					if (board[i][k] == yourstone) { /* 相手の石 */
						you++;
					}
					if (board[i][k] == STONE_SPACE) { /* 空白をカウントして位置を記憶 */
						spacex[nix] = i;
						spacey[nix] = k;
						nix++;
					}
				}
			
				if (me == 4 && nix == 1) { /* 四連（次の手で勝利） */
					score[spacex[0]][spacey[0]] += win;
				}
				
				if (me == 3 && nix == 2) { /* 三連（四連を生成） */
					if (board[i][j] == STONE_SPACE && board[i][j + 4] == STONE_SPACE) {
						score[i][j] += sanren;
						score[i][j + 4] += sanren;
					}
					else { /* 三連ではない3つ */
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
			
			for (j = 0; j < BOARD_SIZE - 5; j++) { /* 飛び三連の評価 */
					
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
					if (board[i][k] == STONE_SPACE && board[i][k + 5] == STONE_SPACE) { /* 飛び三連なら */
						score[spacex[1]][spacey[1]] += tobi; /* 四連を作る位置に加点 */
					}
					else {
						score[spacex[0]][spacey[0]] += 3;
						score[spacex[1]][spacey[1]] += 3;
						score[spacex[2]][spacey[2]] += 3;
					}
				}
			}
		}
		
		for (i = 0; i < BOARD_SIZE; i++) { /* 横方向に走査 */
			for (j = 0; j < BOARD_SIZE - 4; j++) {
				
				me = 0;
				you = 0;
				nix = 0;
				
				for (k = j; k < j + 5; k++) { /* 5個ずつ走査 */
					
					if (board[k][i] == mystone) { /* 自分の石 */
						me++;
					}
					if (board[k][i] == yourstone) { /* 相手の石 */
						you++;
					}
					if (board[k][i] == STONE_SPACE) { /* 空白をカウントして位置を記憶 */
						spacex[nix] = k;
						spacey[nix] = i;
						nix++;
					}
				}
				
				if (me == 4 && nix == 1) { /* 四連（次の手で勝利） */
					score[spacex[0]][spacey[0]] += win;
				}
				
				if (me == 3 && nix == 2) { /* 三連（四連を生成） */
					if (board[j][i] == STONE_SPACE && board[j + 4][i] == STONE_SPACE) {
						score[j][i] += sanren;
						score[j + 4][i] += sanren;
					}
					else { /* 三連ではない3つ */
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
			for (j = 0; j < BOARD_SIZE - 5; j++) { /* 飛び三連の評価 */
					
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
					if (board[k][i] == STONE_SPACE && board[k + 5][i] == STONE_SPACE) { /* 飛び三連なら */
						score[spacex[1]][spacey[1]] += tobi; /* 四連を作る位置に加点 */
					}
					else {
						score[spacex[0]][spacey[0]] += 3;
						score[spacex[1]][spacey[1]] += 3;
						score[spacex[2]][spacey[2]] += 3;
					}
				}
			}
		}
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* 右斜め下方向を横に走査 */
			
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
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* 右斜め下方向を縦に走査 */
			
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
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* 左斜め下方向を横に走査 */
			
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
		
		for (i = 0; i < BOARD_SIZE - 4; i++) { /* 左斜め下方向を縦に走査 */
			
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
// 評価
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

//=======================================================
// Group 11
//=======================================================

void strategy11(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, int count, const position *history);
void str11_b_fitcheck(const int board[][BOARD_SIZE], int count, int *pos_x, int *pos_y);
void str11_b_fitInit(int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE]);
void str11_b_fitfunc(const int board[][BOARD_SIZE], int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE], int atack[8], int defense[8], int which[8], int x, int xi, int xj, int a, int b, int c);
int str11_b_fitcal1(int number, int block);
int str11_b_fitcal2(int number, int block);

void strategy11(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, int count, const position *history)
{
	printf("Strategy 11\t");

	if (count == 0){
		*pos_x = BOARD_SIZE / 2;
		*pos_y = BOARD_SIZE / 2;
	}
	else if (count == 1){
		*pos_x = history[0].x + ((history[0].x < BOARD_SIZE / 2) ? 1 : -1);
		*pos_y = history[0].y + ((history[0].y < BOARD_SIZE / 2) ? 1 : -1);
	}
	else{
		str11_b_fitcheck(board, count, &*pos_x, &*pos_y);
	}
}

void str11_b_fitcheck(const int board[][BOARD_SIZE], int count, int *pos_x, int *pos_y)
{
	int i, j, x, xi, xj, a, b, c, which[8];
	int atack[8], defense[8];
	int b_fita[BOARD_SIZE][BOARD_SIZE], b_fitd[BOARD_SIZE][BOARD_SIZE];
	int num1 = (count % 2 == 0) ? 30 : 20;
	int num2 = (count % 2 == 0) ? 20 : 30;

	position elite;
	position elite_fit;
	elite.x = 0, elite.y = 0, elite_fit.x = 0, elite_fit.y = 0;
	str11_b_fitInit(b_fita, b_fitd);

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < 8; j++){
			atack[j] = 0;
			defense[j] = 0;
			which[j] = 0;
		}
		for (j = 0; j < BOARD_SIZE; j++) {
			x = 0, xi = i, xj = j, a = 0, b = 1, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			x = 1, xi = BOARD_SIZE - i - 1, xj = BOARD_SIZE - j - 1, a = 0, b = -1, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			x = 2, xi = j, xj = i, a = 1, b = 0, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			x = 3, xi = BOARD_SIZE - j - 1, xj = BOARD_SIZE - i - 1, a = -1, b = 0, c = j;
			str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
		}
	}
	for (i = 0; i < BOARD_SIZE - 4; i++){
		for (j = 0; j < 8; j++){
			atack[j] = 0;
			defense[j] = 0;
			which[j] = 0;
		}
		for (j = 0; j < BOARD_SIZE; j++){
			if (j + i < BOARD_SIZE){
				x = 4, xi = j, xj = j + i, a = 1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 5, xi = BOARD_SIZE - 1 - j, xj = j + i, a = -1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 6, xi = j, xj = BOARD_SIZE - 1 - (j + i), a = 1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 7, xi = BOARD_SIZE - 1 - j, xj = BOARD_SIZE - 1 - (j + i), a = -1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			}
		}
	}
	for (i = 1; i < BOARD_SIZE - 4; i++){
		for (j = 0; j < 8; j++){
			atack[j] = 0;
			defense[j] = 0;
			which[j] = 0;
		}
		for (j = 0; j < BOARD_SIZE; j++){
			if (j + i < BOARD_SIZE){
				x = 4, xi = j + i, xj = j, a = 1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 5, xi = BOARD_SIZE - 1 - (j + i), xj = j, a = -1, b = 1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 6, xi = j + i, xj = BOARD_SIZE - 1 - j, a = 1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
				x = 7, xi = BOARD_SIZE - 1 - (j + i), xj = BOARD_SIZE - 1 - j, a = -1, b = -1, c = j + i;
				str11_b_fitfunc(board, b_fita, b_fitd, atack, defense, which, x, xi, xj, a, b, c);
			}
		}
	}
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] != STONE_SPACE){
				b_fita[i][j] = 0;
				b_fitd[i][j] = 0;
			}
		}
	}

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (b_fita[i][j] > num1){
				if (elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
			}
			if (b_fitd[i][j] > num2){
				elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				i = BOARD_SIZE, j = BOARD_SIZE;
				break;
			}
			else if (elite_fit.x <= b_fita[i][j] || elite_fit.y <= b_fitd[i][j]){
				if (b_fita[i][j] > 12 && elite_fit.x < b_fita[i][j] || b_fitd[i][j] > 16 && elite_fit.y < b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] > 10 && elite_fit.y < 15){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] > 10 && elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] < 8 && b_fitd[i][j] > 8 && elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
				else if (b_fita[i][j] >= elite_fit.y && elite_fit.x + elite_fit.y <= b_fita[i][j] + b_fitd[i][j]){
					if (0 < i && i < BOARD_SIZE - 1 && 0 < j && j < BOARD_SIZE - 1){
						elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
					}
				}
				else if (b_fita[i][j] > elite_fit.x && b_fitd[i][j] > elite_fit.y){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
				}
			}
		}
	}

	if (i == BOARD_SIZE && j == BOARD_SIZE && elite_fit.x == 0 && elite_fit.y == 0){
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (board[i][j] == STONE_SPACE){
					elite.x = i, elite.y = j, elite_fit.x = b_fita[i][j], elite_fit.y = b_fitd[i][j];
					break;
				}
			}
		}
	}
	*pos_x = elite.y, *pos_y = elite.x;
}
void str11_b_fitfunc(const int board[][BOARD_SIZE], int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE], int atack[8], int defense[8], int which[8], int x, int xi, int xj, int a, int b, int c)
{
	if (board[xi][xj] == STONE_BLACK){
		if (defense[x] > 0){
			defense[x] = 1;
			if (atack[x] > 0 && which[x] == STONE_WHITE){
				atack[x] = 0;
			}
		}
		atack[x]++;
		which[x] = STONE_BLACK;
	}
	else if (board[xi][xj] == STONE_WHITE){
		if (atack[x] > 0){
			atack[x] = 1;
			if (defense[x] > 0 && which[x] == STONE_BLACK){
				defense[x] = 0;
			}
		}
		defense[x]++;
		which[x] = STONE_WHITE;
	}
	else{
		if (which[x] == STONE_BLACK){
			b_fitd[xi][xj] += 0;
			if (c == 0 && atack[x] < 4 || c == BOARD_SIZE - 1 && atack[x] < 4){
				b_fita[xi][xj] += 0;
			}
			else if (board[xi + a][xj + b] == STONE_WHITE && atack[x] < 4){
				b_fita[xi][xj] += 0;
			}
			else if (defense[x] == 0){
				if (board[xi + a][xj + b] == STONE_BLACK && board[xi + 2 * a][xj + 2 * b] != STONE_WHITE){
					b_fita[xi][xj] += 2 * str11_b_fitcal1(atack[x], 0);
				}
				else{
					b_fita[xi][xj] += str11_b_fitcal1(atack[x], 0);
				}
				if (0 < c && c < BOARD_SIZE - 1){
					b_fita[xi + a][xj + b] += str11_b_fitcal2(atack[x], 0);
				}
			}
			else{
				b_fita[xi][xj] += str11_b_fitcal1(atack[x], 1);
				if (0 < c && c < BOARD_SIZE - 1){
					b_fita[xi + a][xj + b] += str11_b_fitcal2(atack[x], 1);
				}
			}
		}
		else if (which[x] == STONE_WHITE){
			b_fita[xi][xj] += 0;
			if (c == 0 && defense[x] < 4 || c == BOARD_SIZE - 1 && defense[x] < 4){
				b_fitd[xi][xj] += 0;
			}
			else if (board[xi + a][xj + b] == STONE_BLACK && defense[x] < 4){
				b_fitd[xi][xj] += 0;
			}
			else if (atack[x] == 0){
				if (board[xi + a][xj + b] == STONE_WHITE && board[xi + 2 * a][xj + 2 * b] != STONE_BLACK){
					b_fitd[xi][xj] += 2 * str11_b_fitcal1(defense[x], 0);
				}
				else{
					b_fita[xi][xj] += str11_b_fitcal1(atack[x], 0);
				}
				b_fitd[xi][xj] += str11_b_fitcal1(defense[x], 0);
				if (0 < c && c < BOARD_SIZE - 1){
					b_fitd[xi + a][xj + b] += str11_b_fitcal2(defense[x], 0);
				}
			}
			else{
				b_fitd[xi][xj] += str11_b_fitcal1(defense[x], 1);
				if (0 < c && c < BOARD_SIZE - 1){
					b_fitd[xi + a][xj + b] += str11_b_fitcal2(defense[x], 1);
				}
			}
		}
		atack[x] = 0, defense[x] = 0, which[x] = 0;
	}
}

int str11_b_fitcal1(int number, int block)
{
	int n = number - 2 * block, two = 1, i;
	for (i = 0; i <= n; i++){
		two = two * 2;
	}

	return 2 * (1 - block) + two * (number - 1) + 20 * (number / 4) * block;
}

int str11_b_fitcal2(int number, int block)
{
	int n = number - 2 * block - 1, two = 1, i;
	for (i = 0; i <= n; i++){
		two = two * 2;
	}

	return 1 * (1 - block) + two * (number - 1) + 2 * (number / 3) + (1 / number - 1);
}

void str11_b_fitInit(int b_fita[][BOARD_SIZE], int b_fitd[][BOARD_SIZE])
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			b_fita[i][j] = STONE_SPACE;
			b_fitd[i][j] = STONE_SPACE;
		}
	}
}


//=======================================================
// Group 12
//=======================================================

void strategy12( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history );
void str12_thinking(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int self_stone, const int rival_stone, const int count, const position *history);
int str12_eval_firstphase(int x, int y, const int board[][BOARD_SIZE]);
int str12_eval_secondphase(int x, int y, const int board[][BOARD_SIZE]);

//-------------------------------------------------
// 味方アルゴリズム起動
//-------------------------------------------------
void strategy12(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	int check;
	int phase = 0;

	check = 0;
	while( check == 0 )
	{
		//先手後手チェック
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

//決定アルゴリズム
void str12_thinking(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int self_stone, const int rival_stone, const int count, const position *history){

	int eval[BOARD_SIZE][BOARD_SIZE] = {0};
	int max_eval = -1;
	double max_pos_x, max_pos_y;

	//一手目・三手目判定	
	//一手目だったら中のアルゴリズムを起動
	if(count == 0){
		*pos_x = 5;
		*pos_y = 5;
		return;
	//三手目だったら中のアルゴリズムを起動
	}else if(count == 2){
		
		//浦月・花月判定

		printf("%d\n", history[count - 2].x + history[count - 2].y);
		printf("%d\n", history[count - 1].x + history[count - 1].y);
		
		if((history[count - 2].x + history[count - 2].y - history[count - 1].x + history[count - 1].y) % 2 == 1 ){
			printf("Kagetsu");
			//花月
			*pos_x = history[count - 1].x - (history[count - 2].y - history[count - 1].y);
			*pos_y = history[count - 1].y - (history[count - 2].x - history[count - 1].x);
			
			return;
		}else if((history[count - 2].x + history[count - 2].y - history[count - 1].x + history[count - 1].y) % 2 == 0){
			printf("Fugetsu\n");
			//浦月
			*pos_x = history[count - 2].x - (history[count - 2].x - history[count - 1].x);
			*pos_y = history[count - 2].y + (history[count - 2].y - history[count - 1].y);
			
			return;
		}

	//一、三手目でなければ以下のプログラムを起動
	}else{
	//盤面上のすべての空きスペースの評価を検索//
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

	//最大評価値の検出
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

		*pos_x = (int)max_pos_x;
		*pos_y = (int)max_pos_y;

		return;
	}
	
	return;

}
//決定アルゴリズム終わり

//連続石チェック先手
int str12_eval_firstphase(int x, int y, const int board[][BOARD_SIZE]){
	
	int seq[4][9];
	int count[4] = {0}, count_self = 0;
	int distance = 0;
	int seq_enemy = 0;
	int eval = 0;
	
	//右肩下がり
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y - 4 + i < 0 || y - 4 + i > 9){
			seq[0][i] = - 1;
		}else{
			seq[0][i] = board[x - 4 + i][y - 4 + i];
		}
	}

	//右肩上がり
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y + 4 - i < 0 || y + 4 - i > 9){
			seq[1][i] = - 1;
		}else{
			seq[1][i] = board[x - 4 + i][y + 4 - i];
		}
	}

	//横まっすぐ
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9){
			seq[2][i] = - 1;
		}else{
			seq[2][i] = board[x - 4 + i][y];
		}
	}

	//縦まっすぐ
	for(int i = 0; i < 9; i++){
		if(y - 4 + i < 0 || y - 4 + i > 9){
			seq[3][i] = - 1;
		}else{
			seq[3][i] = board[x][y - 4 + i];
		}
	}


	
	//評価計算
	//８方向評価
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

//連続石チェック後手
int str12_eval_secondphase(int x, int y, const int board[][BOARD_SIZE]){
	
	int seq[4][9];
	int count[4] = {0}, count_self = 0;
	int distance = 0;
	int seq_enemy = 0;
	int eval = 0;
	
	//右肩下がり
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y - 4 + i < 0 || y - 4 + i > 9){
			seq[0][i] = - 1;
		}else{
			seq[0][i] = board[x - 4 + i][y - 4 + i];
		}
	}

	//右肩上がり
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9 || y + 4 - i < 0 || y + 4 - i > 9){
			seq[1][i] = - 1;
		}else{
			seq[1][i] = board[x - 4 + i][y + 4 - i];
		}
	}

	//横まっすぐ
	for(int i = 0; i < 9; i++){
		if(x - 4 + i < 0 || x - 4 + i > 9){
			seq[2][i] = - 1;
		}else{
			seq[2][i] = board[x - 4 + i][y];
		}
	}

	//縦まっすぐ
	for(int i = 0; i < 9; i++){
		if(y - 4 + i < 0 || y - 4 + i > 9){
			seq[3][i] = - 1;
		}else{
			seq[3][i] = board[x][y - 4 + i];
		}
	}
	
	//評価計算
	//８方向評価
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

//=======================================================
// Group 15
//=======================================================

void strategy15( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
	double tmp_d;
	int check, i, j;

	printf( "Strategy 15\t" );

	check = 0;
	while (check == 0)
	{
		//後攻定石
		if (count == 1){
			for (i = 0; i < BOARD_SIZE; i++){
				for (j = 0; j < BOARD_SIZE; j++){
					if (board[j][i] != STONE_SPACE){
						*pos_x = i + 1;
						*pos_y = j;
					}
				}
			}
		}

		//先攻定石一手目
		else if (count == 0){
			*pos_x = 5;
			*pos_y = 5;
		}
		//先攻定石二手目()
		else if (count == 2){
			int end_flag = 0;
			int kaisuu = 0;
			i = 4;
			j = 4;
			while (end_flag == 0){
				if ((board[j][i] == STONE_WHITE) && ((j + i) % 2 == 1)){
					if (i == 4 || i == 6){
						*pos_x = i;
						*pos_y = j + 1;
						end_flag = 1;
					}
					else {
						*pos_x = i + 1;
						*pos_y = j;
						end_flag = 1;
					}
				}
				else if ((board[j][i] == STONE_WHITE) && ((j + i) % 2 == 0)){
					if (i == 4){
						*pos_x = j;
						*pos_y = i + 2;
						end_flag = 1;
					}
					else {
						*pos_x = j;
						*pos_y = i - 2;
						end_flag = 1;
					}
				}
				else {
					if (kaisuu < 3){
						j++;
						kaisuu++;
					}
					else if (kaisuu == 3){
						i = 5;
						j = 4;
						kaisuu++;
					}
					else if (kaisuu < 6){
						j++;
						kaisuu++;
					}
					else if (kaisuu == 6){
						i = 6;
						j = 4;
						kaisuu++;
					}
					else if (kaisuu < 9){
						j++;
						kaisuu++;
					}
					else {
						*pos_x = 4;
						*pos_y = 6;
						end_flag = 1;
					}
				}
			}
		}


		// それ以降の戦略（先攻）
		else if (count % 2 == 0){

			int x, y, max_p;
			int count2, lp;
			int ans;
			int check_1[] = { 2, 2, 2, 0 };
			int check_2[] = { 2, 2, 0, 2, 0 };
			int check_3[] = { 1, 1, 1, 0 };
			int check_4[] = { 1, 1, 0, 1, 0 };

			max_p = 0;

			int BLACK_POINT[BOARD_SIZE][BOARD_SIZE];
			int WHITE_POINT[BOARD_SIZE][BOARD_SIZE];
			int BOARD_POINT[BOARD_SIZE][BOARD_SIZE];

			boardInit(BLACK_POINT);
			boardInit(WHITE_POINT);

			int dx[] = { 1, 0, -1, -1, -1, 0, 1, 1 };
			int dy[] = { -1, -1, -1, 0, 1, 1, 1, 0 };

			for (x = 0; x < BOARD_SIZE; x++){
				for (y = 0; y < BOARD_SIZE; y++){
					for (i = 0; i < 8; i++) {
						for (j = 1; j <= 4; j++) {
							if (board[y][x] == STONE_BLACK
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								BLACK_POINT[y][x] = -100;
								BLACK_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
							else if (board[y][x] == STONE_WHITE
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								WHITE_POINT[y][x] = -100;
								WHITE_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
						}
					}
				}
			}
			//自分の4連を5連にする
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (BLACK_POINT[y][x] >= 4 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}

								}
								count2 = 0;
							}
						}
					}
				}
			}
			// 相手の4連を止める
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (WHITE_POINT[y][x] >= 4 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}
								}
								count2 = 0;
							}
						}
					}
				}
			}
			//自分の両端空き3連を4連にする
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (BLACK_POINT[y][x] >= 3 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_3[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){

								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_4[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_4[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}
			// 相手の3連を止める(両端が開いている場合のみ
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (WHITE_POINT[y][x] >= 3 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_1[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){
								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_2[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_2[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}


			int win, lose, win_count, win_x, win_y, place, max_win, max_i, take, draw;
			place = 0;
			max_win = 0;
			int win_sum[100];
			int tmp_board[BOARD_SIZE][BOARD_SIZE];
			int retry_board[BOARD_SIZE][BOARD_SIZE];
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					tmp_board[i][j] = board[i][j];
					retry_board[i][j] = board[i][j];
				}
			}
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					BOARD_POINT[i][j] = BLACK_POINT[i][j] + WHITE_POINT[i][j];
					win_count = count;
					win = 0;
					lose = 0;
					draw = 0;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							retry_board[k][l] = board[k][l];
						}
					}
					if (BOARD_POINT[i][j] > 2){
						tmp_board[i][j] = STONE_BLACK;
						retry_board[i][j] = STONE_BLACK;
						//ループ回数を決める
						while (win + lose + draw < 2200){
							win_count++;
							take = 0;
							for (int k = 0; k < BOARD_SIZE; k++) {
								for (int l = 0; l < BOARD_SIZE; l++) {
									if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_BLACK) {
										win++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_WHITE){
										lose++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
								}
								if (win_count == count){
									break;
								}
							}
							if (win_count % 2 == 0 && win_count != count){
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_BLACK;
										take = 1;
									}
									else if (win_count >= 100){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
							else if (win_count % 2 == 1 && win_count != count) {
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_WHITE;
										take = 1;
									}
									else if (win_count >= 99){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
						}
					}
					win_sum[place] = win;
					place++;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							tmp_board[k][l] = board[k][l];
						}
					}
				}
			}
			for (i = 0; i < 100; i++){
				if (max_win < win_sum[i]){
					max_win = win_sum[i];
					max_i = i;
				}
			}
			y = max_i / 10;
			x = max_i % 10;
			*pos_y = y;
			*pos_x = x;
		}


		//後攻の戦略
		else if (count % 2 == 1){

			int x, y, max_p;
			int count2, plus, lp;
			int ans, tmp_x, tmp_y;
			int check_1[] = { 2, 2, 2, 0 };
			int check_2[] = { 2, 2, 0, 2, 0 };
			int check_3[] = { 1, 1, 1, 0 };
			int check_4[] = { 1, 1, 0, 1, 0 };

			max_p = 0;

			int BLACK_POINT[BOARD_SIZE][BOARD_SIZE];
			int WHITE_POINT[BOARD_SIZE][BOARD_SIZE];
			int BOARD_POINT[BOARD_SIZE][BOARD_SIZE];

			boardInit(BLACK_POINT);
			boardInit(WHITE_POINT);

			int dx[] = { 1, 0, -1, -1, -1, 0, 1, 1 };
			int dy[] = { -1, -1, -1, 0, 1, 1, 1, 0 };

			for (x = 0; x < BOARD_SIZE; x++){
				for (y = 0; y < BOARD_SIZE; y++){
					for (i = 0; i < 8; i++) {
						for (j = 1; j <= 4; j++) {
							if (board[y][x] == STONE_BLACK
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								BLACK_POINT[y][x] = -100;
								BLACK_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
							else if (board[y][x] == STONE_WHITE
								&& y + j*dy[i] < BOARD_SIZE
								&& x + j*dx[i] < BOARD_SIZE
								&& y + j*dy[i] >= 0
								&& x + j*dx[i] >= 0)
							{
								WHITE_POINT[y][x] = -100;
								WHITE_POINT[y + j*dy[i]][x + j*dx[i]] += 1;
							}
						}
					}
				}
			}

			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					//自分の4連を5連にする
					if (WHITE_POINT[y][x] >= 4 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}

								}
								count2 = 0;
							}
						}
					}
				}
			}
			// 相手の4連を止める
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (BLACK_POINT[y][x] >= 4 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 4; i++) {
							for (lp = -4; lp <= 0; lp++){
								for (j = lp; j <= lp + 4; j++) {
									if (board[y + j*dy[i]][x + j*dx[i]] == STONE_BLACK
										&& y + j*dy[i] < BOARD_SIZE
										&& x + j*dx[i] < BOARD_SIZE
										&& y + j*dy[i] >= 0
										&& x + j*dx[i] >= 0) {
										count2++;
									}
									else if (y + j*dy[i] == y && x + j*dx[i] == x){
										continue;
									}
									else if (board[y + j*dy[i]][x + j*dx[i]] == STONE_WHITE
										|| board[y + j*dy[i]][x + j*dx[i]] == STONE_SPACE
										|| y + j*dy[i] >= BOARD_SIZE
										|| x + j*dx[i] >= BOARD_SIZE
										|| y + j*dy[i] < 0
										|| x + j*dx[i] < 0) {
										count2 = 0;
										break;
									}
									if (count2 >= 4){
										*pos_x = x;
										*pos_y = y;
										return;
									}
								}
								count2 = 0;
							}
						}
					}
				}
			}
			//自分の空き3連を4連にする
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					if (WHITE_POINT[y][x] >= 3 && BLACK_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_1[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){
								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_2[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_2[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}
			for (y = 0; y < BOARD_SIZE; y++){
				for (x = 0; x < BOARD_SIZE; x++){
					// 相手の3連を止める(両端が開いている場合のみ
					if (BLACK_POINT[y][x] >= 3 && WHITE_POINT[y][x] >= 0){
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 4; j++) {
								if (check_3[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 4){
								*pos_x = x;
								*pos_y = y;
								return;
							}
						}
						for (i = 0; i < 8; i++) {
							ans = 0;
							for (j = 1; j <= 3; j++) {
								if (check_4[ans] == board[y + j*dy[i]][x + j*dx[i]]
									&& y + j*dy[i] < BOARD_SIZE
									&& x + j*dx[i] < BOARD_SIZE
									&& y + j*dy[i] >= 0
									&& x + j*dx[i] >= 0) {
									ans++;
								}
								else {
									break;
								}
							}
							if (ans == 3){
								for (int k = 1; k <= 2; k++){
									if (check_4[ans] == board[y - k*dy[i]][x - k*dx[i]]
										&& y - k*dy[i] < BOARD_SIZE
										&& x - k*dx[i] < BOARD_SIZE
										&& y - k*dy[i] >= 0
										&& x - k*dx[i] >= 0) {
										ans++;
									}
									else {
										ans = 0;
										break;
									}
								}
								if (ans == 5){
									*pos_x = x;
									*pos_y = y;
									return;
								}
							}
						}
					}
				}
			}

			int win, lose, win_count, win_x, win_y, place, max_win, max_i, take, draw, stop;
			place = 0;
			max_win = 0;
			int win_sum[100];
			int tmp_board[BOARD_SIZE][BOARD_SIZE];
			int retry_board[BOARD_SIZE][BOARD_SIZE];
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					tmp_board[i][j] = board[i][j];
					retry_board[i][j] = board[i][j];
				}
			}
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					BOARD_POINT[i][j] = BLACK_POINT[i][j] + WHITE_POINT[i][j];
					win_count = count;
					win = 0;
					lose = 0;
					draw = 0;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							retry_board[k][l] = board[k][l];
						}
					}
					if (BOARD_POINT[i][j] > 2){
						tmp_board[i][j] = STONE_WHITE;
						retry_board[i][j] = STONE_WHITE;
						while (win + lose + draw < 2200){
							win_count++;
							take = 0;
							for (int k = 0; k < BOARD_SIZE; k++) {
								for (int l = 0; l < BOARD_SIZE; l++) {
									if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_WHITE) {
										win++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else if (lenCheck(tmp_board, l, k) && tmp_board[k][l] == STONE_BLACK){
										lose++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
								}
								if (win_count == count){
									break;
								}
							}
							if (win_count % 2 == 0 && win_count != count){
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_WHITE;
										take = 1;
									}
									else if (win_count >= 100){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
							else if (win_count % 2 == 1 && win_count != count) {
								while (take == 0){
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_x = (int)(tmp_d * 10.0);
									tmp_d = (double)rand() / ((double)RAND_MAX + 1);
									win_y = (int)(tmp_d * 10.0);
									if (tmp_board[win_y][win_x] == STONE_SPACE){
										tmp_board[win_y][win_x] = STONE_BLACK;
										take = 1;
									}
									else if (win_count >= 99){
										take = 1;
										draw++;
										for (int m = 0; m < BOARD_SIZE; m++){
											for (int n = 0; n < BOARD_SIZE; n++){
												tmp_board[m][n] = retry_board[m][n];
											}
										}
										win_count = count;
									}
									else {
										win_count++;
									}
								}
							}
						}
					}
					win_sum[place] = win;
					place++;
					for (int k = 0; k < BOARD_SIZE; k++) {
						for (int l = 0; l < BOARD_SIZE; l++) {
							tmp_board[k][l] = board[k][l];
						}
					}
				}
			}
			for (i = 0; i < 100; i++){
				if (max_win < win_sum[i]){
					max_win = win_sum[i];
					max_i = i;
				}
			}
			y = max_i / 10;
			x = max_i % 10;
			*pos_y = y;
			*pos_x = x;
		}

		if (board[*pos_y][*pos_x] == STONE_SPACE){
			check = 1;
		}
	}
	return;
}

//=======================================================
// main
//=======================================================
int main( int argc, char **argv )
{
    // 変数宣言
    int board[BOARD_SIZE][BOARD_SIZE];
    position *history;
    int which_turn;
    int count;
    int group[2];

	if( argc != 3 )
	{
		group[0] = 6;
		group[1] = 1;
	/*
		printf( "gobang group_first group_second\n" );
		printf( "  group_first: 先攻のグループ番号\n" );
		printf( "  group_first: 後攻のグループ番号\n" );
		exit( 1 );
	*/
	}
	else
	{
		group[0] = atoi( argv[1] );
		group[1] = atoi( argv[2] );
		printf( "group_first (Ｘ) = %d\n", group[0] );
		printf( "group_second (Ｏ) = %d\n", group[1] );
		
		fprintf( stderr, "First: %d, Second: %d\n", group[0], group[1] );
	}

    // 初期処理
    gameInit(board, &which_turn);
    history = (position*)malloc( sizeof(position) * BOARD_SIZE * BOARD_SIZE );
    boardPrint(board);
    srand( (unsigned)time(NULL) );

    //---- メインループ
    for( count = 0; count < BOARD_SIZE * BOARD_SIZE; count++ ){
        //--- 入力処理
        inputPutPos(board, which_turn, count, history, group );

        //--- 出力処理
        boardPrint(board);

        //--- 終了判定
        if (gameEndProcess(board)) { break; }

        //--- 演算処理
        changeTurn(&which_turn);
    }

	if( count == BOARD_SIZE * BOARD_SIZE ){
		fprintf( stderr, "引き分けです\n" );
		printf( "引き分けです\n" );
	}

	free( history );

    return 0;
}

//-------------------------------------------------
// 置く場所入力
//-------------------------------------------------
void inputPutPos(int board[][BOARD_SIZE], const int which, const int count, position *history, const int *group)
{
    int pos_x, pos_y;
    char str[256];
    strategy strtgy[16] = {randomStrategy,
    					strategy1,
    					strategy3,
    					strategy3,
    					strategy4,
    					strategy5,
    					strategy6,
    					strategy7,
    					strategy8,
    					strategy9,
    					strategy10,
    					strategy11,
    					strategy12,
    					keyboardInput,// strategy13 が存在しないため，手打ち入力をここにいれている
    					strategy14,
    					strategy15
    					};
	struct tm *date;
	time_t current_time;
	
	time_t stime, etime;
	int ptime;
	
    printf("%s", (which == 1) ? "Ｘ" : "Ｏ");
    printf("の番です。どこに置きますか\n");

    while (1) {

		// 手の数と時刻を表示
		current_time = time( NULL );
		date = localtime( &current_time );
		strftime(str, 255, "%Y, %B, %d, %A %p%I:%M:%S", date);
		printf( "第 %d 手思考開始： Time: %s\n", count + 1, str );
    printf( "> " );
    /*
		fprintf( stderr, "第 %d 手思考開始： Time: %s\n", count + 1, str );
    */

    time(&stime);
		if( which == STONE_BLACK )
    {
			strtgy[ group[0] ]( board, &pos_x, &pos_y, count, history );
		}
    else if( which == STONE_WHITE ) {
			strtgy[ group[1] ]( board, &pos_x, &pos_y, count, history );
		}
		else
    {
			printf( "Invalid value of which in inputPutPos\n" );
			printf( "  which: %d\n", which );
			exit( 1 );
		}
		time(&etime);

		date = localtime( &current_time );
		strftime(str, 255, "%Y, %B, %d, %A %p%I:%M:%S", date);
		printf( "第 %d 手思考終了： Time: %s\n", count + 1, str );
    /*
		fprintf( stderr, "第 %d 手思考終了： Time: %s\n", count + 1, str );
    */

		ptime = difftime(etime, stime);
		
		if ( ptime >= 61 )
    {
		  printf("時間オーバー:%d\n", ptime);
		  printf("%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ" );
      /*
          fprintf( stderr, "時間オーバー:%d\n", ptime);
          fprintf( stderr, "%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ" );
      */
		  exit(1);
		}
    else
    {
		  printf("思考時間:%d\n", ptime);
		}
		printf( "%d %d\n", pos_x, pos_y );

        if (checkOutPos(pos_x, pos_y) && board[pos_y][pos_x] == STONE_SPACE) { break; }
        printf("不正な入力です\n> ");
        printf("%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ" );
        /*
        fprintf( stderr, "不正な入力です\n> ");
        fprintf( stderr, "%sの勝ちです。\n", (which == 1) ? "Ｏ" : "Ｘ" );
        */
        exit( 1 );
    }
    board[pos_y][pos_x] = which;

    history[count].x = pos_x;
    history[count].y = pos_y;
}

//-------------------------------------------------
// 手番交代処理
//-------------------------------------------------
void changeTurn(int *which_turn)
{
    *which_turn = (*which_turn == STONE_BLACK) ? STONE_WHITE : STONE_BLACK;
}

//-------------------------------------------------
// 範囲外チェック
//-------------------------------------------------
int checkOutPos(int x, int y)
{
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

//-------------------------------------------------
// ゲーム情報初期化
//-------------------------------------------------
void gameInit(int board[][BOARD_SIZE], int *which_turn)
{
    boardInit(board);
    *which_turn = STONE_BLACK;
}

//-------------------------------------------------
// 盤面初期化
//-------------------------------------------------
void boardInit(int board[][BOARD_SIZE])
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = STONE_SPACE;
        }
    }
}

//-------------------------------------------------
// 盤面出力
//-------------------------------------------------
void boardPrint(int board[][BOARD_SIZE])
{
    int i, j;

    printf("  ");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    puts("");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
        for (j = 0; j < BOARD_SIZE; j++) {
            switch (board[i][j]) {
            case STONE_SPACE: printf("・"); break;
            case STONE_BLACK: printf("Ｘ"); break;
            case STONE_WHITE: printf("Ｏ"); break;
            }
        }
        puts("");
    }
    puts("");
}

//-------------------------------------------------
// ゲーム終了処理
//-------------------------------------------------
int gameEndProcess(int board[][BOARD_SIZE])
{
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == STONE_SPACE) { continue; }
            if (lenCheck(board, j, i)) {
                printf( "五連箇所:%d %d\n", j, i );
                printf("%sの勝ちです。\n", (board[i][j] == STONE_BLACK) ? "Ｘ" : "Ｏ");
                /*
                fprintf( stderr, "%sの勝ちです。\n", (board[i][j] == STONE_BLACK) ? "Ｘ" : "Ｏ");
                */
                return 1;
            }
        }
    }
    return 0;
}


//-------------------------------------------------
// 5連確認
//-------------------------------------------------
int lenCheck(int board[][BOARD_SIZE], int x, int y)
{
    int i, j, len_flag;
    int dx[] = { 0, 1, 1, -1 };
    int dy[] = { 1, 0, 1, 1 };

    for (i = 0; i < 4; i++) {
        for (j = 1, len_flag = 1; j <= 4; j++) {
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
        if (len_flag == 1) { return 1; }
    }
    return 0;
}

//------------------------------------------------
// ランダムに手を打つ戦略
//------------------------------------------------
void randomStrategy( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
	double tmp_d;
	int check;

	printf( "randomStrategy selects the position randomly\n" );

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



//------------------------------------------------
// 人間（つまり手打ち入力）
//------------------------------------------------
void keyboardInput( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{
  scanf("%d %d", pos_x, pos_y);

	return;
}



