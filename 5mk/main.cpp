#include "core.h"
#include "groups.h"

//=======================================================
// main
//=======================================================
int main( int argc, char **argv )
{
    // 変数宣言
    int group[2];

	if(argc != 3) {
		printf("先攻のグループ番号："); scanf("%d", &group[0]);
		printf("後攻のグループ番号："); scanf("%d", &group[1]);
	}
	else {
		group[0] = atoi( argv[1] );
		group[1] = atoi( argv[2] );
	}

	printf( "group_first (Ｘ) = %d\n", group[0] );
	printf( "group_second (Ｏ) = %d\n", group[1] );
		
	fprintf( stderr, "First: %d, Second: %d\n", group[0], group[1] );
	

//	初期処理
    int board[BOARD_SIZE][BOARD_SIZE];
    position *history;
    int which_turn;

    gameInit(board, &which_turn);
    history = (position*)malloc( sizeof(position) * BOARD_SIZE * BOARD_SIZE );
    boardPrint(board);
    srand( (unsigned)time(NULL) );

//	メインループ
	int count;
    for(count = 0; count < BOARD_SIZE * BOARD_SIZE; count++){
        //--- 入力処理
        inputPutPos(board, which_turn, count, history, group );

        //--- 出力処理
        boardPrint(board);

        //--- 終了判定
        if (gameEndProcess(board)) { break; }

        //--- 演算処理
        changeTurn(&which_turn);

		system("pause");
    }

	if(count == BOARD_SIZE * BOARD_SIZE){
		fprintf( stderr, "引き分けです\n" );
		printf( "引き分けです\n" );
	}

	free(history);

	system("pause");
    return 0;
}
