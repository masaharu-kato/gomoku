#include "core.h"
#include "groups.h"


//-------------------------------------------------
// �u���ꏊ����
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
    					keyboardInput,// strategy13 �����݂��Ȃ����߁C��ł����͂������ɂ���Ă���
    					strategy14,
    					strategy15
    					};
	struct tm *date;
	time_t current_time;
	
	time_t stime, etime;
	int ptime;
	
    printf("%s", (which == 1) ? "�w" : "�n");
    printf("�̔Ԃł��B�ǂ��ɒu���܂���\n");

    while (1) {

		// ��̐��Ǝ�����\��
		current_time = time( NULL );
		date = localtime( &current_time );
		strftime(str, 255, "%Y, %B, %d, %A %p%I:%M:%S", date);
		printf( "�� %d ��v�l�J�n�F Time: %s\n", count + 1, str );
    printf( "> " );
    /*
		fprintf( stderr, "�� %d ��v�l�J�n�F Time: %s\n", count + 1, str );
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
		printf( "�� %d ��v�l�I���F Time: %s\n", count + 1, str );
    /*
		fprintf( stderr, "�� %d ��v�l�I���F Time: %s\n", count + 1, str );
    */

		ptime = (int)difftime(etime, stime);
		
		if ( ptime >= 61 )
    {
		  printf("���ԃI�[�o�[:%d\n", ptime);
		  printf("%s�̏����ł��B\n", (which == 1) ? "�n" : "�w" );
      /*
          fprintf( stderr, "���ԃI�[�o�[:%d\n", ptime);
          fprintf( stderr, "%s�̏����ł��B\n", (which == 1) ? "�n" : "�w" );
      */
		  exit(1);
		}
    else
    {
		  printf("�v�l����:%d\n", ptime);
		}
		printf( "%d %d\n", pos_x, pos_y );

        if (checkOutPos(pos_x, pos_y) && board[pos_y][pos_x] == STONE_SPACE) { break; }
        printf("�s���ȓ��͂ł�\n> ");
        printf("%s�̏����ł��B\n", (which == 1) ? "�n" : "�w" );
        /*
        fprintf( stderr, "�s���ȓ��͂ł�\n> ");
        fprintf( stderr, "%s�̏����ł��B\n", (which == 1) ? "�n" : "�w" );
        */
        exit( 1 );
    }
    board[pos_y][pos_x] = which;

    history[count].x = pos_x;
    history[count].y = pos_y;
}
