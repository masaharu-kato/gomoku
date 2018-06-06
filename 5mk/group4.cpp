#include "groups.h"

//=======================================================
// Group 4
//=======================================================

void strategy4( const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history )
{

  int myturn,enemy;//é©êgÇÃçïîíÇîªíË
  
  if(count%2==0){
    myturn = STONE_BLACK;
    enemy = STONE_WHITE;
  }else if(count%2==1){
    myturn = STONE_WHITE;
    enemy = STONE_BLACK;
  }
    double tmp_d;
    int check,exit_cn;
  if(count==0||count==1){//èâéË
 
      check = 0;
      while( check == 0 ){//ê^ÇÒíÜïtãﬂÇ…ë≈Ç¬
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
	  for (k = 4; k > 0; k--) {//ï¿ÇÒÇ≈Ç¢ÇÈå¬êî---my_stone
	    for(i = 0; i < 8 ; i++){//ï˚å¸ÇíËÇﬂÇÈ(ÇWï˚å¸)
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
		  return ;
		}
		if(k==3){
		  if(board[y-dy[i]][x-dx[i]]==myturn&&y-dy[i]>=0&&y-dy[i]<10&&x-dx[i]>=0&&x-dx[i]<10){
		    my_sum = my_sum + 10000;
		  }
		}
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
	  for (k = 4; k > 1; k--) {//ï¿ÇÒÇ≈Ç¢ÇÈå¬êî---enemy
	    for(i = 0; i < 8 ; i++){//ï˚å¸ÇíËÇﬂÇÈ(ÇWï˚å¸)
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
		}
		if(k==3){
		  if(board[y+(j+1)*dy[i]][x+(j+1)*dx[i]]==STONE_SPACE){
		    enemy_sum = enemy_sum + 100;
		  }
		  if(board[y-dy[i]][x-dx[i]]==enemy){
		    enemy_sum = enemy_sum + 1000;
		  }
		}
		if(k==2){
		  if(board[y-dy[i]][x-dx[i]]==enemy){
		    if(board[y-2*dy[i]][x-2*dx[i]]==enemy){
		      enemy_sum = enemy_sum + 1000;
		    }else{
		      enemy_sum = enemy_sum + 100;
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
	  return;
	}
      }
  }

}

