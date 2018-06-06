#pragma once
#include "group.h"

namespace Archive {
	
	class Strategy1 : public Strategy {
	private:
		int alphabeta(int board[][BOARD_SIZE], int depth, int which, int count, int alpha, int beta);
		int eval(int board[][BOARD_SIZE], int which, int count);

	public:
		void operator ()(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history) override;

	};

}