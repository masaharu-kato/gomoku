#include "core.h"


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
