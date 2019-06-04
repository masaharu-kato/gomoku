#include "stone.h"
#include <stdexcept>

F19::Stone::Stone()
	: stone(None) {}

F19::Stone::Stone(StoneType stone)
	: stone(stone) {}

F19::Stone::Stone(Char c) {
	if(c == (Char)StoneChar::None ){
		stone = Stone::None;
	}
	else if(c == (Char)StoneChar::Black){
		stone = Stone::Black;
	}
	else if(c == (Char)StoneChar::White){
		stone = Stone::White;
	}
	else{
		throw std::exception("Invalid stone type.");
	}
}

auto F19::Stone::getChar() const -> Char {
	if(stone == Stone::None ) return (Char)StoneChar::None;
	if(stone == Stone::Black) return (Char)StoneChar::Black;
	if(stone == Stone::White) return (Char)StoneChar::White;
	throw std::exception("Invalid stone type.");
}

size_t F19::Stone::getID() const {
	return stone;
}

bool F19::Stone::operator ==(Stone _stone) const {
	return stone == _stone.stone;
}

bool F19::Stone::operator !=(Stone _stone) const {
	return !operator ==(_stone);
}

auto F19::Stone::reverse(Stone stone) -> Stone {
	if(stone == Stone::None ) return Stone::None;
	if(stone == Stone::Black) return Stone::White;
	if(stone == Stone::White) return Stone::Black;
	throw std::exception("Invalid stone type.");
}

auto F19::Stone::getReversed() const -> Stone {
	return reverse(*this);
}


std::istream& F19::operator >>(std::istream& is, Stone& stone) {
	Stone::Char c;
	is >> c;
	stone = (Stone)c;
	return is;
}

std::ostream& F19::operator <<(std::ostream& os, const Stone& stone) {
	return os << stone.getChar();
}

//bool F19::Stone::isNone() const {
//	return stone == None;
//}
//
//bool F19::Stone::isBlack() const {
//	return stone == Black;
//}
//
//bool F19::Stone::isWhite() const {
//	return stone == White;
//}
//
//bool F19::Stone::isNoneOrBlack() const {
//	return isNone() || isBlack();
//}
//
//bool F19::Stone::isNoneOrWhite() const {
//	return isNone() || isWhite();
//}
