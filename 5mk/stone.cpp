#include "stone.h"

F19::Stone::Stone(StoneType stone) : stone(stone) {}

F19::Stone::operator StoneType() const {
	return stone;
}