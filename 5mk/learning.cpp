#include "learning.h"
#include <limits>

using namespace F19;

Learning::Learning(StoneLine init) {
	learn(init, Stone::White);
}

auto Learning::learn(StoneLine line, Stone turn) -> StepsData {

	if(line_to_steps.count(line)) return line_to_steps[line];

	int value = evaluate(line);

	if(value) return line_to_steps[line] = {0, 0};

	Stone next_turn = turn == Stone::Black ? Stone::White : turn == Stone::White ? Stone::Black : Stone::None;

	Steps min_steps_win  = std::numeric_limits<size_t>::max();
	Steps max_steps_lose = std::numeric_limits<size_t>::min();

	for(auto& stone : line) {
		if(stone == Stone::None) {

			auto stone_original = stone;
			stone = turn;
			auto c_sdat = learn(line, next_turn);
			stone = stone_original;

			c_sdat.steps_win ++;
			c_sdat.steps_lose ++;
			
			if(min_steps_win  > c_sdat.steps_win ) min_steps_win  = c_sdat.steps_win; 
			if(max_steps_lose < c_sdat.steps_lose) max_steps_lose = c_sdat.steps_lose; 
		}
	}

	return line_to_steps[line] = {min_steps_win, max_steps_lose};

}


//	•5˜A‘±‚ª‚ ‚ê‚Î -1, ”’5˜A‘±‚ª‚ ‚ê‚Î +1 ,—¼•û‚Æ‚à‚ ‚é/—¼•û‚Æ‚à‚È‚¢ê‡ 0 ‚Ì•]‰¿’l‚ð•Ô‚·
int Learning::evaluate(StoneLine line) {

	int n_con_black = 0;
	int n_con_white = 0;
	int value = 0;

	for(auto stone : line) {
		if(stone == Stone::None) {
			n_con_black = 0;
			n_con_white = 0;
		}
		else if(stone == Stone::Black) {
			n_con_black ++;
			n_con_white = 0;
		}
		else if(stone == Stone::White) {
			n_con_black = 0;
			n_con_white++;
		}
		if(n_con_black == 5) value --;
		if(n_con_white == 5) value ++;
	}

	return value;
}
