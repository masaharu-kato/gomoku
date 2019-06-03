#include "strategy.h"
using namespace F19;

void F19::strategy(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {
	
	static bool is_first_called = false;

//	���b�Z�[�W
	if(!is_first_called && count < 2) {
		std::cout << "������Ƃ܂��Ă�...\n";
		is_first_called = true;
	}


//	���݂̐΂̐F���Z�o
	Stone cstone = (count % 2 == 0) ? Stone::Black : Stone::White;

//	�Ֆʂ�ǂݍ���
	Board board;

	for(Y y = 0; y < Board::size; y++) {
		for(X x = 0; x < Board::size; x++) {
			board.setStone(x, y, (Stone::StoneType)board_array[y][x]);
		}
	}

//	�΂�u���ʒu���v�Z
	Pos next_pos = board.calcNextPosition(cstone);

//	�ʒu����
	*pos_x = next_pos.x;
	*pos_y = next_pos.y;

	return;
}