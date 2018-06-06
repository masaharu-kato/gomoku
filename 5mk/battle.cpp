#include "core.h"

void battle(int group[2]) {

	printf( "group_first (�w) = %d\n", group[0] );
	printf( "group_second (�n) = %d\n", group[1] );
		
	fprintf( stderr, "First: %d, Second: %d\n", group[0], group[1] );
	
//	��������
    int board[BOARD_SIZE][BOARD_SIZE];
    position *history;
    int which_turn;

    gameInit(board, &which_turn);
    history = new position[BOARD_SIZE * BOARD_SIZE];
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
        if (gameEndProcess(board)) break;

        //--- ���Z����
        changeTurn(&which_turn);

		system("pause");
    }

	if(count == BOARD_SIZE * BOARD_SIZE){
		fprintf( stderr, "���������ł�\n" );
		printf( "���������ł�\n" );
	}

	delete[] history;
}
