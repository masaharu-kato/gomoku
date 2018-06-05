#include "core.h"
#include "groups.h"

//=======================================================
// main
//=======================================================
int main( int argc, char **argv )
{
    // �ϐ��錾
    int group[2];

	if(argc != 3) {
		printf("��U�̃O���[�v�ԍ��F"); scanf("%d", &group[0]);
		printf("��U�̃O���[�v�ԍ��F"); scanf("%d", &group[1]);
	}
	else {
		group[0] = atoi( argv[1] );
		group[1] = atoi( argv[2] );
	}

	printf( "group_first (�w) = %d\n", group[0] );
	printf( "group_second (�n) = %d\n", group[1] );
		
	fprintf( stderr, "First: %d, Second: %d\n", group[0], group[1] );
	

//	��������
    int board[BOARD_SIZE][BOARD_SIZE];
    position *history;
    int which_turn;

    gameInit(board, &which_turn);
    history = (position*)malloc( sizeof(position) * BOARD_SIZE * BOARD_SIZE );
    boardPrint(board);
    srand( (unsigned)time(NULL) );

//	���C�����[�v
	int count;
    for(count = 0; count < BOARD_SIZE * BOARD_SIZE; count++){
        //--- ���͏���
        inputPutPos(board, which_turn, count, history, group );

        //--- �o�͏���
        boardPrint(board);

        //--- �I������
        if (gameEndProcess(board)) { break; }

        //--- ���Z����
        changeTurn(&which_turn);

		system("pause");
    }

	if(count == BOARD_SIZE * BOARD_SIZE){
		fprintf( stderr, "���������ł�\n" );
		printf( "���������ł�\n" );
	}

	free(history);

	system("pause");
    return 0;
}
