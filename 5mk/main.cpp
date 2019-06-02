#include "board.h"
#include <iostream>

using namespace F19;

int main(void) {

	Board board;
	std::cin >> board;

	//std::cout << "--Black---------\n";
	//board.calc_value(Stone::Black);

	//std::cout << "--White---------\n";
	//board.calc_value(Stone::White);
	
	std::cout << "--Black---------\n";
	board.calc_next_values(Stone::Black);

	std::cout << "--White---------\n";
	board.calc_next_values(Stone::White);
	
	std::cout << "--Instances-----\n";
	StoneLine::out_instances(std::cout);

	return 0;
}