#include "stone.h"
#include <stdexcept>

F19::Stone::Stone()
	: stone(None) {}

F19::Stone::Stone(StoneType stone)
	: stone(stone) {}

F19::Stone::Stone(char c) {
	if(c == (char)StoneChar::None ){
		stone = Stone::None;
	}else if(c == (char)StoneChar::Black){
		stone = Stone::Black;
	}else if(c == (char)StoneChar::White){
		stone = Stone::White;
	}else{
		throw std::exception("Invalid stone type.");
	}
}

char F19::Stone::getChar() const {
	if(stone == Stone::None ) return (char)StoneChar::None;
	if(stone == Stone::Black) return (char)StoneChar::Black;
	if(stone == Stone::White) return (char)StoneChar::White;
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

auto F19::Stone::reverse(Stone stone) -> F19::Stone {
	if(stone == Stone::None ) return Stone::None;
	if(stone == Stone::Black) return Stone::White;
	if(stone == Stone::White) return Stone::Black;
	throw std::exception("Invalid stone type.");
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
