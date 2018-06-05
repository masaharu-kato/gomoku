#include "core.h"
#include "groups.h"


//-------------------------------------------------
// 置く場所入力
//-------------------------------------------------
void inputPutPos(int board[][BOARD_SIZE], const int which, const int count, position *history, const int *group)
{
    int pos_x, pos_y;
    char str[256];
    strategy strtgy[16] = {randomStrategy,
    					strategy1,
    					strategy2,
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

		ptime = (int)difftime(etime, stime);
		
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
