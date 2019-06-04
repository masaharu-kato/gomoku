#include "board.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <stdexcept>

F19::Board::Board() = default;

auto F19::Board::getStone(X x, Y y) const -> Stone {
	return board[x][y];
}

bool F19::Board::isInside(X x, Y y) {
	return x >= 0 && x < size && y >= 0 && y < size;
}

auto F19::Board::getStone(Pos p) const -> Stone {
	return getStone(p.x, p.y);
}

bool F19::Board::isInside(Pos p) {
	return isInside(p.x, p.y);
}

auto F19::Board::getLine(X begx, Y begy, X invx, Y invy) -> Line {

	X cx = begx;
	Y cy = begy;
	Line poses;

	while(isInside(cx, cy)) {
		poses.emplace_back(cx, cy);
		cx += invx;
		cy += invy;
	}

	return poses;

}

void F19::Board::generateLines() {

	size_t i = 0;
	for(X x = 0; x < size; ++x) lines[i++] = getLine(x, 0, 0, 1);
	for(Y y = 0; y < size; ++y) lines[i++] = getLine(0, y, 1, 0);
	for(X x = 0; x < size; ++x) lines[i++] = getLine(x, 0, 1, 1);
	for(Y y = 1; y < size; ++y) lines[i++] = getLine(0, y, 1, 1);
	for(X x = 0; x < size; ++x) lines[i++] = getLine(x, 0, -1, 1);
	for(Y y = 1; y < size; ++y) lines[i++] = getLine(size - 1, y, -1, 1);

	f_lines_generated = true;
	
}


auto F19::Board::calcNextPosition(Stone target) -> Pos {

	PosValueMap values;

	Stone astone = target;
	Stone estone = target.getReversed();

//	全ての位置の評価値を計算する
	forEachLines([&](const Line& line, StoneLine::Ptr ptr){
		for(size_t i = 0; i < line.size(); i++) {
			auto cpos = line[i];
			if(getStone(cpos) == Stone::None) {
				values[cpos] +=
					         val_ally_weight  * -log(1 - pow(ptr->next_value(astone, i), val_exp_lv))
					+ (1.0 - val_ally_weight) * -log(1 - pow(ptr->next_value(estone, i), val_exp_lv));
			}
		}
	});

//	最大値をとる位置を求める

	Pos max_pos;
	StoneLine::Value max_value;
	bool isfirst = true;

	for(const auto& i : values) {
		if(isfirst || max_value < i.second){
			max_pos = i.first;
			max_value = i.second;
		}
		isfirst = false;
	}

	if(isfirst) throw std::exception("No empty positions.");


#ifdef _DEBUG
	std::cout << "\n==== F19 Debug Information ====\n";
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			Pos p(x, y);
			if(values.count(p)){
				auto cval = values[p];
				if(std::isinf(cval)) cval = 99.999999;
				printf("%+10.6f ", cval);
			}else{
				auto cstone = getStone(p);
			//	if(cstone == Stone::None) throw std::exception("None stone is impossible here.");
				std::cout << "    [" << cstone << "]    ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "================================\n";
#endif

	return max_pos;

};




void F19::Board::setStone(X x, Y y, Stone stone) {
	board[x][y] = stone;
}

void F19::Board::setStone(Pos p, Stone stone) {
	return setStone(p.x, p.y, stone);
}

//void F19::Board::forEachEmpties(std::function<void(Pos)> func) const {
//	for(Y y = 0; y < Board::size; ++y) {
//		for(X x = 0; x < Board::size; ++x) {
//			if(getStone(x, y) == Stone::None) func({x, y});
//		}
//	}
//}
//
//void F19::Board::forEachEmpties(std::function<void(X, Y)> func) const {
//	for(Y y = 0; y < Board::size; ++y) {
//		for(X x = 0; x < Board::size; ++x) {
//			if(getStone(x, y) == Stone::None) func(x, y);
//		}
//	}
//}

void F19::Board::forEachLines(std::function<void(const Line&, StoneLine::Ptr)> func) const {

	if(!f_lines_generated) generateLines();

	for(const auto& line : lines) func(line, generateStoneLine(line));

}

auto F19::Board::generateStoneLine(const Line& line) const -> StoneLine::Ptr {
	StoneLine::Base base;
	for(auto pos : line) {
		base.push_back(getStone(pos));
	}
	return StoneLine::instance(base);
}


//	入力関数
std::istream& F19::operator >>(std::istream& is, Board& board) {
	for(Y y = 0; y < Board::size; ++y) {
		std::string str;
		is >> str;
		if(!y && !str.length()) continue;
		if(str.length() != Board::size) throw std::exception("Invalid input length.");
		for(X x = 0; x < Board::size; ++x) {
			board.setStone(x, y, Stone(str[x]));
		}
	}
	return is;
}

//	出力関数
std::ostream& F19::operator <<(std::ostream& os, const Board& board) {
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			os << board.getStone(x, y);
		}
		os << "\n";
	}
	return os;
}


//	static 変数の初期化

//	評価値の累乗係数
int F19::Board::val_exp_lv = 1;

//	評価値の自分の評価値の重み係数
double F19::Board::val_ally_weight = 0.5;

//	盤面のすべての列を生成したかどうか
bool F19::Board::f_lines_generated = false;

//	盤面のすべての列
F19::Board::AllLines F19::Board::lines;