#include "strategy.h"
using namespace F19;

void F19::strategy(const int board_array[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) {
	
	static bool is_first_called = false;

//	メッセージ
	if(!is_first_called && count < 2) {
		std::cout << "ちょっとまってね...\n";
		is_first_called = true;
	}


//	現在の石の色を算出
	Stone cstone = (count % 2 == 0) ? Stone::Black : Stone::White;

//	盤面を読み込む
	Board board;

	for(Y y = 0; y < Board::size; y++) {
		for(X x = 0; x < Board::size; x++) {
			board.setStone(x, y, (Stone::StoneType)board_array[y][x]);
		}
	}

//	石を置く位置を計算
	Pos next_pos = board.calcNextPosition(cstone);

//	位置を代入
	*pos_x = next_pos.x;
	*pos_y = next_pos.y;

	return;
}