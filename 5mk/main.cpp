#include "Learning.h"
#include <iostream>

using namespace F19;

int main(void) {

	while(std::cin){
		Learning::StoneLine stone_line;
		std::cin >> stone_line;

		std::cout << "value: " << Learning::evaluate(stone_line) << "\n";
	}

	return 0;
}