#include "groups.h"


//=======================================================
// Group 2
//=======================================================


int str2_scan(int x,int y,int a,int b,const int board[][BOARD_SIZE]);
void strategy2(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history);

int str2_scan(int x,int y,int a,int b,const int board[][BOARD_SIZE]){ //a=0:左上から　1:真上から　2:右上から 3:真横から
    //b=1:黒　2:白
    int dx[]={-1,1,0,0,1,-1,-1,1};
    int dy[]={-1,1,-1,1,-1,1,0,0};
    int len=0;

    for(int i=1;i<=4;i++){
        if (b==board[y+i*dy[2*a]][x+i*dx[2*a]]
           && y+i*dy[2*a] < BOARD_SIZE
           && x+i*dx[2*a] < BOARD_SIZE
           && y+i*dy[2*a] > 0
           && x+i*dx[2*a] > 0)
        {
            len++;
        }
        else{
            break;
        }
        if(len==4){
            return 100000;
        }
    }
    for(int i=1;i<=4;i++){
        if (b==board[y+i*dy[2*a+1]][x+i*dx[2*a+1]]
            && y+i*dy[2*a+1] < BOARD_SIZE
            && x+i*dx[2*a+1] < BOARD_SIZE
            && y+i*dy[2*a+1] > 0
            && x+i*dx[2*a+1] > 0)
        {
            len++;
        }
        else{
            break;
        }
        if(len==4){
            return 100000;
        }
    }
    if(len==3){
        return 5000;
    }
    if(len==2){
        return 2000;
    }
    if(len==1){
        return 1000;
    }
    return 0;
}




void strategy2(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history){
    int search[BOARD_SIZE][BOARD_SIZE];

    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            search[i][j]=0;
        }
    }
    
    if(count==0){ //先手の初手は4,5
        *pos_x=4;
        *pos_y=5;
        return;
    }
    if(count==1){  //後手の初手は先手のななめ
        *pos_x=history[0].x+1;
        *pos_y=history[0].y+1;
        return;
    }
    
    if(count==2){
        if(board[6][5]!=0){
            *pos_x=6;
            *pos_y=5;
            return;
        }
        else{
            *pos_x=2;
            *pos_y=5;
            return;
        }
    }
    for(int i=0;i<count;i++){
        int p=history[i].x;
        int q=history[i].y;
        if(board[q+1][p+1]==0 && p+1 < BOARD_SIZE && q+1 < BOARD_SIZE){
            search[q+1][p+1]=1;
        }
        if(board[q+1][p]==0 && q+1 < BOARD_SIZE && p >= 0 && p < BOARD_SIZE){
            search[q+1][p]=1;
        }
        if(board[q+1][p-1]==0 && q+1 < BOARD_SIZE && p-1 >= 0 ){
            search[q+1][p-1]=1;
        }
        if(board[q][p+1]==0 && q >= 0 && q < BOARD_SIZE && p+1 < BOARD_SIZE){
            search[q][p+1]=1;
        }
        if(board[q-1][p-1]==0 && p-1 >= 0 && q-1 >= 0 ){
            search[q-1][p-1]=1;
        }
        if(board[q][p-1]==0 && q >= 0 && q < BOARD_SIZE && p-1 >= 0 ){
            search[q][p-1]=1;
        }
        if(board[q-1][p+1]==0 && q-1 >= 0 && p+1 < BOARD_SIZE){
            search[q-1][p+1]=1;
        }
        if(board[q-1][p]==0 && q-1 >= 0 &&  p >= 0 && p < BOARD_SIZE){
            search[q-1][p]=1;
        }
    }
    
    
    int max_x=0,max_y=0,max;
    int s;
    max=0;
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            double f=0;
            if(search[j][i]==1){
                for(int a=0;a<4;a++){
                    s=str2_scan(i,j,a,count%2+2,board);
                    
                    f+=3/2*s;
                    s=str2_scan(i,j,a,count%2+3,board);
                    f+=s/2;
                    if(max<f){
                        max_x=i;
                        max_y=j;
                        max=(int)f;
                    }
                }
            }
        }
    }
    
    *pos_x=max_x;
    *pos_y=max_y;
    return;
}

