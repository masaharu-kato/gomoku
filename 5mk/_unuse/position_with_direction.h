#pragma once
#include "position.h"
#include "directions.h"

namespace Kato {

	class PositionWithDirection : public Position {
		Direction dir;

	public:
		PositionWithDirection(const Position& pos, Direction dir) : Position(pos), dir(dir) {}
	};

}
