#include "groups.h"

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
	  return best_pos;
  }
}

