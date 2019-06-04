#include "board.h"
#include <iostream>

using namespace F19;

int main(void) {

	Stone stone;
	size_t n_board;
	decltype(StoneLine::miss_coef) miss_coef;
	decltype(Board::val_exp_lv) val_exp_lv;

	std::cin >> n_board >> stone >> miss_coef >> val_exp_lv;

	StoneLine::miss_coef = miss_coef;
	Board::val_exp_lv = val_exp_lv;
	
	std::cout << "miss_coef: " << StoneLine::miss_coef
		<< ", val_exp_lv: " << Board::val_exp_lv << "\n";

	for(size_t i = 1; i <= n_board; i++) {

		Board board;
		std::cin >> board;
	
		std::cout << stone << i << "\n";
		board.calc_next_values(stone);

		stone = Stone::reverse(stone);

	}
	
	return 0;
}