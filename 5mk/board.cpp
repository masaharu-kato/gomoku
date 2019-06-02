#include "board.h"
#include <algorithm>

auto F19::Board::getStone(X x, Y y) const -> Stone {
	return board[x][y];
}

bool F19::Board::isInside(X x, Y y) const {
	return x >= 0 && x < size && y >= 0 && y < size;
}

auto F19::Board::getStone(Pos p) const -> Stone {
	return getStone(p.x, p.y);
}

bool F19::Board::isInside(Pos p) const {
	return isInside(p.x, p.y);
}

auto F19::Board::getVertical(X x) const -> StoneLine::Ptr {
	return getLine(x, 0, 0, 1);
}

auto F19::Board::getHorizontal(Y y) const -> StoneLine::Ptr {
	return getLine(0, y, 1, 0);
}

auto F19::Board::getLeftDiagonal(X x) const -> StoneLine::Ptr {
	return getLine(x, 0, 1, 1);
}

auto F19::Board::getLeftDiagonal(Y y) const -> StoneLine::Ptr {
	return getLine(0, y, 1, 1);
}

auto F19::Board::getRightDiagonal(X x) const -> StoneLine::Ptr {
	return getLine(x, 0, -1, 1);
}

auto F19::Board::getRightDiagonal(Y y) const -> StoneLine::Ptr {
	return getLine(size - 1, y, -1, 1);
}

auto F19::Board::getLine(X begx, Y begy, X invx, Y invy) const -> StoneLine::Ptr {

	X cx = begx;
	Y cy = begy;
	StoneLine::Base line;

	while(isInside(cx, cy)) {
		line.push_back(getStone(cx, cy));
		cx += invx;
		cy += invy;
	}

	return StoneLine::instance(line);

}

void F19::Board::forEachLines(std::function<void(StoneLine::Ptr)> func) const {

	for(X x = 0; x < size; ++x) func(getVertical(x));
	for(Y y = 0; y < size; ++y) func(getHorizontal(y));
	for(X x = 0; x < size; ++x) func(getLeftDiagonal(x));
	for(Y y = 1; y < size; ++y) func(getLeftDiagonal(y));
	for(X x = 0; x < size; ++x) func(getRightDiagonal(x));
	for(Y y = 1; y < size; ++y) func(getRightDiagonal(y));
	
}

auto F19::Board::calc_value(Stone target) const -> Value {

	std::unordered_map<StoneLine::Steps, size_t> counts;

//	全ての並びの猶予値を調べる
//	std::vector<StoneLine::Steps> graces;
	forEachLines([&](StoneLine::Ptr ptr){
		auto grace = ptr->grace_steps(target);

		counts[grace] ++;

	//	graces.push_back(grace);
	//#ifdef _DEBUG
	//	std::cout << *ptr << "\tg:" << grace << "\n";
	//#endif
	});

	auto value = counts[-StoneLine::Steps_Infinity] * 500000
		+ counts[-1] * 100000
		+ counts[0] * 10000
		+ counts[1] * 1000
		+ counts[2] * 100
		+ counts[3] * 10
		+ counts[4];


////	昇順に並び替え
//	std::sort(graces.begin(), graces.end());
//
////	最初に値がインデックス値を下回るときの猶予値を求める
////	ただし、猶予値が負の場合/Infinity に到達した場合/猶予値リストの最後まで到達すれば打ち切る
//
//	StoneLine::Steps i = 0;
//	if(graces[i] >= 0) {
//		for(i = 0; i < graces.size(); i++) {
//			if(graces[i] == StoneLine::Steps_Infinity || graces[i] < i) break;
//		}
//	}
//
//	auto value = graces[i];
////
//#ifdef _DEBUG
//	std::cout << "graces: ";
//	for(auto grace : graces) std::cout << grace << ", ";
//	std::cout << "\n";
//	std::cout << "value: " << value << " (index: " << i << ")\n";
//#endif

	return value;
}



void F19::Board::calc_next_values(Stone target) {

	std::unordered_map<Pos, Value, Pos::Hash> values;

	forEachEmpties([&](Pos p){
	//	std::cout << p.x << ", " << p.y << "\n";
		setStone(p, target);

		auto value = - calc_value(target) + calc_value(Stone::reverse(target));
		
		values.insert_or_assign(p, value);

		setStone(p, Stone::None);
	});

#ifdef _DEBUG
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			Pos p(x, y);
			if(values.count(p)){
				printf("%+7d ", values[p]);
			}else{
				auto cstone = getStone(p);
				if(cstone == Stone::None) throw std::exception("None stone is impossible here.");
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


//	入力関数
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

//	出力関数
std::ostream& F19::operator <<(std::ostream& os, const Board& board) {
	for(Y y = 0; y < Board::size; ++y) {
		for(X x = 0; x < Board::size; ++x) {
			os << board.getStone(x, y).getChar();
		}
		os << "\n";
	}
	return os;
}