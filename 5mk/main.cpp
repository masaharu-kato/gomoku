#include "StoneLine.h"
#include <iostream>

using namespace F19;

int main(void) {

	//while(std::cin){
	//	StoneLine stone_line;
	//	std::cin >> stone_line;

	//	std::cout << "value: " << Learning::evaluate(stone_line) << "\n";
	//}

	auto stone_line = StoneLine::instance({10, Stone::None}); //->getWith(4, Stone::Black);
	stone_line->win_steps    (Stone::Black);
	stone_line->blocked_steps(Stone::Black);
	stone_line->win_steps    (Stone::White);
	stone_line->blocked_steps(Stone::White);

	StoneLine::out_instances(std::cout);

	//std::cout << stone_line.getWith(3, Stone::Black) << "\n";
	//std::cout << stone_line.getWith(8, Stone::White) << "\n";
	//std::cout << stone_line.getWith(9, Stone::White) << "\n";
	//std::cout << stone_line.getWith(0, Stone::Black) << "\n";
	//std::cout << stone_line.getWith(3, Stone::Black).getWith(4, Stone::Black) << "\n";
	//std::cout << stone_line.getWith(8, Stone::White).getWith(2, Stone::Black) << "\n";
	//std::cout << stone_line.getWith(9, Stone::White).getWith(3, Stone::White).getWith(1, Stone::White) << "\n";
	//std::cout << stone_line.getWith(0, Stone::Black).getWith(1, Stone::White).getWith(6, Stone::Black).getReversed() << "\n";

	return 0;
}