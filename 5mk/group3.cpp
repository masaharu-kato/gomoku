#include "groups.h"


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
