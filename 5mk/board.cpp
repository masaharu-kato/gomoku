#include "board.h"
#include <algorithm>

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


void F19::Board::calc_next_values(Stone target) {

	PosValueMap values;

//	‘S‚Ä‚Ì•À‚Ñ‚Ì—P—\’l‚ð’²‚×‚é
	forEachLines([&](const Line& line, StoneLine::Ptr ptr){
		for(size_t i = 0; i < line.size(); i++) {
			auto cpos = line[i];
			if(getStone(cpos) == Stone::None) {
				auto cval = ptr->next_value(target, i);
				if(values[cpos] < cval) values[cpos] = cval;
			}
		}
	});

#ifdef _DEBUG
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			Pos p(x, y);
			if(values.count(p)){
				printf("%+7.4f ", values[p]);
			}else{
				auto cstone = getStone(p);
			//	if(cstone == Stone::None) throw std::exception("None stone is impossible here.");
				std::cout << "  [" << cstone.getChar() << "]   ";
			}
		}
		std::cout << "\n";
	}
#endif

};




void F19::Board::setStone(X x, Y y, Stone stone) {
	board[x][y] = stone;
}

void F19::Board::setStone(Pos p, Stone stone) {
	return setStone(p.x, p.y, stone);
}

void F19::Board::forEachEmpties(std::function<void(Pos)> func) const {
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			if(getStone(x, y) == Stone::None) func({x, y});
		}
	}
}

void F19::Board::forEachEmpties(std::function<void(X, Y)> func) const {
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			if(getStone(x, y) == Stone::None) func(x, y);
		}
	}
}

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


//	“ü—ÍŠÖ”
std::istream& F19::operator >>(std::istream& is, Board& board) {
	for(Y y = 0; y < Board::size; ++y) {
		std::string str;
		is >> str;
		if(str.length() < Board::size) throw std::exception("Invalid input length.");
		for(X x = 0; x < Board::size; ++x) {
			board.setStone(x, y, Stone(str[x]));
		}
	}
	return is;
}

//	o—ÍŠÖ”
std::ostream& F19::operator <<(std::ostream& os, const Board& board) {
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			os << board.getStone(x, y).getChar();
		}
		os << "\n";
	}
	return os;
}

bool F19::Board::f_lines_generated = false;
F19::Board::AllLines F19::Board::lines;